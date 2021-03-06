/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _import_action.c
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

int over_capacity(object env, string database, int amount, int capacity);
void input_object(object env, string database, string basename, int amount);

// 物品輸入指令
void do_import(object me, string arg, string database, int capacity)
{
	int amount;
	string option;
	string basename, env_city;
	object ob, env, master, *obs;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要輸入什麼物品？\n");

	env = environment(me);
	master = env->query_master();

	if( !userp(me) ) return;

	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"無法在別人的建築中輸入物品。\n");

	env_city = env->query_city();
	
	if( arg == "all" )
		obs = filter_array(all_inventory(me)+all_inventory(env), (: !$1->is_keeping() && !query("flag/no_import", $1) && count(query("value", $1),">",0) :));		
	else
	{
		if( sscanf(arg, "%s %s", option, arg) == 2 )
		{
			if( option == "all" ) ;
			// 如果 option 並不是數字單位
			else if( !big_number_check(option) )
			{
				arg = option + " " + arg;
				amount = 1;	
			}	
			else if( to_int(option) < 1 )
				amount = 1;
			else
				amount = to_int(option);
		}
		else amount = 1;
	
		if( !objectp(ob = present(arg, me) || present(arg, env)) )
			return tell(me, "這附近並沒有 "+arg+" 這樣東西。\n");
	
		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留(keep)狀態。\n");
		
		if( query("flag/no_import", ob) )
			return tell(me, ob->query_idname()+"不允許輸入。\n");

		if( query_temp("decorated", ob) )
			return tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的裝潢。\n");

		if( option == "all" )
			amount = ob->query_amount();

		if( amount > ob->query_amount() )
			return tell(me, "這附近並沒有那麼多"+(query("unit", ob)||"個")+ob->query_idname()+"。\n");
	
		if( ob->is_living() )
			return tell(me, pnoun(2, me)+"無法輸入一個生物！\n");
			
		obs = ({ ob });
	}
	
	if( !sizeof(obs) )
		return tell(me, "沒有任何可以輸入的物品。\n");
	
	foreach( ob in obs )
	{
		basename = base_name(ob);
		
		if( arg == "all" )
			amount = ob->query_amount();

		if( count(query("value", ob), "<", 1) )
		{
			tell(me, ob->query_idname()+"沒有價值，無法輸入。\n");
			continue;
		}

		if( query_temp("decorated", ob) )
		{
			tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的裝潢。\n");
			continue;
		}

		if( !undefinedp(query_temp("endurance", ob)) && query_temp("endurance", ob) <= 0 )
		{
			tell(me, ob->query_idname()+"已經損壞了，無法輸入。\n");
			continue;
		}

		// 超過容量
		if( over_capacity(master, database, amount, capacity) )
		{
			master->delay_save(300);
			return tell(me, env->query_room_name()+"無法再容納那麼多物品了。\n");
		}
		
		input_object(master, database, basename, amount);
		
		msg("$ME將"+QUANTITY_D->obj(ob, amount)+"整理後輸入"+env->query_room_name()+"。\n"NOR, me, 0, 1);

		destruct(ob, amount);
	}
	
	master->delay_save(300);
}
