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

int no_available(object me)
{
	return query("battlereward/halberdbook", me) ? 0 : 1;
}

mapping inlay_effect =
([
	"/obj/gem/gem1" : (: addn(ATTACK, standard_calculate($1), $2) :),
	"/obj/gem/gem2" : (: addn(BUFF_PHY, standard_calculate($1), $2) :),
	"/obj/gem/gem3" : (: addn(BUFF_AGI, standard_calculate($1), $2) :),
	"/quest/monster_fossil/lava_meteor" : (: set(BUFF_FATAL_POWER, calculate_chance($1*5) > 12 ? 60 : calculate_chance($1*5)*5, $2) :),
	"/quest/monster_fossil/monster_gall" : (: set(BUFF_FATAL_CHANCE, calculate_chance($1) > 6 ? 6 : calculate_chance($1), $2) :),
	"/obj/inlay/hydra_stick_secret_art_1st" : (: set("hydra_stick_secret_art_1st", $1, $2) :),
	"/obj/inlay/hydra_stick_secret_art_2nd" : (: set("hydra_stick_secret_art_2nd", $1, $2) :),
	"/obj/inlay/faith_halberd_secret_art_1st" : (: set("faith_halberd_secret_art_1st", $1, $2) :),
	"/obj/inlay/faith_halberd_secret_art_2nd" : (: set("faith_halberd_secret_art_2nd", $1, $2) :),
	"/obj/inlay/sky_2nd_secret_art" : (: set("sky_2nd_secret_art", $1, $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_PHY, $1, $2) :),
	(: addn(ATTACK, $1, $2) :),	
	(: addn(BUFF_AGI, $1, $2) :),	
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
	"name":		"戟",
	
	//英文名稱
	"id":		"halberd",

	//材料需求
	"material":	([ "man_made_fibers":20, "steel":150, "mechanical_structure":15, "rubber":15 ]),

	//技能需求
	"skill":	([ "shortrange-fac":100, "technology":100 ]),

	//生產工廠
	"factory":	"shortrange",
]);
