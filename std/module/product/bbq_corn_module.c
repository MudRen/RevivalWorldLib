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
	"/obj/materials/fuel" : (: addn("mass", -($1 > 10 ? 10 : $1) * 10, $2) :),
	"/obj/farming/product/corn" : (: addn("effect", ($1 > 30 ? 30 : $1) * 8, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 8, $2) :),
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
	"name":		"炭烤玉米",
	
	//英文名稱
	"id":		"bbq corn",

	//材料需求
	"material":	([ "/obj/farming/product/corn":10, "normal_timber":5 ]),

	//技能需求
	"skill":	([ "food-fac":10, "technology":10 ]),

	//生產工廠
	"factory":	"food",
]);
