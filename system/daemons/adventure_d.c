/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : adventure_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-22
 * Note   : 探險系統
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <message.h>

#define DATA		"/data/daemon/adventure.o"
#define NPC_TYPE	({ "leader", "prospector", "explorer", "guard", "scout" })
mapping adventure;


int save()
{
	save_object(DATA);
}

//
// 是否正在進行探險
//
int is_doing_adventure(mixed env)
{
	if( stringp(env) )
		return !undefinedp(adventure[env]);
	else if( objectp(env) )
		return !undefinedp(adventure[base_name(env)]);
	else
		return 0;
}

//
// 尚需要的時間
//
int time_still(object env)
{
	return adventure[base_name(env)]["time"];
}

//
// 開始探險活動
//
void start_adventure(object env, mapping data)
{
	adventure[base_name(env)] = data;
}

//
// 心跳函式
//
void heart_beat()
{
	object env;
	object ob;
	string npctype;
	string filename, filename2;
	mapping data, data2;
	
	// 取出所有 NPC 物件, 檢查資料
	foreach(filename, data in adventure)
	{
		foreach(npctype in NPC_TYPE)
		{
			if( data[npctype] && file_exists(data[npctype]) )
				adventure[filename][npctype+"_ob"] = load_object(data[npctype]);
			else
				map_delete(adventure[filename], npctype+"_ob");
		}
		
		// 探險基地不見了 -> 取消探險任務
		if( !file_exist(filename) )
		{
			foreach(npctype in NPC_TYPE)
			{
				if( !objectp(ob = adventure[filename][npctype+"_ob"]) ) continue;
				delete("adventure", ob);
				ob->save();
			}

			map_delete(adventure, filename);
		}
	}
	
	// 進行探險活動
	foreach(filename, data in adventure)
	{
		adventure[filename]["time"]--;

		// 探險結束
		if( adventure[filename]["time"] <= 0 )
		{
			env = load_object(filename);
			
			foreach(npctype in NPC_TYPE)
			{
				if( !objectp(ob = adventure[filename][npctype+"_ob"]) ) continue;

				delete("adventure", ob);
				ob->move(env);
				ob->save();
				msg("$ME結束了在「"+AREA_D->query_area_idname(data["area"], data["num"])+"」的探險活動，疲憊地回到此地。\n", ob, 0, 1);
			}
			
			map_delete(adventure, filename);
			continue;
		}

		foreach(filename2, data2 in adventure)
		{
			// 不同區域不進行戰鬥
			if( data["area"] != data2["area"] || data["num"] != data2["num"] || filename == filename2 ) continue;

			// 偵查兵能力愈高愈能避免戰鬥
			if( data["scout_ob"] && random(data["scout_ob"]->query_agi()) > 5 ) continue;
			
			// 兩雙方皆有護衛, 進行戰鬥
			if( data["guard_ob"] && data2["guard_ob"] )
			{
				if( random(data["guard_ob"]->query_phy()) > random(data2["guard_ob"]->query_phy()) )
					CHANNEL_D->channel_broadcast("nch", "消息傳來在「"+AREA_D->query_area_idname(data["area"], data["num"])+"」中，"+capitalize(data["boss"])+" 與 "+capitalize(data2["boss"])+" 兩支探險隊伍發生嚴重衝突。\n");				
				
				map_delete(adventure, filename);
			}
			// 若只有我方有護衛
			else if( data["guard_ob"] )
			{
				if( random(data["guard_ob"]->query_phy()) > 5 )
					CHANNEL_D->channel_broadcast("nch", "消息傳來在「"+AREA_D->query_area_idname(data["area"], data["num"])+"」中，"+capitalize(data["boss"])+" 與 "+capitalize(data2["boss"])+" 兩支探險隊伍發生嚴重衝突。\n");	
			}
			// 若只有對方有護衛
			else if( data2["guard_ob"] )
			{
				if( random(data2["guard_ob"]->query_phy()) > 5 )
					CHANNEL_D->channel_broadcast("nch", "消息傳來在「"+AREA_D->query_area_idname(data["area"], data["num"])+"」中，"+capitalize(data["boss"])+" 與 "+capitalize(data2["boss"])+" 兩支探險隊伍發生嚴重衝突。\n");	
			}
			// 若雙方都沒有護衛
			else
			{
				if( !random(3) )
					CHANNEL_D->channel_broadcast("nch", "消息傳來在「"+AREA_D->query_area_idname(data["area"], data["num"])+"」中，"+capitalize(data["boss"])+" 與 "+capitalize(data2["boss"])+" 兩支探險隊伍發生嚴重衝突。\n");	
			}
		}
	}
	
	save();
}


int remove()
{
	return save();
}

void create()
{
	if( !restore_object(DATA) )
	{
		adventure = allocate_mapping(0);

		save();
	}
	
	set_heart_beat(60);
}

string query_name()
{
	return "探險系統(ADVENTURE_D)";
}
