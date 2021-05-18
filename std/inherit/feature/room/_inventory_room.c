/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _inventory_room.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-09
 * Note   : 房間物品紀錄繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

int save_inventory()
{
	string basename;
	int amount;
	mapping objects = allocate_mapping(0);
	string *decorated_objects = allocate(0);

	if( !this_object()->query_database() ) return 0;

	foreach( object ob in all_inventory() )
	{
		if( ob->is_user_ob() ) continue;

		basename = base_name(ob);
		amount = ob->query_amount();

		if( query_temp("decorated", ob) )
			decorated_objects += ({ basename });
			
		objects[basename] += amount;
	}
			
	if( sizeof(objects) )
		set("objects", objects);
	else
		delete("objects");
		
	if( sizeof(decorated_objects) )
		set("decorated_objects", decorated_objects);
	else
		delete("decorated_objects");

	return 1;
}
