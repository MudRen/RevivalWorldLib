/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : more.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : more 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <runtime_config.h>

#define LINE_NUMBER	1

inherit COMMAND;

string help = @HELP
	標準 more 指令。
HELP;

private void command(object me, string arg)
{
	string *parse;
	int flag, line = 0;
	object ob;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "請輸入檔名。\n");

	parse = single_quote_parse(arg);
	
	if( sizeof(parse) >= 2 )
	{		
		if( parse[0] == "-n" )
		{
			arg = parse[1];
			flag |= LINE_NUMBER;
		}
		else 
			arg = parse[0];
			
		if( to_int(parse[<1]) )
			line = to_int(parse[<1]);
	}
	else arg = parse[0];

	if( objectp(ob = present(arg)) )
		arg = base_name(ob);
	else
		arg = resolve_path(me, arg); 
	
	intact_file_name(ref arg);
	
	if( !file_exist(arg) )
		return tell(me, "沒有 "+arg+" 這個檔案。\n");

	if( file_size(arg) > get_config(__MAX_STRING_LENGTH__) )
		return tell(me, "檔案過大( > "+get_config(__MAX_STRING_LENGTH__)+" )，請使用 tail 檢視。\n");

	set("current_work/file", arg, me);
	me->more_file(arg, line, flag,"最近更新 "NOR WHT"-"HIW+TIME_D->replace_ctime(stat(arg)[1])+NOR WHT"-"NOR+"\n");	
}