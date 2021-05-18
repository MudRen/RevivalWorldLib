/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval_special.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-13
 * Note   : save_all 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
        標準 save_all 指令。
HELP;
	
void command(object me, string arg)
{
	int level;

        if( !is_command() ) return;

	level = to_int(arg);

	if( level < 0 || level > 5 )
		return tell(me, "儲存等級只能介於 0 至 5 之間。\n");

	SYSTEM_D->prepare_to_save_all(level);
}
