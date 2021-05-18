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

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/materials/fuel" : (: addn("mass", -($1 > 10 ? 10 : $1 ) * 10, $2) :),
	"/obj/farming/product/wheat" : (: addn("effect", ($1 > 30 ? 30 : $1) * 5, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 5, $2) :),
});

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
	"name":		"饅頭",
	
	//英文名稱
	"id":		"steamed roll",

	//材料需求
	"material":	([ "/obj/farming/product/wheat":5, "tap_water":5, "natural_gas":1 ]),

	//技能需求
	"skill":	([ "food-fac":30, "technology":30 ]),

	//生產工廠
	"factory":	"food",
]);
