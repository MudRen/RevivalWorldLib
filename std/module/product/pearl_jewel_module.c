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
	"/obj/gem/gem1" : (: addn(ATTACK, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem4" : (: addn(SPEED, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem5" : (: addn(DEFEND, standard_calculate($1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn(ATTACK, $1, $2) :),
	(: addn(SPEED, $1, $2) :),
	(: addn(DEFEND, $1, $2) :),
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
	"name":		"珍珠飾品",
	
	//英文名稱
	"id":		"pearl jewel",

	//材料需求
	"material":	([ "precious_metal":50, "/obj/fishfarm/product/pearl":2 ]),

	//技能需求
	"skill":	([ "clothing-fac":90, "technology":90 ]),

	//生產工廠
	"factory":	"clothing",
]);
