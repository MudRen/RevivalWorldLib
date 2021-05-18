/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <citydata.h>
#include <fly.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

#define X	0
#define Y	1

// 開飛機
void do_fly(object me, string arg, object ob)
{
	int x, y, total_fuel, need_fuel;
	object fuel, *fuels, env = environment(me);
	string to_area, from_area;
	int to_num, from_num, *to_coor, *from_coor;
	int loctype;
	array loc, loc_temp;
	object target;
	array path;
	array map_data;
	array process = allocate(0);
			
	if( !me->valid_move() ) 
		return;

	if( !me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"必須先裝備"+ob->query_idname()+"。\n");

	if( !arg || sscanf(arg, "%s %d %d,%d", to_area, to_num, x, y) != 4 )
		return tell(me, pnoun(2, me)+"想要飛去哪裡？(例：lightforest 1 50,50)\n");

	to_area = lower_case(to_area);
	to_num--;
	x--;
	y--;

	if( env->is_maproom() )
	{
		if( !stringp(from_area = env->query_area()) )
			return tell(me, pnoun(2, me)+"必須在機場或是郊區才能夠起飛與降落。\n");
		else if( AREA_D->query_area_info(from_area, "need_flag") )
			return tell(me, pnoun(2, me)+"無法從這個區域飛往任何地方。\n");

		from_num = env->query_num();
	}
	else if( env->query_building_type() == "airport" && query("function", env) == "airstrip" )
	{
		from_area = env->query_city();
		from_num = env->query_city_num();
	}
	else
		return tell(me, pnoun(2, me)+"必須在機場或是郊區才能夠起飛與降落。\n");

	if( env->is_maproom() )
	{
		loc = query_temp("location", me);
		
		loctype = MAP_D->query_coor_data(loc, TYPE);
	
		if( loctype == SEA || loctype == RIVER )
			return tell(me, pnoun(2, me)+"目前所在位置是海洋或河流，無法駕駛飛機。\n");
	}

	if( CITY_D->city_exist(to_area) )
	{
		string file = CITY_ROOM_MODULE(to_area, to_num, x, y, "airport");

		if( !objectp(target = load_module(file)) || query("function", target) != "airstrip" )
			return tell(me, "座標"+loc_short(to_area, to_num, x, y)+"並沒有飛機場跑道。\n");
			
		loc = arrange_area_location(x, y, to_area, to_num);
	}
	else if( AREA_D->area_exist(to_area))
	{
		if( AREA_D->query_area_info(to_area, "need_flag") )
			return tell(me, pnoun(2, me)+"所設定的目的地是特殊區域，無法飛行至該處。\n");

		if( !AREA_D->valid_coordinate(x, y, to_area, to_num) )
			return tell(me, pnoun(2, me)+"必須輸入正確的座標位置(例：lightforest 1 50,50)。\n");
		
		loc = arrange_area_location(x, y, to_area, to_num);
		
		loctype = AREA_D->query_coor_data(loc, TYPE);

		if( loctype == SEA || loctype == RIVER )
			return tell(me, "座標"+loc_short(loc)+"是海洋或河流，"+ob->query_idname()+"無法在該處降落。\n");
	}
	else
		return tell(me, pnoun(2, me)+"必須輸入正確的座標位置(例：lightforest 1 50,50)。\n");

 	fuels = filter_array(all_inventory(me), (: $1->query_module() == "liquid_fuel" :));
 	
 	total_fuel = implode(map(fuels, (: $1->query_amount() :)), (: $1 + $2 :));
 
 	from_coor = MAP_D->query_world_map_location(from_area, from_num);
 	to_coor = MAP_D->query_world_map_location(to_area, to_num);
 	
 	need_fuel = 1 + (to_int(pythagorean_thm(from_coor[X] - to_coor[X], from_coor[Y] - to_coor[Y]))/2);
 	
 	if( total_fuel < need_fuel )
 		return tell(me, pnoun(2, me)+"至少需要 "+need_fuel+" 個液態燃料才能夠起飛前往"+loc_short(loc)+"。\n");
 		
 	total_fuel = need_fuel;
 
 	foreach(fuel in fuels)
 	{
 		if( fuel->query_amount() >= total_fuel )
 		{
 			destruct(fuel, total_fuel);
 			break;
 		}
 		else
 		{
 			total_fuel -= fuel->query_amount();
 			destruct(fuel);
 		}
 	}
  
  path = ALGORITHM_PATH_D->search_map_path(from_coor, to_coor);
	
	me->start_fly(LIMITED_FLY_TYPE);
	
	foreach(array coor in path[1..<2])
	{
		map_data = MAP_D->query_world_map(coor[X], coor[Y]);

		loc_temp = arrange_area_location(range_random(0,99), range_random(0,99), map_data[0], map_data[1]);
		
		process += ({ 
			(: objectp($(me)) ? $(me)->move($(loc_temp)) : 0 :), 
			(: objectp($(me)) ? msg("$ME駕駛著$YOU飛越"+loc_short($(loc_temp))+"的上空...。\n", $(me), $(ob), 1) : 0 :),
			1 
		});
	}
	
 	if( arrayp(loc) )
  	{
  		process += ({ 
  			(: objectp($(me)) ? $(me)->move($(loc)) : 0 :),
  			(: objectp($(me)) ? msg("$ME駕駛著$YOU平穩地降落在"+loc_short($(loc))+"。\n", $(me), $(ob), 1) : 0 :),
  			(: objectp($(me)) ? $(me)->stop_fly() : 0 :),
  		});
		msg("$ME駕駛"+ob->query_idname()+"飛上天際，花費 "+need_fuel+" 個液態燃料前往"+loc_short(loc)+"。\n", me, 0, 1);
		process_function(process);
	}
	else if( objectp(target) )
	{
  		process += ({ 
  			(: objectp($(me)) ? $(me)->move($(target)) : 0 :),
  			(: objectp($(me)) ? msg("$ME駕駛著$YOU平穩地降落在"+loc_short($(loc))+"。\n", $(me), $(ob), 1) : 0 :),
  			(: objectp($(me)) ? $(me)->stop_fly() : 0 :),
  		});
		msg("$ME駕駛"+ob->query_idname()+"飛上天際，花費 "+need_fuel+" 個液態燃料前往"+target->query_room_name()+"。\n", me, 0, 1);
		process_function(process);
	}
	else
		error("發生錯誤。\n");
		
	
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"fly"		: (: do_fly :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"滑翔機",
	
	//英文名稱
	"id":		"glider",

	//材料需求
	"material":	([ "tire":5, "plane_body":1, "engine":2, "steel":100, "integrated_circuit":5, "mechanical_structure":5, "man_made_fibers":30, "wire":10, "petroleum":500 ]),

	//技能需求
	"skill":	([ "aircraft-fac":40, "technology":40 ]),

	//生產工廠
	"factory":	"aircraft",
]);
