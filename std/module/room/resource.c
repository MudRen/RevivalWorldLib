/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : resource.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 資源採集場
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <buff.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";

mapping type_name = ([
"metal"	:	"/obj/materials/metal"->query_idname(),
"stone" :	"/obj/materials/stone"->query_idname(),
"water"	:	"/obj/materials/water"->query_idname(),
"wood"	:	"/obj/materials/wood"->query_idname(),
"fuel"	:	"/obj/materials/fuel"->query_idname(),
]);

array occupied_areas(string city)
{
	int i;
	string area, *sort_city;
	mapping area_section_influence;
	array occupied_areas = allocate(0);

	foreach(area in AREA_D->query_areas())
	{	
		for(i = 0;i<AREA_D->query_area_nums(area);++i)
		{
			area_section_influence = AREA_D->query_section_info(area, i, "area_section_influence");
			
			if( !sizeof(area_section_influence) )
				continue;	

			sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));

			if( !sizeof(sort_city) || sort_city[0] != city || area_section_influence[city]< 50000)
				continue;

			occupied_areas += ({ ({ area, i}) });
		}
	}	
	
	return occupied_areas;
}

void heart_beat(object room)
{
	string type;
	string file;
	object warehouse;
	object boss;
	object *labors = allocate(0);
	int amount;
	int total_amount;
	int users_inside;
	string owner = query("owner", room);
	array occupied_areas;
	array selected_area;
	int stamina_cur;
	int stamina_cost;
	int collect_buff;
	
	if( !objectp(boss = find_player(owner)) )
		return;

	foreach(object ob in all_inventory(room))
	{
		if( ob->is_module_npc() && query("job/type", ob) == WORKER && query("boss", ob) == owner )
			labors += ({ ob });
		else if( userp(ob) )
			users_inside = 1;
	}

	if( !sizeof(labors) ) return;

	type = query("setup/type", room);
	file = query("setup/warehouse", room);
	warehouse = load_module(file);
	
	if( !type )
	{
		if( users_inside )
			broadcast(room, "尚未設定採集種類，員工無法進行採集作業。\n");
	
		return;
	}

	if( !objectp(warehouse) )
	{
		if( users_inside )
			return broadcast(room, "尚未設定倉儲中心，員工無法進行採集作業。\n");
		
		return;
	}

	occupied_areas = occupied_areas(room->query_city());

	if( !sizeof(occupied_areas) )
	{
		if( users_inside )
			return broadcast(room, CITY_D->query_city_idname(room->query_city())+"尚未佔領任何郊區，無法進行採集工作。\n");
			
		return;
	}

	collect_buff = boss->query_all_buff(BUFF_COLLECT_ABILITY);
	
	file = "/obj/materials/"+type;
	
	foreach(object ob in labors)
	{
		stamina_cur = ob->query_stamina_cur();
		stamina_cost = (stamina_cur/75)*75;

		amount = stamina_cur / 75 * ob->query_skill_level(type) / 100;
		amount += amount * collect_buff / 100;

		if( amount > 0 )
			ob->cost_stamina(stamina_cost);
		
		if( users_inside )
		{
			if( amount > 0 )
			{
				selected_area = occupied_areas[random(sizeof(occupied_areas))];
				broadcast(room, ob->query_idname()+"自"+loc_short(selected_area[0],selected_area[1], random(100), random(100))+"採集回來 "+amount+" 個"+type_name[type]+"。\n");
			}
			else
			{
				broadcast(room, ob->query_idname()+"因為體力或採集技能的影響無法再採集資源回來。\n");
			}
		}	
		total_amount += amount;
	}

	if( total_amount <= 0 )
		return;

	input_object(warehouse->query_master(), "products", file, total_amount);
	warehouse->delay_save(1200);
}

string look_room(object room)
{
	string type = query("setup/type", room);
	string file = query("setup/warehouse", room);
	object warehouse = load_module(file);

	if( query("function", room) == "line" )
		return "倉儲中心："+(objectp(warehouse) ? warehouse->query_room_name() : "未設定")+"、採集種類："+ (type?type_name[type]:"未設定")+"\n";

	return "";
}

void do_setup(object me, string arg)
{
	int x, y;
	object env = environment(me);
	string city = env->query_city();
	int num = env->query_city_num();
	string type = query("setup/type", env);
	string file = query("setup/warehouse", env);
	object warehouse = load_module(file);

	if( query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這個資源採集場的擁有者。\n");

	if( !arg )
		return tell(me, "倉儲中心："+(objectp(warehouse) ? warehouse->query_room_name() : "未設定")+"\n採集種類："+ (type?type_name[type]:"未設定")+"\n");
	else if( arg == "warehouse -d" )
	{
		if( !objectp(warehouse) )
			return tell(me, "這裡原本便沒有設定倉儲中心。\n");
			
		delete("setup/warehouse", env);
		return tell(me, "取消倉儲中心設定。\n");
		
	}
	else if( sscanf(arg, "warehouse %d,%d", x, y) == 2 )
	{
		file = CITY_ROOM_MODULE(city, num, (x-1), (y-1), "warehouse");

		if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
			return tell(me, "座標"+loc_short(city, num, x-1, y-1)+"並沒有倉庫。\n");

		if( query("owner", warehouse) != me->query_id(1) )
			return tell(me, warehouse->query_room_name()+"並不屬於"+pnoun(2, me)+"。\n");
			
		set("setup/warehouse", file, env);
			
		env->save();
			
		return msg("$ME設定"+warehouse->query_room_name()+"為此資源收集區的倉儲中心。\n", me, 0, 1);
	}
	else if( arg == "type -d" )
	{
		if( !type )
			return tell(me, "這裡原本便沒有設定資源採集種類。\n");
			
		delete("setup/type", env);
		return tell(me, "取消資源採集種類設定。\n");
	}
	else if( sscanf(arg, "type %s", type) == 1 )
	{
		if( undefinedp(type_name[type]) )
			return tell(me, "沒有 "+type+" 這種資源種類。\n");
			
		set("setup/type", type, env);
		
		env->save();
	
		return msg("$ME設定此資源收集區的資源採集種類設定為「"+type_name[type]+"」。\n", me, 0, 1);
	}
	else
		return tell(me, "請輸入正確的指令格式。\n");
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"line"	:
	([
		"short"	: NOR WHT"作業區"NOR,
		"help"	:
			([
"topics":
@HELP
    資源採集場提供員工進行五大基本資源採集作業。
HELP,

"setup":
@HELP
設定倉庫資料的指令，用法如下：[管理指令]
  setup warehouse 21,33		- 設定此資源回收場對應的資源倉庫座標在 21,33
  setup warehouse -d		- 取消倉庫設定
  setup type '資源種類'		- 設定採集資源種類(metal、stone、water、wood、fuel)
  setup type -d			- 取消採集資源種類設定
HELP,
			]),
		"heartbeat":30,	// 實際時間 1 秒為單位
		"job": WORKER,
		"action":
			([
				"setup"		: (: do_setup :),
			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIY"資源"NOR YEL"採集場"NOR

	// 開張此建築物之最少房間限制
	,5

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,INDIVIDUAL

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,INDUSTRY_REGION

	// 開張儀式費用
	,5000000

	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,2
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,2
});
