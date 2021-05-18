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
	"/obj/gem/gem1" : (: addn(BUFF_INT, standard_calculate($1) * 2, $2) :),
	"/obj/gem/gem5" : (: addn(SPEED, standard_calculate($1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_INT, $1, $2) :),
	(: addn(SPEED, $1, $2) :),
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
	"name":		"笛",
	
	//英文名稱
	"id":		"flute",

	//材料需求
	"material":	([ "mold":3, "refined_timber":10, "precious_metal":2 ]),

	//技能需求
	"skill":	([ "instrument-fac":30, "technology":30 ]),

	//生產工廠
	"factory":	"instrument",
]);
