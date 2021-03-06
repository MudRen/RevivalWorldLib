/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : say.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : say 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    用來與同一個位置的玩家們談話
HELP;

private void do_command(object me, string arg)
{
	if( !arg )
		return msg("$ME自言自語不知道在說些什麼。\n", me, 0, 1, SAYMSG);

	msg("$ME說道: "HIG+arg+NOR"\n", me, 0, 1, SAYMSG);
	
	me->add_msg_log("say", pnoun(2, me)+"說道: "HIG+arg+NOR"\n");
	(present_objects(me)-({ me }))->add_msg_log("say", me->query_idname()+"說道: "HIG+arg+NOR"\n");
	
	//if( !wizardp(me) )
	//	log_file("command/say", me->query_idname(1)+"說道: "+arg, -1);
}