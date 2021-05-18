/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : get.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-10-10
 * Note   : get 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
將物品撿拾起來

get [數量|all] [物品] [排序]	- 將某數量或排序的某物品撿拾起來
get all from [某物]		- 將某物品上的所有東西撿拾起來
get autoloot			- 啟動與關閉自動撿拾屍體的功能

HELP;

private void do_command(object me, string arg)
{
	int i, mass, from_environment=1;
	object ob, target, env;
	string from, msg="";
	string option;
	int amount;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆想要我撿什麼東西？\n");
		else
			return tell(me, pnoun(2, me)+"想要撿什麼東西？\n");
	}

	if( arg == "autoloot" )
	{
		if( !query("auto/loot", me) )
		{
			set("auto/loot", 1, me);
			tell(me, "啟動自動撿拾屍體的功能。\n");
		}
		else
		{
			delete("auto/loot", me);
			tell(me, "關閉自動撿拾屍體的功能。\n");
		}
		me->save();
		return;
	}

	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	env = environment(me);

	if( sscanf(arg, "%s from %s", arg, from) == 2 )
	{
		if( !objectp(target = present(from)) )
			return tell(me, "這裡沒有 "+from+" 這個人或物品。\n");
		else
			from_environment = 0;
	}
	else target = env;

	if( objectp(target) && target->is_living() )
	{
		if( target->is_module_npc() )
		{
			if( query("boss", target) != me->query_id(1) )
				return tell(me, pnoun(2, me)+"沒有權利從"+target->query_idname()+"身上撿拾任何物品。\n");
		}
		else if( !target->is_dead() && !wizardp(me) ) 
			return tell(me, pnoun(2, me)+"沒有權利從"+target->query_idname()+"身上撿拾任何物品。\n");
	}

	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		if( option == "all") ;

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

	if( objectp(ob = present(arg, target)) )
	{
		if( me == ob )
			return tell(me, pnoun(2, me)+"如果真的可以把自己拿起來，請至巫師大廳領取十萬元獎金。\n");

		if( me == target )
			return tell(me, ob->query_idname()+"已經在"+pnoun(2, me)+"身上了！！。\n");

		if( userp(target) )
			return msg("$ME在$YOU身上東摸西摸，試著把"+QUANTITY_D->obj(ob, amount)+"給拿走！\n", me, target, 1);

		if( ob->is_living() )
			return msg("$ME在$YOU身上東摸西摸，試著找到施力點把$YOU給拿起來！！\n", me, ob,1);

		if( option != "all" && amount > ob->query_amount() )
		{
			if( me->is_npc() )
				return me->do_command("say "+"這裡的"+ob->query_idname()+"沒有那麼多個。\n");
			else
				return tell(me, "這裡的"+ob->query_idname()+"沒有那麼多個。\n");
		}

		if( query("mass", ob) == -1 )
		{
			msg("$ME隨手將"+QUANTITY_D->obj(ob, amount)+"一手舉起，只見$ME輕輕地說了一句「啊咧！好像太重了...」。\n",me,0,1);
			return me->faint();
		}

		if( query_temp("decorated", ob) )
			return tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的裝潢。\n");
		
		if( query_temp("activate", ob) )
			return tell(me, pnoun(2, me)+"無法撿起"+ob->query_idname()+"。\n");

		if( option == "all" )
		{
			amount = (me->query_max_loading() - me->query_loading()) / (query("mass", ob)||1);

			if( amount > ob->query_amount() )
				amount = ob->query_amount();

			if( amount == 0 )
				return tell(me, pnoun(2, me)+"拿不動"+ob->query_idname()+"了。\n");
		}

		if( objectp(target) && target->is_living() )
		{
			if( ob->is_keeping() || ob->is_equipping() )
				return tell(me, pnoun(2, me)+"無法從"+target->query_idname()+"上撿起"+ob->query_idname()+"。\n");
		}

		if( target->is_dead() && !target->is_module_npc() && !wizardp(me) )
		{
			CHANNEL_D->channel_broadcast("nch", me->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");

			if( !sizeof(all_inventory(target)) )
				target->corpse_disappear();			
		}

		if( me->available_get(ob, amount) )
		{
			msg("$ME把"+QUANTITY_D->obj(ob, amount)+(from_environment?"":"從"+target->query_idname()+"中")+"撿了起來。\n", me,0,1);
			ob->before_get(amount);
			ob->move(me, amount);
			me->delay_save(300);
			return;
		}
		else
			return msg("$ME把"+QUANTITY_D->obj(ob, amount)+(from_environment?"":"從"+target->query_idname()+"中")+"拿了起來，只聽到大腿一聲喀啦，大腿骨快斷了！又趕緊放手。\n",me,0,1);
	}

	if( lower_case(arg) == "all" )
	{
		object *all_ob;

		if( target->is_living() )
			all_ob = all_inventory(target);
		else
			all_ob = filter_array(present_objects(me), (: !$1->is_living() && query("mass",$1) >= 0 :));

		if( userp(target) && (!wizardp(me) || SECURE_D->level_num(me) < SECURE_D->level_num(target)) )
			return msg("$ME在$YOU身上東摸西摸，試著把$YOU所有的東西全部拿走！\n",me,target,1);

		if( sizeof(all_ob) )
		{
			int too_heavy, loading, max_loading, special;

			max_loading = me->query_max_loading();

			foreach(ob in all_ob)
			{
				mass = query("mass",ob);
				special = query("special", ob);

				if( mass == -1 || ob->is_keeping() || ob->is_equipping() || query_temp("decorated", ob) || query_temp("activate", ob)) continue;

				amount = ob->query_amount();

				loading = max_loading - me->query_loading();

				if( objectp(target) )
					target->unequip(ob, ref i);

				if( loading < amount * mass )
				{
					if( loading < mass || !me->available_get(ob, loading/mass) ) continue;

					msg += HIR"．"NOR+QUANTITY_D->obj(ob, loading/mass, 1)+"\n";

					if( target->is_living() && target->is_dead() && !target->is_module_npc() && !wizardp(me) )
					{
						CHANNEL_D->channel_broadcast("nch", me->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");

						if( special )
						{
							if( me->is_module_npc() )
							{
								string boss = query("boss", me);
									
								if( stringp(boss) && objectp(find_player(boss)) )
									CHANNEL_D->channel_broadcast("combat", find_player(boss)->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");
							}
							else
								CHANNEL_D->channel_broadcast("combat", me->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");
						}
					}
					
					ob->before_get(loading/mass);	
					ob->move(me, loading/mass);

					too_heavy = 1;
				}
				else
				{
					if( !me->available_get(ob, amount) ) continue;

					msg += HIR"．"NOR+trim(QUANTITY_D->obj(ob, amount))+"\n";

					if( target->is_dead() && !target->is_module_npc() && !wizardp(me) )
					{
						CHANNEL_D->channel_broadcast("nch", me->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");

						if( special )
						{
							if( me->is_module_npc() )
							{
								string boss = query("boss", me);
									
								if( stringp(boss) && objectp(find_player(boss)) )
									CHANNEL_D->channel_broadcast("combat", find_player(boss)->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");
							}
							else
								CHANNEL_D->channel_broadcast("combat", me->query_idname()+"從"+target->query_idname()+"身上撿起了"+ob->query_idname()+"。");
						}

						if( !sizeof(all_inventory(target)) )
							target->corpse_disappear();
					}
					ob->before_get(amount);
					ob->move(me , amount);
				}
			}

			if( msg == "" )
			{
				if( me->is_npc() )
					return me->do_command("say 我撿不起來任何的東西。\n");
				else
					return tell(me, pnoun(2, me)+"撿不起來任何的東西。\n");
			}

			me->delay_save(300);
			msg("$ME把"+(from_environment?"地上撿得動的東西都撿了起來":"$YOU身上能拿的東西通通拿了出來")+"。\n"+msg+"\n", me, target,1);

			if( too_heavy ) tell(me, "剩下的東西太重拿不動了。\n\n");

			return;
		}

		tell(me, (from_environment?"這個地方":target->query_idname())+"沒有任何物品可以撿耶...。\n");

		if( objectp(target) &&  target->is_dead() && !target->is_module_npc() && !sizeof(all_inventory(target)) )
			target->corpse_disappear();

		return;
	}

	if( me->is_npc() )
		return me->do_command("say 這裡沒有 "+arg+" 這件物品。\n");
	else
		return tell(me, "這裡沒有 "+arg+" 這件物品。\n");
}
