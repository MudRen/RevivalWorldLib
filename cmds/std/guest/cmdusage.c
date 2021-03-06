/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cmdusage.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-08-27
 * Note   : cmdusage 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 cmdusage 指令。
HELP;

private void command(object me, string arg)
{
	mapping cmdrecord = COMMAND_D->query_user_command_record();
	string *keys = keys(cmdrecord);
	string msg;

	if( !is_command() ) return;
	
	keys = sort_array(keys, (: count($(cmdrecord)[$1], "<", $(cmdrecord)[$2]) ? 1 : -1 :));
	
	msg = sprintf("%-30s%s\n", "指令", "下達次數");
	
	foreach(string key in keys)
		msg += sprintf("%-30s%s\n", key, cmdrecord[key]);
		
	return me->more(msg);
}