/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quit.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : quit ���O
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
���u���O

���}���ͪ��@��
HELP;

private void do_command(object me, string arg)
{
	if( COMBAT_D->in_fight(me) )
		return tell(me, pnoun(2, me)+"���b�԰����A�L�k���u�I\n");

	if( me != this_player(1) ) 
		tell(me, this_player(1)->query_idname()+"�j��"+pnoun(2, me)+"���}�C���C\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

     	catch{COMMAND_D->find_command_object("snoop")->remove_user(me);};

	set_temp("quiting", 1, me);

	LOGOUT_D->quit(me, arg);
}
