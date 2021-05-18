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
#include <condition.h>

#define MAP_ID		"lostland"
#define MAP_NUM		2

inherit __DIR__"_mapnpc.c";

int timetick;
int last_boss_pop_time;

array npc =
({
	(["group":(["/obj/npc/lostland/hydra" : 1,]), "x":({ 19, 79 }), "y":({ 19, 79 }), "move_speed" : 30, "reborn_time" : 600, ]),
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

void start_cooldown(object* attackers)
{
	"/obj/npc/lostland/hydra"->start_cooldown(attackers, LOSTSEAL);
}

int delay_start_cooldown(object* attackers)
{
	return call_out((: start_cooldown :), 600, attackers);
}

void heart_beat()
{
	timetick++;

	::mapnpc_heart_beat(ref npc, MAP_ID, MAP_NUM);

/*
	// 一分鐘檢查一次是否 pop BOSS
	if( !(timetick % 60) )
	{
		object ob;
		mapping data;

		foreach(data in npc)
			foreach(ob in data["objects"])
				if( objectp(ob) )
					return;
	
		// 一天只會 pop 一次
		if( last_boss_pop_time < time() - 60*60*20 )
		{
			ob = new("/obj/npc/lostland/nessiteras_rhombopteryx");
			
			CHANNEL_D->channel_broadcast("news", ob->query_idname()+"受到"HIB"深海的"NOR BLU"召喚"NOR"，出現在"+AREA_D->query_area_idname("lostland", 1)+"北方的島嶼上...");
			ob->move( ({ 37, 26, 0, "lostland", 1, 0, "37/26/0/lostland/1/0" }) );
			
			last_boss_pop_time = time();
		}
	}
	*/
}
