/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : waitback.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-9
 * Note   : waitback 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
等待指令

這個指令可以讓您等待閒置中的使用者，直到該使用者開始活動時通知您。

指令格式:
waitback <使用者代號> <留言>

相關指令: beep
HELP;

private void do_command(object me, string arg)
{
	object target;
	mapping waitback_list, info;
	string msg;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要等誰回神？\n", CMDMSG);

	sscanf(arg, "%s %s", arg, msg);
	
	if( !objectp(target = find_player(arg)) )
		return tell(me, "沒有 "+arg+" 這位玩家。\n", CMDMSG);

	if( target == me )
		return tell(me, "等"+pnoun(2, me)+"自己回神？？\n", CMDMSG);

	info = allocate_mapping(2);
	info["time"] = time();
	info["msg"] = msg;
	
	if( mapp(waitback_list = query_temp("waitback_list", target)) ) 
		set_temp("waitback_list", waitback_list + ([ me : info ]), target);
	else
		set_temp("waitback_list", ([ me : info ]), target);	
	
	msg(HIG"$ME"HIG"開始等待著"HIG"$YOU回神。\n"NOR, me, target, 1);
	tell(target, "\a", 0);
}