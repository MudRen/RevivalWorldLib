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

#define MAP_ID		"memoryland"
#define MAP_NUM		8

inherit __DIR__"_mapnpc.c";

array npc =
({
	// 茵玲羽
	([
		"group":
		([
			"/quest/yin_ling_yu/yin_ling_yu" : 1,
		]),
		"x" 		: ({ 11, 11 }),
		"y"		: ({ 53, 53 }),
		"move_speed" 	: 0,
		"reborn_time"	: 1,
	]),
	
	// 黑寡婦
	(["group":(["/obj/npc/boss/latrodectus_mactans" : 1,]), "x":({ 21, 23 }), "y":({ 29, 31 }), "move_speed" : 600,"reborn_time" : range_random(1800, 3600), ]),
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
