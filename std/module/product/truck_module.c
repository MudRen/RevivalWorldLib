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
#include <daemon.h>
#include <map.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/gem/gem4" : (: addn(BUFF_LOADING_MAX, to_int(pow(to_float($1), 0.9))*100, $2) :),
	"/obj/gem/gem5" : (: addn(BUFF_SLOT_MAX, to_int(pow(to_float($1), 0.5)), $2) :),
]);

array enhancement = 
({ 
	(: addn(BUFF_LOADING_MAX, $1 * 10, $2) :),
	(: addn(BUFF_SLOT_MAX, $1, $2) :),
});

// 開車
void do_drive(object me, string arg, object ob)
{

}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			//"drive"		: (: do_drive :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"卡車",
	
	//英文名稱
	"id":		"truck",

	//材料需求
	"material":	([ "tire":14, "car_body":1, "engine":1, "steel":100, "integrated_circuit":1, "mechanical_structure":5,"petroleum":500 ]),

	//技能需求
	"skill":	([ "transportation-fac":80, "technology":80 ]),

	//生產工廠
	"factory":	"transportation",
]);
