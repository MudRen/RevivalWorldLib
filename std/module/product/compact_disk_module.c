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
#include <condition.h>

inherit PRODUCT_ACTION_MOD;

// 放置物品
void do_action(object me, string arg)
{


}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "聆聽音響。",
	"action":
		([
			//"play"	: (: do_play :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"音樂光碟",
	
	//英文名稱
	"id":		"compact disk",

	//材料需求
	"material":	([ "precious_metal":2, "plastic":2, "sulfur":1 ]),

	//技能需求
	"skill":	([ "instrument-fac":70, "technology":70 ]),

	//生產工廠
	"factory":	"instrument",
]);
