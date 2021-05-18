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
#include <inherit.h>
#include <feature.h>
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;
inherit BULLETIN_BOARD;

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "無",
	"action":
		([
		    "post"		:(: do_post :),
		    "read"		:(: do_read :),
		    "discard"		:(: do_discard :),
		    "followup"		:(: do_followup :),
		    "mail"		:(: do_mail :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"留言板",
	
	//英文名稱
	"id":		"board",

	//材料需求
	"material":	([ "normal_timber":30 ]),

	//技能需求
	"skill":	([ "furniture-fac":30, "technology":30 ]),

	//生產工廠
	"factory":	"furniture",
]);
