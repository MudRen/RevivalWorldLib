/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : edit_help.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-9-15
 * Note   : edit_help 指令
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
	標準 edit_help 指令。
HELP;

private void modify_help(object me, string filename, string help)
{
	int i, start_line, end_line, size;
	string *file = explode(read_file(filename), "\n");
	
	size = sizeof(file);
	
	for(i=0;i<size;i++)
	{
		if( file[i][0..18] == "string help = @HELP" )
			start_line = i;
		else if( file[i][0..4] == "HELP;" )
		{
			end_line = i;
			break;
		}
	}

	file = file[0..start_line] + ({ kill_repeat_ansi(help+NOR) }) + file[end_line..];

	if( write_file(filename, implode(file, "\n"), 1) )
	{
		CHANNEL_D->channel_broadcast("sys", me->query_idname()+"編輯 "+filename+" 說明檔。");
		tell(me, "修改 "+filename+" 說明檔完成。\n", CMDMSG);
	}
	else
		tell(me, "寫入權限不足。\n", CMDMSG);
}


private void command(object me, string arg)
{
	string current_help;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "請輸入欲編輯說明檔之檔案名稱。\n", CMDMSG);

	arg = resolve_path(me, arg);
	
	intact_file_name(ref arg);
	
	if( !file_exist(arg) )
		return tell(me, "沒有 "+arg+" 這個檔案。\n", CMDMSG);
		
	if( !(current_help = fetch_variable("help", load_object(arg))) )
		return tell(me, arg+" 不需要編輯說明檔。\n", CMDMSG);
	
	tell(me, arg+" 之原本說明如下：\n"+current_help+"\n", CMDMSG);
	me->edit( (:modify_help, me, arg:) );
}