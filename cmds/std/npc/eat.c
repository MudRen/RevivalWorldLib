/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eat.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : eat 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
食用食物的指令
eat '物品'		- 將某個物品吃到肚子裡

HELP;

private void do_command(object me, string arg)
{
	int capacity;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆想要我吃些什麼？\n");
		else
			return tell(me, pnoun(2, me)+"想要吃些什麼？\n");
	}
	
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		if( me->is_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	ob = present(arg, me) || present(arg, environment(me));
	
	if( !objectp(ob) )
	{
		if( me->is_npc() )
			return me->do_command("say 附近並沒有 "+arg+" 這種食物。\n");
		else
			return tell(me, "附近並沒有 "+arg+" 這種食物。\n");
	}

	if( !ob->is_food() )
	{
		if( me->is_npc() )
			return me->do_command("say "+ob->query_idname()+"不是食物，無法食用。\n");
		else
			return tell(me, ob->query_idname()+"不是食物，無法食用。\n");
	}

	if( query_temp("decorated", ob) )
		return tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的裝潢。\n");

	if( query_temp("coldtime/eat", me) > time() )
		return tell(me, pnoun(2, me)+"還需要等待 "+(query_temp("coldtime/eat", me)-time())+" 秒後才能食用"+ob->query_idname()+"。\n");
	
	capacity = query("capacity", ob);

	if( capacity > 0 && !me->add_food(capacity) )
	{
		if( me->is_npc() )
			return me->do_command("say 我吃不下"+ob->query_idname()+"了！\n");
		else
			return tell(me, pnoun(2, me)+"吃不下"+ob->query_idname()+"了！\n");
	}

	ob->do_eat(me);

	if( query("coldtime", ob) )
		set_temp("coldtime/eat", time() + query("coldtime", ob), me);

	destruct(ob, 1);
}