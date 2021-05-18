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

#include <ansi.h>
#include <daemon.h>

#define MAP_ID		"godsland"
#define MAP_NUM		0

inherit __DIR__"_mapnpc.c";

array npc =
({
	(["group":(["/obj/npc/boss/sky_1st_statue" : 1,]), "x":({ 72, 72 }), "y":({ 91, 91 }), "move_speed" : -1, "reborn_time" : 300, ]),
	(["group":(["/obj/npc/boss/sky_2nd_statue" : 1,]), "x":({ 75, 75 }), "y":({ 91, 91 }), "move_speed" : -1, "reborn_time" : 300, ]),
	(["group":(["/obj/npc/boss/sky_3rd_statue" : 1,]), "x":({ 78, 78 }), "y":({ 91, 91 }), "move_speed" : -1, "reborn_time" : 300, ]),
});

array query_npc_data()
{
	return npc;
}

void create()
{
	::load_mapnpc(ref npc, MAP_ID, MAP_NUM, 1);
}

int remove()
{
	::unload_mapnpc(ref npc);	
}

void heart_beat()
{
	::mapnpc_heart_beat(ref npc, MAP_ID, MAP_NUM);
}
