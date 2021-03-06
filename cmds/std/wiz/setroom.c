/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : setroom.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-5-8
 * Note   : setroom 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <inherit.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 setroom 指令
HELP;

private void command(object me, string arg)
{	
	string file;
	object room;
	
	if( !is_command() ) return;
	
	if( !arg || arg == "")
		return tell(me, "請輸入你要設定房間之檔案名稱或路徑。\n", CMDMSG);
	
	if( sscanf(arg, "%s %s", file, arg) != 2 )
		return tell(me, "請輸入你要設定房間之檔案名稱或路徑與設定選項。\n", CMDMSG);

	
	file = resolve_path(me, file);

	file = file[<2..<1] == ".o" ? file : file + ".o";
	
	if( file_size(file) < 0 )
		return tell(me, "你無法設定 "+file+" 這個檔案。\n", CMDMSG);

	room = load_object(file[0..<3]);
	me->edit( (: call_other, __FILE__, "set_description", me, arg, room :) );
}

void set_description(object me, string arg, object room, string str)
{
	set(arg, str, room);
	tell(me, "設定房間 "+base_name(room)+" 之 "+arg+" 為 "+str+"。\n", CMDMSG);
	room->save();
}