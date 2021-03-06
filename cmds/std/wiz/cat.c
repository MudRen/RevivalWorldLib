/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cat.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-10
 * Note   : cat 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <message.h>
#include <runtime_config.h>

inherit COMMAND;

string help = @HELP
	標準 cat 指令
HELP;

private void command(object me, string arg)
{
	object ob;
	
	if( !is_command() ) return;
	
	if( !arg )
		return tell(me, "請輸入檔名。\n", CMDMSG);

	if( objectp(ob = present(arg)) )
		arg = base_name(ob);
	else
		arg = resolve_path(me, arg); 
	
	intact_file_name(ref arg);
	
	if( !file_exist(arg) )
		return tell(me, "沒有 "+arg+" 這個檔案。\n", CMDMSG);

	if( file_size(arg) > get_config(__MAX_READ_FILE_SIZE__) )
		return tell(me, "檔案過大( > "+get_config(__MAX_READ_FILE_SIZE__)+" )，請使用 more 檢視。\n", CMDMSG);

	set("current_work/file", arg, me);
    	return tell(me, read_file(arg)+"\n\n", CMDMSG);
}
