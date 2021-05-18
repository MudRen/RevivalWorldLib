/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _mapnpc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-13
 * Note   : 地圖系統上的 NPC 定位與移動
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <location.h>
#include <daemon.h>
#include <nature.h>
#include <nature_data.h>
#include <map.h>

mapping corresponding =
([
	"north"		:	"south",
	"south"		:	"north",
	"east"		:	"west",
	"west"		:	"east",
	"northwest"	:	"southeast",
	"northeast"	:	"southwest",
	"southwest"	:	"northeast",
	"southeast"	:	"northwest",
	"up"		:	"down",
	"down"		:	"up",
]);

mapping weather_go =
([
    SUNNY		:	"滿頭大汗地",
    CLOUDY		:	"伴隨著和風",
    RAINY		:	"腳底拖著雨水",
    WINDY		:	"風塵僕僕",
    SNOWY		:	"帶著斑斑白雪",
    SHOWER		:	"從頭到腳濕淋淋地",
    TYPHOON		:	"全身一塌糊塗地",
    FOEHN		:	"撲著熱風"
]);

void startup_heart_beat()
{
	set_heart_beat(1);
}

varargs void load_mapnpc(array ref npc, string map_id, int map_num, int no_default_npc)
{
	int i, amount;
	int loc_check;
	string filename;
	float bonus;
	array loc;
	object ob;

	if( undefinedp(no_default_npc) )
		npc += load_object(__DIR__"all_area.c")->query_all_area_npc();

	for(i=0;i<sizeof(npc);i++)
	{
		npc[i]["objects"] = allocate(0);
		
		if( npc[i]["move_speed"] == -1 )
			npc[i]["move_speed_count"] = -1;
		else
			npc[i]["move_speed_count"] = random(npc[i]["move_speed"]);

		loc_check = 0;
		do
		{
			loc = arrange_location(
				range_random(npc[i]["x"][0], npc[i]["x"][1]), 
				range_random(npc[i]["y"][0], npc[i]["y"][1]), 
				0, 
				map_id, 
				map_num, 
				0
			);
			
			if( ++loc_check > 100 ) break;

		} while ( AREA_D->query_coor_data(loc, FLAGS) & NO_MOVE );
		
		foreach(filename, amount in npc[i]["group"])
		{
			bonus = 1. + (pow(amount, 3.5)/100.);

			while(amount--)
			{
				ob = new(filename);
				ob->move(loc);
				broadcast(loc, HIY+ob->query_idname()+HIY"突然從角落竄了出來！\n"NOR);
				set_temp("mapnpc", 1, ob);
				npc[i]["objects"] += ({ ob });
			}
		}
	}
	
	call_out( (: startup_heart_beat :), random(10) );
}

int unload_mapnpc(array ref npc)
{
	int i;
	object ob;

	for(i=0;i<sizeof(npc);i++)
	{
		if( arrayp(npc[i]["objects"]) )
		foreach(ob in npc[i]["objects"])
		{
			if( objectp(ob) )
			{
				ob->do_command("die 系統更新");
				destruct(ob);
			}
		}
				
		npc[i]["objects"] = allocate(0);
	}
}

