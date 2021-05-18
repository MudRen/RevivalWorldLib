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

#define MAP_ID		"lostland"
#define MAP_NUM		0

inherit __DIR__"_mapnpc.c";

int timetick;
int last_boss_pop_time;

array npc =
({
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
	(["group":(["/obj/npc/lostland/forgotten_warrior" : 2,]), "x":({ 1, 98 }), "y":({ 1, 98 }), "move_speed" : 60, "reborn_time" : 600, ]),
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
	
		// 半小時 pop 一次
		if( last_boss_pop_time < time() - 900 && sizeof(children("/obj/npc/lostland/forgotten_ancient_knight")) <= 1)
		{
			ob = new("/obj/npc/lostland/forgotten_ancient_knight");
			
			CHANNEL_D->channel_broadcast("combat", ob->short()+"受到"HIR"血腥的"NOR RED"召喚"NOR"，出現在"+AREA_D->query_area_idname("lostland", 0)+"的中央...");
			ob->move( ({ 49, 49, 0, "lostland", 0, 0, "49/49/0/lostland/0/0" }) );
			
			last_boss_pop_time = time();
		}
	}
}
