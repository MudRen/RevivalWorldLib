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
	"name":		"人造纖維",
	
	//英文名稱
	"id":		"man made fibers",

	//材料需求
	"material":	([ "petroleum":1, "natural_gas":2 ]),

	//技能需求
	"skill":	([ "chemical-fac":10, "technology":10 ]),

	//生產工廠
	"factory":	"chemical",
]);
