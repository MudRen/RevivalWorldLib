/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : give.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-01-01
 * Note   : give 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <object.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
將物品或金錢給予某人

give [某人] [數量|all] [物品] [排序]		- 將某數量或排序的某物品給予某人
give [數量|all] [物品] [排序] to [某人]		- 將某數量或排序的某物品給予某人
HELP;


private void do_give(object me, object target, string arg)
{
	int mass;
	object ob;
	mixed option;
	int amount;

	if( target == me )
		return tell(me, "東西已經在"+pnoun(2, me)+"的身上了！\n");
	
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( sscanf(arg, "$%s %s", arg, option) == 2 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法給予金錢。\n");
	
		if( !MONEY_D->money_unit_exist(arg) )
			return tell(me, "並沒有 "+arg+" 這種貨幣。\n");
		
		option = to_int(big_number_check(option));
		
		if( !option )
			return tell(me, "請輸入正確的數字。\n");
		
		if( option <= 0 )
			return tell(me, "金額必須要大於零。\n");
		
		if( !me->spend_money(arg, option) )
			return tell(me, pnoun(2, me)+"身上沒有那麼多的 $"+upper_case(arg)+" 錢。\n");
			
		target->earn_money(arg, option);
		
		if( !wizardp(target) )
		{
			log_file("command/give", me->query_idname()+"給"+target->query_idname()+" "+money(arg, option));
			CHANNEL_D->channel_broadcast("nch", me->query_idname()+"給"+target->query_idname()+" "+money(arg, option));
		}
		
		if( target->is_npc() )
			call_out((: call_other :), 0, target, "reply_get_money", me, arg, option);

		return msg("$ME拿給$YOU "HIY+money(arg, option)+NOR"。\n", me, target, 1);
	}
		
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
	
	if( objectp(ob = present(arg, me)) )
	{
		string ob_idname = ob->query_idname(), unit = query("unit", ob)||"個";
		
		if( option == "all" )
			amount = ob->query_amount();

		if( amount > ob->query_amount() )
			return tell(me, pnoun(2, me)+"身上沒有那麼多"+unit+ob_idname+"。\n");
		
		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留(keep)狀態。\n");

		if( query("flag/no_give", ob) )
			return tell(me, ob->query_idname()+"不允許交易。\n");

		if( target->available_get(ob, amount) )
		{
			msg("$ME把"+QUANTITY_D->obj(ob, amount)+"拿給$YOU。\n", me, target, 1);
			ob->move(target, amount);
			me->delay_save(300);
			target->delay_save(300);
		}
		else
			msg("$ME想把"+QUANTITY_D->obj(ob, amount)+"拿給$YOU，但對$YOU而言似乎太重了。\n", me, target, 0);
		
		return;
	}
	
	if( lower_case(arg) == "all" )
	{		
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			int longest, too_heavy, loading, max_loading;
			string msg="", str;
		
			max_loading = target->query_max_loading();
			
			foreach(ob in all_ob)
			{
				mass = query("mass", ob);
				
				if( mass == -1 || ob->is_keeping() || query("flag/no_give", ob) ) continue;
				
				amount = ob->query_amount();
				
				loading = max_loading - target->query_loading();
				
				if( loading < amount * mass )
				{
					if( loading < mass || !target->available_get(ob, loading/mass) ) continue;
					
					str = (msg==""?"「 ":"   ")+QUANTITY_D->obj(ob, loading/mass, 1)+"\n";
					ob->move(target, loading/mass);
					too_heavy = 1;
				}
				else
				{
					if( !target->available_get(ob, amount) ) continue;

					str = (msg==""?"「 ":"   ")+QUANTITY_D->obj(ob, amount, 1)+"\n";
					ob->move(target, amount);
				}
				
				
				msg += str;	
				
				if( noansi_strlen(str) > longest ) longest = noansi_strlen(str);
			}
			
			if( msg == "" )
				return msg("$ME試著給$YOU所有的物品，但$YOU身上負重太重了，什麼都拿不動了。\n", me, target, 0);
			
			msg = msg[0..<2]+repeat_string(" ", longest-noansi_strlen(str)+1 );
			
			msg("$ME把身上能給的東西全給了$YOU。\n"+msg+"」\n\n", me, target, 1);
			
			me->delay_save(300);
			target->delay_save(300);
					
			if( too_heavy )
				msg("剩下的東西似乎太重$YOU拿不動了。\n", me, target, 0);
			
			return;
		}
		return tell(me, pnoun(2, me)+"身上沒有任何物品可以給"+target->query_idname()+"。\n");
	}
	return tell(me, pnoun(2, me)+"身上沒有 "+arg+" 這件物品。\n");
}

private void confirm_give(string input, object me, object target, string arg)
{
	if( input && lower_case(input)[0] == 'y' )
		do_give(me, target, arg);
	else
		tell(me, pnoun(2, me)+"取消將物品拿給"+target->query_idname()+"的動作。\n");
		
	me->finish_input();
}

private void do_command(object me, string arg)
{
	object target;
	string target_name;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要給誰什麼東西？\n");

	if( sscanf(arg, "%s to %s", arg, target_name) != 2 )
		sscanf(arg, "%s %s", target_name, arg);
	
	if( !arg || !target_name )
		return tell(me, pnoun(2, me)+"想要給誰什麼東西？\n");

	if( !objectp(target = present(target_name, environment(me))) )
		return tell(me, "這裡沒有 "+target_name+" 這個人。\n");

	if( !target->is_living() )
		return tell(me, pnoun(2, me)+"不能給"+target->query_idname()+"任何東西。\n");
		
	if( userp(target) )
		do_give(me, target, arg);
	else
	{
		if( target->is_module_npc() )
		{
			if( query("boss", target) == me->query_id(1) )
				tell(me, HIY"注意：若是給予員工金錢將無法再取回\n"NOR+pnoun(2, me)+"確定要將物品拿給"+target->query_idname()+"嗎(Yes/No)？\n");
			else
				tell(me, HIY"注意：給予的物品或金錢將無法再取回\n"NOR+pnoun(2, me)+"確定要將物品拿給"+target->query_idname()+"嗎(Yes/No)？\n");		
		}
		else if( !function_exists("reply_get_object", target) && !function_exists("reply_get_money", target) )
			return tell(me, pnoun(2, me)+"不能給"+target->query_idname()+"任何東西。\n");
		else
			tell(me, HIY"注意：給予的物品或金錢將無法再取回\n"NOR+pnoun(2, me)+"確定要將物品拿給"+target->query_idname()+"嗎(Yes/No)？\n");	

		input_to((: confirm_give :), me, target, arg);
	}
}