/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : quit 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
離線指令

離開重生的世界
HELP;

private void do_command(object me, string arg)
{
	if( COMBAT_D->in_fight(me) )
		return tell(me, pnoun(2, me)+"正在戰鬥中，無法離線！\n");

	if( me != this_player(1) ) 
		tell(me, this_player(1)->query_idname()+"強迫"+pnoun(2, me)+"離開遊戲。\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

     	catch{COMMAND_D->find_command_object("snoop")->remove_user(me);};

	set_temp("quiting", 1, me);

	LOGOUT_D->quit(me, arg);
}
