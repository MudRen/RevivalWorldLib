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

#include <ansi.h>
#include <feature.h>
#include <condition.h>
#include <location.h>
#include <daemon.h>
#include <delay.h>

inherit PRODUCT_ACTION_MOD;

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			//"launch"	: (: do_launch :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"導彈防禦機",
	
	//英文名稱
	"id":		"antimissile",

	//材料需求
	"material":	([ "mechanical_structure":2, "integrated_circuit":2, "petroleum":5 ]),

	//技能需求
	"skill":	([ "longrange-fac":90, "technology":90 ]),

	//生產工廠
	"factory":	"longrange",
]);
