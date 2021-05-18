/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : check.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-20
 * Note   : check 指令
 * Update :
 *  o 2000-00-00  
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
check		- 在地圖系統裡檢查地上物品的狀態
HELP;

#define LOG			"daemon/map_d"

private void do_command(object me, string arg)
{
	object env = environment(me);
	array loc = query_temp("location", me);
	object* inv;
	
	if( !env->is_maproom() || !arrayp(loc) )
		return tell(me, pnoun(2, me)+"只能在地圖系統上執行這個檢查命令。\n");
	
	inv = MAP_D->coordinate_inventory(loc);
	
	tell(me, "在這個座標上共有 "+sizeof(inv)+" 件物品：\n");

	foreach(object ob in inv)
	{
		if( !query_temp("location", ob) )
		{
			tell(me, ob->query_idname()+"喪失座標資料，已紀錄並通知巫師處理。\n");
			log_file(LOG, "地圖系統中的 "+file_name(ob)+" 喪失座標資料。\n");
			CHANNEL_D->channel_broadcast("sys", "地圖系統中的 "+file_name(ob)+" 喪失座標資料。");
		}
		else if( !environment(ob) )
		{
			tell(me, ob->query_idname()+"喪失環境資料，已紀錄並通知巫師處理。\n");
			log_file(LOG, "地圖系統中的 "+file_name(ob)+" 喪失環境資料。\n");
			CHANNEL_D->channel_broadcast("sys", "地圖系統中的 "+file_name(ob)+" 喪失環境資料。");
		}
		else
			tell(me, ob->query_idname()+"的座標資料一切正常。\n");
	}
}
