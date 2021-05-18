/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : eval_special.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-13
 * Note   : save_all ���O�C
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
        �з� save_all ���O�C
HELP;
	
void command(object me, string arg)
{
	int level;

        if( !is_command() ) return;

	level = to_int(arg);

	if( level < 0 || level > 5 )
		return tell(me, "�x�s���ťu�श�� 0 �� 5 �����C\n");

	SYSTEM_D->prepare_to_save_all(level);
}
