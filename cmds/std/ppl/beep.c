/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : beep.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-10
 * Note   : beep ���O
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
�I�s���O

�o�ӫ��O�i�H�ΨөI�s��L�ϥΪ̡A�Y�ӨϥΪ̪� Client �䴩�h�|���񴣥��n

���O�榡:
beep <�ϥΪ̥N��>

�������O: waitback
HELP;

private void do_command(object me, string arg)
{
	object ob;
    	string *ignore;
   
    	if( !arg )
    		return tell(me, pnoun(2, me)+"�Q�I�s����u�W���a�H\n");
   
    	if( !(ob = find_player(arg)) ) 
    		return tell(me, "�S���o���a�C\n");
    	
   	if( arrayp(ignore = query("ignore", ob)) && member_array(me->query_id(1), ignore) != -1 )
   		return tell(me, ob->query_idname()+"�w�N"+pnoun(2, me)+"�[�J"+pnoun(3, ob)+"���¦W�椤�A�]��"+pnoun(2, me)+"�ثe�T��I�s"+pnoun(3, ob)+"�C\n");		
   
    	msg("$ME���$YOU�������͹ͥs�A�n���F�C\n", me, ob, 1);
    	tell(ob,  "\a", 0);
}