/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-13
 * Note   : 地圖系統上的 NPC 定位與移動
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define MAP_ID		"blueflowers"
#define MAP_NUM		0

inherit __DIR__"_mapnpc.c";

array npc =
({
	// 老太婆
	([
		"group":
		([
			"/quest/yin_ling_yu/grandam" : 1,
		]),
		"x" 		: ({ 47, 47 }),
		"y"		: ({ 51, 51 }),
		"move_speed" 	: 0,
		"reborn_time"	: 1,
	]),
});

array query_npc_data()
{
	return npc;
}

void create()
{
	::load_mapnpc(ref npc, MAP_ID, MAP_NUM);
}

int remove()
{
	::unload_mapnpc(ref npc);	
}

void heart_beat()
{
	::mapnpc_heart_beat(ref npc, MAP_ID, MAP_NUM);
}
