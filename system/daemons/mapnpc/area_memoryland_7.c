/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mapnpc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 地圖系統上的 NPC 定位與移動
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define MAP_ID		"memoryland"
#define MAP_NUM		7

inherit __DIR__"_mapnpc.c";

array npc =
({
	// 雲豹
	(["group":(["/obj/npc/boss/neofelis_nebulosa" : 1,]), "x":({ 57, 59 }), "y":({ 20, 22 }), "move_speed" : 600,"reborn_time" : range_random(180, 360), ]),
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
