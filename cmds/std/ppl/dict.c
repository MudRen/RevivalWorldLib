/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dict.c
 * Author : Msr@RevivalWorld
 * Date   : 2005-07-21
 * Note   : dict 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP

可以讓您即時翻譯
中 <-> 英


HELP;

nosave string *deer = ({ "deer", "elk", "wapiti" });

private void do_command(object me, string arg)
{
	if( !arg ) return tell( me, "請加入想要翻譯的字。\n", CMDMSG);

	if( member_array(arg, deer) != -1) {
		me->faint();
		return;
	}

	DICT_D->find_word( me, arg);
	return;
}
