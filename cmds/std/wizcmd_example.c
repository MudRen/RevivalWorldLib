/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-00-00
 * Note   : 巫師指令標準格式
 * Update :
 *  o 2005-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

inherit COMMAND;

string help = @HELP
	標準 wizcmd_example 指令。
HELP;

private void command(object me, string arg)
{
       	if( !is_command() ) return;
       	

}
