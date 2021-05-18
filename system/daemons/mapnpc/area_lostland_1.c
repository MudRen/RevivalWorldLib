/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mapnpc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-18
 * Note   : 地圖系統上的 NPC 定位與移動
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define MAP_ID		"lostland"
#define MAP_NUM		1

inherit __DIR__"_mapnpc.c";

int timetick;
int last_boss_pop_time;

array npc =
({
	(["group":(["/obj/npc/lostland/blainville's_beaked_whale" : 1,]), "x":({ 15, 15 }), "y":({ 50, 50 }), "move_speed" : -1, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/blainville's_beaked_whale" : 1,]), "x":({ 12, 12 }), "y":({ 81, 81 }), "move_speed" : -1, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/blainville's_beaked_whale" : 1,]), "x":({ 47, 47 }), "y":({ 48, 48 }), "move_speed" : -1, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/blainville's_beaked_whale" : 1,]), "x":({ 72, 72 }), "y":({ 95, 95 }), "move_speed" : -1, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/blainville's_beaked_whale" : 1,]), "x":({ 51, 51 }), "y":({ 15, 15 }), "move_speed" : -1, "reborn_time" : 600, ]),
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
	timetick++;

	::mapnpc_heart_beat(ref npc, MAP_ID, MAP_NUM);

	// 一分鐘檢查一次是否 pop BOSS
	if( !(timetick % 60) )
	{
		object ob;
		mapping data;

		foreach(data in npc)
			foreach(ob in data["objects"])
				if( objectp(ob) )
					return;
	
		// 15min pop 一次
		if( last_boss_pop_time < time() - 900 && sizeof(children("/obj/npc/lostland/nessiteras_rhombopteryx")) <= 1 )
		{
			ob = new("/obj/npc/lostland/nessiteras_rhombopteryx");
			
			CHANNEL_D->channel_broadcast("combat", ob->short()+"受到"HIB"深海的"NOR BLU"召喚"NOR"，出現在"+AREA_D->query_area_idname("lostland", 1)+"北方的島嶼上...");
			ob->move( ({ 37, 26, 0, "lostland", 1, 0, "37/26/0/lostland/1/0" }) );
			
			last_boss_pop_time = time();
		}
	}
}
