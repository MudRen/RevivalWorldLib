/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _export_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void output_object(object env, string database, string basename, int amount);

// 物品輸出指令
void do_export(object me, string arg, string database)
{
	int i, j, which = 1;
	int allow_mayor_levy = 0, amount = 1, pamount;
	object env, master, ob, newob;
	mapping objects;
	string env_city, objectname, basename, option;
	string *shelfsort, shelf;
	
	if( !userp(me) ) return;
			
	env = environment(me);
	master = env->query_master();
	env_city = env->query_city();

	if( query("owner", env) != me->query_id(1) && !wizardp(me) )
	{
		if( CITY_D->is_mayor(env_city, me) )
		{
			object user = load_user(query("owner", env));
			
			// 兩個月未上線
			if( time() - query("last_on/time", user) > 60*60*24*60 )
			{
				if( !userp(user) )
					destruct(user);
				allow_mayor_levy = 1;
			}
			else
			{
				if( !userp(user) )
					destruct(user);					
				return tell(me, pnoun(2, me)+"無法在別人的建築中輸出物品。\n");
			}
		}
		else
			return tell(me, pnoun(2, me)+"無法在別人的建築中輸出物品。\n");
	}

	if( !arg )
		return tell(me, pnoun(2, me)+"想要輸出什麼物品？\n");
	
	sscanf(arg, "%s %s", option, objectname);
	
	if( !objectname || (option != "all" && !(amount = to_int(option))) )
	{
		objectname = arg;
		amount = 1;
	}
	
	if( sscanf( objectname, "%s %d", arg, which ) == 2 )
	{
		if( which < 1 ) which = 1;
		objectname = arg;
	}

	objects = query(database, master);
	
	if( !sizeof(objects) )
		return tell(me, env->query_room_name()+"裡並沒有任何物品。\n");

	shelfsort = keys(objects) | (query("shelfsort", master) || allocate(0));

	foreach( shelf in shelfsort )
	{
		for(i=0;i<sizeof(objects[shelf]);i+=2)
		{
			basename = objects[shelf][i];
			pamount = to_int(objects[shelf][i+1]);
			
			if( catch(ob = load_object(basename)) )
				continue;

			// 檔案已經消失, 刪除資料
			if( !objectp(ob) && !file_exists(basename) )
			{
				output_object(master, database, basename, -3);
				continue;
			}

			j++;
	
			if( j == to_int(objectname) || objectname == "all" || (ob->query_id(1) == lower_case(objectname) && !(--which)) )
			{
				if( option == "all" || objectname == "all" )
					amount = pamount;
				else if( amount > pamount )
					return tell(me, "這裡並沒有這麼多"+(query("unit", ob)||"個")+ob->query_idname()+"。\n");
				else if( amount < 1 )
					return tell(me, "請輸入正確的輸出數量。\n");
				
				if( allow_mayor_levy && !query("material", ob) )
					return tell(me, pnoun(2, me)+"無法輸出"+ob->query_idname()+"。\n");

				newob = new(basename);
				
				if( !newob->no_amount() )
					newob->set_amount(amount);
				else if( amount > 500 )
					amount = 500;

				msg("$ME將"+QUANTITY_D->obj(ob, amount)+"自"+env->query_room_name()+"輸出。\n",me,0,1);

				if( !me->available_get(newob, amount) )
				{
					if( amount == 1 )
					{
						tell(me, pnoun(2, me)+"身上的物品太多或太重無法再拿更多東西了，只好先放在地上。\n");
						newob->move_to_environment(me);
					}
					else
					{
						tell(me, pnoun(2, me)+"身上的物品太多或太重無法再拿更多東西了。\n");
						destruct(newob);
						master->delay_save(300);
						return;
					}
				}
				else
					newob->move(me, amount);
	
				output_object(master, database, basename, amount);
				
				if( objectname != "all" )
				{
					master->delay_save(300);
					return;
				}
			}
		}
	}
	
	if( objectname != "all" )
		return tell(me, "這裡並沒有 "+objectname+" 這種商品。\n");
	
	master->delay_save(300);
}
