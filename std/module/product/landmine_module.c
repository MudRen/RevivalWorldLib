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
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

// 啟動地雷
void do_activate(object me, string arg, object ob)
{
	if( !BATTLEFIELD_D->in_battle(me) && !wizardp(me) )
		return tell(me, me->query_idname()+"不在戰場中，無法放置地雷攻擊。\n");
		
	if( environment(ob) == me )
	{
		msg("$ME小心翼翼地啟動$YOU並放置在地面。\n", me, ob, 1);
		new("/obj/weapon/landmine", me, ob)->move_to_environment(me);
	}
	else if( environment(ob)->is_maproom() )
	{
		msg("$ME小心翼翼地啟動地面上的$YOU。\n", me, ob, 1);
		new("/obj/weapon/landmine", me, ob)->move_to_environment(ob);
	}
	else
	{
		msg("$ME無法在這裡啟動$YOU。\n", me, ob, 1);
		return;	
	}
	
	destruct(ob, 1);
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"activate"	: (: do_activate :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"地雷",
	
	//英文名稱
	"id":		"landmine",

	//材料需求
	"material":	([ "sulfur":3, "mold":1, "wire":1 ]),

	//技能需求
	"skill":	([ "longrange-fac":30, "technology":30 ]),

	//生產工廠
	"factory":	"longrange",
]);
