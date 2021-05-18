/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mapnpc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : �a�Ϩt�ΤW�� NPC �w��P����
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

	// �@�����ˬd�@���O�_ pop BOSS
	if( !(timetick % 60) )
	{
		object ob;
		mapping data;

		foreach(data in npc)
			foreach(ob in data["objects"])
				if( objectp(ob) )
					return;
	
		// �b�p�� pop �@��
		if( last_boss_pop_time < time() - 900 && sizeof(children("/obj/npc/lostland/forgotten_ancient_knight")) <= 1)
		{
			ob = new("/obj/npc/lostland/forgotten_ancient_knight");
			
			CHANNEL_D->channel_broadcast("combat", ob->short()+"����"HIR"��{��"NOR RED"�l��"NOR"�A�X�{�b"+AREA_D->query_area_idname("lostland", 0)+"������...");
			ob->move( ({ 49, 49, 0, "lostland", 0, 0, "49/49/0/lostland/0/0" }) );
			
			last_boss_pop_time = time();
		}
	}
}
