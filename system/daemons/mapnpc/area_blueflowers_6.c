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
#define MAP_NUM		6

inherit __DIR__"_mapnpc.c";

array npc =
({
	// 強尼‧巴頓
	([
		"group":
		([
			"/quest/bohr/john_barton" : 1,
		]),
		"x" 		: ({ 53, 53 }),
		"y"		: ({ 53, 53 }),
		"move_speed" 	: 0,
		"reborn_time"	: 1,
	]),
	
	// 臺灣黑熊
	(["group":(["/obj/npc/boss/ursus_thibetanus_formosanus" : 1,]), "x":({ 82, 84 }), "y":({ 34, 36 }), "move_speed" : 600,"reborn_time" : range_random(180, 360), ]),
		
	// 工人領班
	(["group":(["/quest/suphia_and_benjamin/npc/foreman" : 1,]), "x":({ 69, 75 }), "y":({ 31, 35 }), "move_speed" : 180, "reborn_time" : 180, ]),
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
