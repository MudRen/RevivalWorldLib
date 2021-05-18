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

mapping inlay_effect =
([
	"/obj/gem/gem0" : (: addn(BUFF_STAMINA_REGEN, standard_calculate($1), $2) :),
	"/obj/gem/gem3" : (: addn(BUFF_ENERGY_REGEN, standard_calculate($1), $2) :),
	"/obj/gem/gem6" : (: addn(BUFF_HEALTH_REGEN, standard_calculate($1), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_STAMINA_REGEN, $1, $2) :),
	(: addn(BUFF_ENERGY_REGEN, $1, $2) :),
	(: addn(BUFF_HEALTH_REGEN, $1, $2) :),
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
	"name":		"寶石飾品",
	
	//英文名稱
	"id":		"gem jewel",

	//材料需求
	"material":	([ "precious_metal":50, "gem":50 ]),

	//技能需求
	"skill":	([ "clothing-fac":70, "technology":70 ]),

	//生產工廠
	"factory":	"clothing",
]);
