/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _inventory_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-15
 * Note   : 登入時載入身上物件繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#include <message.h>

#define AMOUNT		0
#define TEMP_DATABASE	1

void save_inventory()
{
	string basename;
	mapping save_inventory_list = allocate_mapping(0);

	foreach(object ob in all_inventory(this_object()))
	{
		basename = base_name(ob);
		
		if( !arrayp(save_inventory_list[basename]) )
			save_inventory_list[basename] = allocate(2);
		if( !arrayp(save_inventory_list[basename][TEMP_DATABASE]) )
			save_inventory_list[basename][TEMP_DATABASE] = allocate(0);

		if( ob->no_amount() )
			save_inventory_list[basename][AMOUNT]++;
		else
			save_inventory_list[basename][AMOUNT] += ob->query_amount();

		save_inventory_list[basename][TEMP_DATABASE] += ({ ob->query_temp_database() });
	}
	
	if( !sizeof(save_inventory_list) )
		delete("save_inventory");
	else
		set("save_inventory", save_inventory_list);
}

void restore_inventory()
{
	int index, int_amount, status;
	object ob, newob;
	mapping save_inventory_list = query("save_inventory");

	if( !sizeof(save_inventory_list) )
		return;
		
	foreach(string filename, array info in save_inventory_list)
	{
		index = 0;

		if( catch(ob = new(filename)) || !objectp(ob) )
		{
			tell(this_object(), pnoun(2, this_object())+"身上的東西似乎有些被系統回收了。\n");
			log_file("command/inventory", this_object()->query_id(1)+" 身上的 "+filename+" 無法載入。", -1);
			continue;
		}

		ob->set_temp_database(info[TEMP_DATABASE][index++]);

		int_amount = to_int(info[AMOUNT]);

		if( ob->no_amount() )
		{
			if( ob->is_equipping() )
				this_object()->equip(ob, ref status);

			ob->move(this_object());
			
			// 超過 100 就代表有問題
			if( int_amount > 100 )
				int_amount = 100;

			while(--int_amount)
			{
				catch(newob = new(filename));

				if( index < sizeof(info[TEMP_DATABASE]) )
					catch(newob->set_temp_database(info[TEMP_DATABASE][index++]));
				
				if( newob->is_equipping() )
					this_object()->equip(newob, ref status);
					
				newob->move(this_object());
			}	
			continue;
		}	
		else
		{
			ob->set_amount(int_amount);

			if( ob->is_equipping() )
				this_object()->equip(ob, ref status);
		
			ob->move(this_object());
		}
	}
}
