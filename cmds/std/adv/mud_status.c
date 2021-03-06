/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mud_status.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : mud_status 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        標準 mud_status 指令。
HELP;

private void command(object me, string arg)
{
	if( !is_command() ) return;
	
	tell(me, "資料型態                            數量    Bytes\n"+mud_status(), CMDMSG);
}
