/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : data.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-4-21
 * Note   : data 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 data 指令。
HELP;

private void command(object me, string arg)
{
	string data;
	object ob;
	string res = resolve_path(me, arg);

	if( !is_command() ) return;

	if( !arg || arg == "me" ) 
		ob = me;

	else if( arg == "here" )
	{
		object env = environment(me);

		if( !env ) 
			return tell(me, "你並沒有處於任何的環境當中。\n", CMDMSG);

		if( env->is_maproom() ) 
		{
			array loc = query_temp("location", me);
			data = "你所在座標 "+query_temp("location",me)[CODE]+" 的資料如下\n";
			data += sprintf("%O\n",	(MAP_D->query_map_system(loc))->query_coor_data(loc));
			tell(me, data, CMDMSG);
			return;
		}
		else ob = env;
	}

	if( ob || objectp(ob = present(arg) || find_object(res) || load_object(res) || find_player(arg)) )
	{
		data = (ob->short()||query("short",ob)||file_name(ob))+"之物件資料庫資訊如下：\n";

		data += HIG+remove_ansi(sprintf("%O\n",  ob->query_database()||"無"))+NOR;
		data += NOR GRN+remove_ansi(sprintf("%O\n",  ob->query_temp_database()||"無"))+NOR;
	}
	else
		return tell(me, "沒有 "+arg+" 這個物件。\n", CMDMSG);

	me->more(data);

}
