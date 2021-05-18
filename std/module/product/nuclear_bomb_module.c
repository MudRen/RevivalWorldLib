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

#define ACTIVATE_TIME 	1200

// 啟動核彈
void do_nuke(object me, string arg, object ob)
{
	if( !BATTLEFIELD_D->in_battle(me) && !wizardp(me) )
		return tell(me, me->query_idname()+"不在戰場中，無法啟動核彈攻擊。\n");
		
	if( time() - query_temp("nuclear_bomb/time", me) < ACTIVATE_TIME )
		return tell(me, "必須等待 "+ACTIVATE_TIME+" 秒鐘後才能再次使用"+ob->query_idname()+"。\n");
		
	if( environment(ob) == me )
	{
		msg("$ME小心翼翼地啟動$YOU並放置在地面。\n", me, ob, 1);
		new("/obj/weapon/nuclear_bomb", me, ob)->move_to_environment(me);
	}
	else if( environment(ob)->is_maproom() )
	{
		msg("$ME小心翼翼地啟動地面上的$YOU。\n", me, ob, 1);
		new("/obj/weapon/nuclear_bomb", me, ob)->move_to_environment(ob);
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
			"nuke"	: (: do_nuke :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"核彈",
	
	//英文名稱
	"id":		"nuclear bomb",

	//材料需求
	"material":	([ "precious_metal":50, "liquid_fuel":100, "man_made_fibers":5, "mold":5, "integrated_circuit":10, "gem":20 ]),

	//技能需求
	"skill":	([ "longrange-fac":100, "technology":100 ]),

	//生產工廠
	"factory":	"longrange",
]);
