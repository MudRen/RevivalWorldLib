/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : mv 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
 
inherit COMMAND;

#define FROM		0
#define TO		1

string help = @HELP
	標準 mv 指令
HELP;

private void command(object me, string arg)
{
	string from, to, *parse;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "指令格式: mv <原檔名> <目標檔名> \n", CMDMSG);
	
	parse = single_quote_parse(arg);
	
	if( sizeof(parse) != 2 )
		return tell(me, "指令格式: mv <原檔名> <目標檔名> \n", CMDMSG);
	 
	from = resolve_path(me, parse[FROM]);
	to = resolve_path(me, parse[TO]);
	
	switch(file_size(from))
	{
		case -1:
			return tell(me, "沒有 "+from+" 這個檔案或目錄。\n", CMDMSG);
		case -2:
			if( from[<1] != '/' ) from += "/";
			
			switch( file_size(to) )
			{
				case -1:
					if( to[<1] != '/' ) to += "/";
					break;
				case -2:
					if( to[<1] != '/' ) to += "/";
					to += from[strsrch(from[0..<2], "/",-1)+1..];
					break;
				default:
					return tell(me, "無法覆蓋檔案 "+to+"。\n", CMDMSG);
			}
			break;
		default:
			switch( file_size(to) )
			{
				case -1:
					if( file_size(to[0..strsrch(to, "/",-1)]) != -2 )
						return tell(me, "沒有 "+to[0..strsrch(to, "/",-1)]+"這個目錄。\n", CMDMSG);
					break;
				case -2:
					if( to[<1] != '/' ) to += "/";
					to += from[strsrch(from, "/",-1)+1..];
					break;
				default:
					return tell(me, "無法覆蓋檔案 "+to+"。\n", CMDMSG);
			}
			break;
	}

	if( !rename(from, to) )	
		tell(me, "mv "+from+" -> "+to+" Done...。\n", CMDMSG);
	else
		tell("mv "+from+" -> "+to+" Failed...。\n", CMDMSG);
}