void mapnpc_heart_beat(array ref npc, string map_id, int map_num)
{
	int i, j, size;
	object ob;
	array loc;
	array firstloc;
	string *valid_direction;
	string direction;
	array nextloc;
	string filename;
	mapping current_objects_count;
	int amount;
	int loc_check;

	size = sizeof(npc);

	for(i=0;i<size;i++)
	{
		// 清除不存在的物件
		npc[i]["objects"] -= ({ 0 });

		// 找第一個物件來判斷目前座標	
		if( sizeof(npc[i]["objects"]) )
			loc = query_temp("location", npc[i]["objects"][0]);
		else
			loc = 0;

		current_objects_count = allocate_mapping(0);
		// 計算現有數量, 找出目前座標
		foreach(ob in npc[i]["objects"])
			current_objects_count[base_name(ob)]++;

		
		// 檢查整個團隊的物件是否還存在
		foreach(filename, amount in npc[i]["group"])
		{
			// 實際數量少於設定數量(必須進行重生)
			if( current_objects_count[filename] < amount )
			{
				if( undefinedp(npc[i]["reborn_filename"]) )
					npc[i]["reborn_filename"] = allocate_mapping(0);
					
				if( undefinedp(npc[i]["reborn_filename"][filename]) )
					npc[i]["reborn_filename"][filename] = allocate(0);
			
				// 若目前排序中的重生資料不足缺少的物件數 -> 新增重生資料
				if( sizeof(npc[i]["reborn_filename"][filename]) < amount - current_objects_count[filename] )
					npc[i]["reborn_filename"][filename] += allocate(amount - current_objects_count[filename], npc[i]["reborn_time"]);

				// 掃瞄目前重生資料 -> 倒數完畢進行重生
				for(j=0;j<sizeof(npc[i]["reborn_filename"][filename]);j++)
				{
					// 倒數完畢, 進行重生
					if( !--npc[i]["reborn_filename"][filename][j] )
					{
						loc_check = 0;
						// 沒有座標, 建立新座標
						if( !arrayp(loc) )
						{
							do
							{
								loc = arrange_location(
									range_random(npc[i]["x"][0], npc[i]["x"][1]), 
									range_random(npc[i]["y"][0], npc[i]["y"][1]), 
									0, 
									map_id, 
									map_num, 
									0
								);
								
								if( ++loc_check > 100 ) break;

							} while ( AREA_D->query_coor_data(loc, FLAGS) & NO_MOVE );
						}
						
						ob = new(filename);
						ob->move(loc);
						broadcast(loc, HIY+ob->query_idname()+HIY"突然從角落竄了出來！\n"NOR);
						set_temp("mapnpc", 1, ob);
						npc[i]["objects"] += ({ ob });
					}
				}
				npc[i]["reborn_filename"][filename] -= ({ 0 });
				
				if( !sizeof(npc[i]["reborn_filename"][filename]) )
					map_delete(npc[i]["reborn_filename"], filename);
			}
		}

		// 若通通都還沒重生, 就略過		
		if( !sizeof(npc[i]["objects"]) ) continue;

		// 進行移動處理
		if( npc[i]["move_speed_count"] != -1 && --npc[i]["move_speed_count"] <= 0 )
		{
			npc[i]["move_speed_count"] = npc[i]["move_speed"];
			
			// 有移動範圍的話就進行移動
			if( npc[i]["x"][0] != npc[i]["x"][1] || npc[i]["y"][0] != npc[i]["y"][1] )
			{
				firstloc = copy(loc);

				foreach(ob in npc[i]["objects"])
				{
					loc = query_temp("location", ob);
			
					// 若是第一隻或是後續幾隻的座標與第一隻不同時
					if( ob == npc[i]["objects"][0] || firstloc[X] != loc[X] || firstloc[Y] != loc[Y] )
					{
						valid_direction = allocate(0);
						
						if( loc[X] > npc[i]["x"][0] ) valid_direction += ({ "west" });
						if( loc[X] < npc[i]["x"][1] ) valid_direction += ({ "east" });
						if( loc[Y] > npc[i]["y"][0] ) valid_direction += ({ "north" });
						if( loc[Y] < npc[i]["y"][1] ) valid_direction += ({ "south" });
		
						direction = valid_direction[random(sizeof(valid_direction))];
						
						switch(direction)
						{
							case "west":
								nextloc = arrange_location(loc[X]-1, loc[Y], 0, map_id, map_num, 0);
								break;
							case "east":
								nextloc = arrange_location(loc[X]+1, loc[Y], 0, map_id, map_num, 0);
								break;
							case "north":
								nextloc = arrange_location(loc[X], loc[Y]-1, 0, map_id, map_num, 0);
								break;
							case "south":
								nextloc = arrange_location(loc[X], loc[Y]+1, 0, map_id, map_num, 0);
								break;
						}
					}

					if( COMBAT_D->in_fight(ob) || ob->is_faint() || ob->is_dead() || AREA_D->query_coor_data(nextloc, FLAGS) & NO_MOVE )
						continue;

					broadcast(loc, ob->query_idname()+"朝"+CHINESE_D->to_chinese(direction)+"離開。\n", 0, ob);
					ob->move(nextloc);
					broadcast(nextloc, ob->query_idname()+weather_go[NATURE_D->query_nature(AREA_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，自"+CHINESE_D->to_chinese(corresponding[direction])+"走了過來。\n", 0, ob);
					
					ob->follower_move(loc, nextloc);	
				}
			}
		}
	}	
}