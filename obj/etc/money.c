/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : money.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-15
 * Note   : 貨幣
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <type.h>
#include <inherit.h>
#include <daemon.h>
#include <location.h>

inherit STANDARD_OBJECT;

int is_money()
{
	return 1;
}

varargs void move(mixed where, string amount)
{	
	string money;
	string this_unit = query("unit");
	string this_base_name = base_name(this_object());
	
	object new_env;

	if( !where ) return;
	
	switch(typeof(where))
	{
		// 移動至地圖座標上
		case ARRAY:	
			{
				string code;
				
				code = where[CODE] = where[X]+"/"+where[Y]+"/"+where[Z]+"/"+where[PLACE]+"/"+where[NUM]+"/"+where[EXTRA];
				
				// 將所有數量轉移

				set_temp("location", copy(where));
					
				new_env = MAP_D->query_maproom(where);
						
				// 檢查座標處是否已有相同物件
				foreach( object ob in MAP_D->coordinate_inventory(where) )
				{			
					if( base_name(ob) == this_base_name && query("unit", ob) == this_unit )
					{
						money = count(copy(query("money", ob))||1, "+", query("money"));
						
						set("money", money , ob);
						
						ob->set_idname(this_unit+" money", HIY+"$"+this_unit+" "+NUMBER_D->number_symbol(money)+NOR);
						
						destruct(this_object());
						return;
					}
				}
				
				MAP_D->enter_coordinate(where, this_object());
				move_object(new_env);
	
				break;
			}
		case OBJECT:
				new_env = where;
				
				if( userp(new_env) )
				{
					if( amount && count(amount, "<", copy(query("money"))) )
					{
						new_env->earn_money(this_unit, amount);
						
						money = count(copy(query("money")), "-", amount);
						
						set("money", money);
						
						set_idname(this_unit+" money", HIY+"$"+this_unit+" "+NUMBER_D->number_symbol(money)+NOR);
					}
					else
					{
						new_env->earn_money(this_unit, copy(query("money")));
						MAP_D->leave_coordinate(query_temp("location"), this_object());
						destruct(this_object());
					}
					
					return;
				}
				
				foreach( object ob in all_inventory(new_env) )
				{
					if( base_name(ob) == this_base_name && query("unit", ob) == this_unit )
					{
						money = count(copy(query("money", ob))||1, "+", query("money"));
						
						set("money", money , ob);
							
						ob->set_idname(this_unit+" money", HIY+"$"+this_unit+" "+NUMBER_D->number_symbol(money)+NOR);
						MAP_D->leave_coordinate(query_temp("location"), this_object());
						destruct(this_object());
						return;
					}	
				}
	
				MAP_D->leave_coordinate(query_temp("location"), this_object());
				move_object(where);

				break;
				
		default:
				printf("無法找到或載入物件 %O 。\n", where);
				break;
	}	
}

int move_to_environment(object ob)
{
	object env = environment(ob);
	
	if( !env ) return 0;
	
	if( env->is_maproom() )
		move( query_temp("location", ob) );
	else
		move( env );
		
	return 1;
}

varargs string short(int need_quantity)
{
	return this_object()->query_idname();
}

varargs void create(string unit, string amount)
{
	if( clonep(this_object()) && (!unit || !amount ))
	{
		call_out( (: destruct, this_object() :), 1 ); 
		return;
	}

	set_idname(unit+" money", HIY+"$"+unit+" "+NUMBER_D->number_symbol(amount)+NOR);
	
	set("long", "白花花的鈔票。");
	set("unit", unit);
	set("money", amount);
}

void remove()
{
	//CHANNEL_D->channel_broadcast("nch", sprintf("call: %O \n", call_stack(1)));	
}