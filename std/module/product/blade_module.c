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
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

int calculate_chance(int value)
{
	value = to_int(pow(to_float(value), 0.5));
		
	return value;
}

mapping inlay_effect =
([
	"/obj/gem/gem0" : (: addn(BUFF_STR, standard_calculate($1), $2) :),
	"/obj/gem/gem1" : (: addn(ATTACK, standard_calculate($1), $2) :),
	"/quest/monster_fossil/lava_meteor" : (: set(BUFF_FATAL_POWER, calculate_chance($1*5) > 12 ? 60 : calculate_chance($1*5)*5, $2) :),
	"/quest/monster_fossil/monster_gall" : (: set(BUFF_FATAL_CHANCE, calculate_chance($1) > 6 ? 6 : calculate_chance($1), $2) :),
	"/obj/inlay/ancient_blade_secret_art_1st" : (: set("ancient_blade_secret_art_1st", $1, $2) :),
	"/obj/inlay/ancient_blade_secret_art_2nd" : (: set("ancient_blade_secret_art_2nd", $1, $2) :),
	"/obj/inlay/sky_3rd_secret_art" : (: set("sky_3rd_secret_art", $1, $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_STR, $1, $2) :),
	(: addn(ATTACK, $1, $2) :),
});

// 放置物品
void do_action(object me, string arg)
{


}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			//"action"	: (: do_action :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"刀",
	
	//英文名稱
	"id":		"blade",

	//材料需求
	"material":	([ "mold":5, "steel":60, "plastic":6 ]),

	//技能需求
	"skill":	([ "shortrange-fac":50, "technology":5 ]),

	//生產工廠
	"factory":	"shortrange",
]);
