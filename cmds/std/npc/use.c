/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : use.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : use 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <use.h>

inherit COMMAND;

string help = @HELP
使用物品的指令
use '物品'		- 使用某個物品

HELP;

private void do_command(object me, string arg)
{
	string args;
	object ob;
	string basename;
	int coldtime;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆想要我使用什麼物品？\n");
		else
			return tell(me, pnoun(2, me)+"想要使用什麼物品？\n");
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
		sscanf(arg, "%s to %s", arg, args);
		ob = present(arg, me) || present(arg, environment(me));
	}

	if( !objectp(ob) )
	{
		if( me->is_npc() )
			return me->do_command("say 附近並沒有 "+arg+" 這種物品。\n");
		else
			return tell(me, "附近並沒有 "+arg+" 這種物品。\n");
	}

	if( !function_exists("do_use", ob) )
	{
		if( me->is_npc() )
			return me->do_command("say 我不知道如何使用"+ob->query_idname()+"。\n");
		else
			return tell(me, pnoun(2, me)+"不知道如何使用"+ob->query_idname()+"。\n");
	}

	if( query_temp("decorated", ob) )
		return tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的裝潢。\n");

	basename = base_name(ob);

	if( (coldtime = query("coldtime/"+basename, me)) > time() )
		return tell(me, pnoun(2, me)+"還需要等待 "+(coldtime - time())+" 秒後才能使用"+ob->query_idname()+"。\n");
	
	if( (coldtime = query("coldtime", ob)) )
		set("coldtime/"+basename, time() + coldtime, me);

	switch( ob->do_use(me, args) )
	{
		case 0:
			delete("coldtime/"+basename, me);
			break;
		case DESTRUCT_ONE:
			destruct(ob, 1);
			break;
		case DESTRUCT_ALL:
			destruct(ob);
			break;
	}
}
