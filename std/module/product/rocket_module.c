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

#include <ansi.h>
#include <feature.h>
#include <condition.h>
#include <location.h>
#include <daemon.h>
#include <combat.h>

inherit PRODUCT_ACTION_MOD;

int calculate_distance(int num)
{
	switch(num)
	{
		case 0..100: 	return 0; break;
		case 101..200: 	return 1; break;
		case 201..300: 	return 2; break;
		case 301..400:	return 3; break;
		case 401..500:	return 4; break;
		case 501..600:	return 5; break;
		case 601..700:	return 6; break;
		case 701..800:	return 7; break;
		case 801..900:	return 8; break;
		case 901..1000:	return 9; break;
		case 1001..MAX_INT: return 10; break;
	}
}

int calculate_range(int num)
{
	switch(num)
	{
		case 0..100: 	return 0; break;
		case 101..200: 	return 1; break;
		case 201..300:  return 2; break;
		case 301..400:	return 3; break;
		case 401..500:	return 4; break;
		case 501..MAX_INT: return 5; break;
	}
}

mapping inlay_effect =
([
	"/obj/materials/fuel" : (: addn("distance", calculate_distance($1), $2) :),
	"/obj/materials/metal" : (: addn("range", calculate_range($1), $2) :),
]);

// 放置物品
void do_fire(object me, string arg, object ob)
{
	int distance = query("distance", ob);
	int range = query("range", ob);
	array my_loc;
	array target_loc;
	object *targets;
	int damage;
	string direction;
	int max_health;
	
	if( !BATTLEFIELD_D->in_battle(me) )
		return tell(me, ob->query_idname()+"只能在戰場裡發射。\n");

	if( time() - query_temp("rocket_fire_time", me) < 3 )
		return tell(me, "必須等待 3 秒鐘後才能再次發射"+ob->query_idname()+"。\n");

	my_loc = query_temp("location", me);
	target_loc = copy(my_loc);

	switch(arg)
	{
		case "n":
		case "north":
			target_loc[Y] -= distance;
			direction = "北方";		
			break;
		case "s":
		case "south":
			target_loc[Y] += distance;
			direction = "南方";
			break;
		case "w":
		case "west":
			target_loc[X] -= distance;
			direction = "西方";
			break;
		case "e":
		case "east":
			target_loc[X] += distance;
			direction = "東方";
			break;
		case "ne":
		case "northeast":
			target_loc[Y] -= distance;
			target_loc[X] += distance;
			direction = "東北方";
			break;
		case "se":
		case "southeast":
			target_loc[Y] += distance;
			target_loc[X] += distance;
			direction = "東南方";
			break;
		case "nw":
		case "northwest":
			target_loc[Y] -= distance;
			target_loc[X] -= distance;
			direction = "西北方";
			break;
		case "sw":
		case "southwest":
			target_loc[Y] += distance;
			target_loc[X] -= distance;
			direction = "西南方";
			break;
		default:
			return tell(me, pnoun(2, me)+"想往哪個方向發射"+ob->query_idname()+"(n,s,w,e,ne,se,nw,sw)？\n");
	}
	
	if( !MAP_D->valid_coordinate(target_loc) )
		return tell(me, "發射目標已經超出地圖範圍，請選擇其他方向發射。\n");

	targets = keys(MAP_D->query_range_inventory(target_loc, range));

	msg("$ME往"+direction+"發射"+ob->query_idname()+"。\n", me, 0, 1);

	foreach(object target in keys(MAP_D->query_range_inventory(target_loc, range)))
	{
		max_health = target->query_health_max();
		
		damage = range_random(max_health/2, to_int(max_health/1.5));

		if( damage <= 0 || !target->is_living() || target->is_dead() || target->is_boss() || target->is_faint() || target->in_condition(INVINCIBILITY) )
			continue;	
		
		msg("$ME"HIY"發射的"+ob->query_idname()+HIY"命中$YOU"HIY"，造成嚴重傷害。\n"NOR, me, target, 1);
		
		COMBAT_D->cause_damage(me, target, damage, NO_FATALBLOW | NO_ATTACKDEFEND);
	}

	set_temp("rocket_fire_time", time(), me);

	destruct(ob, 1);
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"fire"	: (: do_fire :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"火箭",
	
	//英文名稱
	"id":		"rocket",

	//材料需求
	"material":	([ "sulfur":10, "mechanical_structure":1, "integrated_circuit":1, "plastic":1, "liquid_fuel":3 ]),

	//技能需求
	"skill":	([ "longrange-fac":50, "technology":50 ]),

	//生產工廠
	"factory":	"longrange",
]);
