/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _reset_room.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-4
 * Note   : ©Ð¶¡­«¸mÄ~©ÓÀÉ
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <message.h>

void reset_objects()
{
	string basename;
	mixed amount;
	mapping objects;
	string *decorated_objects;

	if( !this_object()->query_database() )	return;

	objects = (copy(query("objects")) || allocate_mapping(0)) + (copy(query("temp_objects")) || allocate_mapping(0));
	decorated_objects = copy(query("decorated_objects")) || allocate(0);

	if( sizeof(objects) )
	{
		object ob;
		object tmp_ob;
		int index;

		foreach( basename, amount in objects )
		{
			amount = to_int(amount);

			catch { ob = new(basename) || load_object(basename); };

			if( !objectp(ob) ) continue;
			
			if( !ob->no_amount() )
				ob->set_amount(amount);
			else
			while(--amount > 0)
			{
				tmp_ob = new(basename) || load_object(basename);
				
				index = member_array(basename, decorated_objects);
				
				if( index != -1 )
				{
					decorated_objects = decorated_objects[0..index-1] + decorated_objects[index+1..];
					set_temp("decorated", 1, tmp_ob);
				}

				tmp_ob->move(this_object());
			}
			
			index = member_array(basename, decorated_objects);
			if( index != -1 )
			{
				decorated_objects = decorated_objects[0..index-1] + decorated_objects[index+1..];
				set_temp("decorated", 1, ob);
			}

			ob->move(this_object());
		}
	}
	
	delete("temp_objects");
}

void reset()
{

}
