/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mapnpc_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-13
 * Note   : 地圖系統上的 NPC 定位與移動
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define MAPNPC_DATA_FILE(X, Y)	"/system/daemons/mapnpc/area_"+X+"_"+Y+".c"

void create()
{
	string mapname;
	int i, num;
	string mapnpc_data_file;

	foreach(mapname in AREA_D->query_areas())
	{
		for(num=0;num<AREA_D->query_area_nums(mapname);++num)
		{
			mapnpc_data_file = MAPNPC_DATA_FILE(mapname, num);

			if( !file_exists(mapnpc_data_file) ) continue;

			i++;

			//tell(find_player("clode"), mapnpc_data_file+"\n");
			
			call_out( (: load_object($(mapnpc_data_file)) :) , i*2);
		}
	}
}

void become_named_npc(object npc)
{
	int min = 15;
	int max = 30;

	addn("attr/str", to_int(query("attr/str", npc)*10./range_random(min, max)), npc);
	addn("attr/phy", to_int(query("attr/phy", npc)*10./range_random(min, max)), npc);
	addn("attr/int", to_int(query("attr/int", npc)*10./range_random(min, max)), npc);
	addn("attr/agi", to_int(query("attr/agi", npc)*10./range_random(min, max)), npc);
	addn("attr/cha", to_int(query("attr/cha", npc)*10./range_random(min, max)), npc);

	addn("abi/stamina/max", to_int(query("abi/stamina/max", npc)*10./range_random(min, max)), npc);
	addn("abi/stamina/cur", to_int(query("abi/stamina/cur", npc)*10./range_random(min, max)), npc);
	addn("abi/health/max", to_int(query("abi/health/max", npc)*10./range_random(min, max)), npc);
	addn("abi/health/cur", to_int(query("abi/health/cur", npc)*10./range_random(min, max)), npc);
	addn("abi/energy/max", to_int(query("abi/energy/max", npc)*10./range_random(min, max)), npc);
	addn("abi/energy/cur", to_int(query("abi/energy/cur", npc)*10./range_random(min, max)), npc);

	addn("skills/unarmed/level", to_int(query("skills/unarmed/level", npc)*10./range_random(min, max)), npc);
	addn("skills/dodge/level", to_int(query("skills/dodge/level", npc)*10./range_random(min, max)), npc);
	
	addn("skills/fatalblow/level", 5, npc);
	
	set_temp("status/"HIM"突"NOR MAG"變"NOR, 999999, npc);
	
	set("named", 1, npc);
}

int remove() { }

string query_name()
{
	return "地圖 NPC 管理系統(MAPNPC_D)";
}
