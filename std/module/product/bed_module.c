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
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

#define REGEN	30

mapping inlay_effect =
([
	"/obj/pasture/product/wool" : (: addn("effect", ($1 > 100 ? 100 : $1) / 5, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1, $2) :),
});

// 睡覺
void do_sleep(object me, string arg, object ob)
{

}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "睡覺。",
	"action":
		([
			//"sleep"	: (: do_sleep :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"床",
	
	//英文名稱
	"id":		"bed",

	//材料需求
	"material":	([ "high_timber":30, "/obj/farming/product/cotton":10, "/obj/pasture/product/wool":5 ]),

	//技能需求
	"skill":	([ "furniture-fac":50, "technology":50 ]),

	//生產工廠
	"factory":	"furniture",
]);
