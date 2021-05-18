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
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

#define REGEN	50

mapping inlay_effect =
([
	"/obj/materials/metal" : (: addn("effect", $1 * 20, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 20, $2) :),
});

// 睡覺休息
void do_sleep(object me, string arg, object ob)
{
	int cost = me->query_stamina_max() - me->query_stamina_cur();
	int time;
	int regen;

	if( COMBAT_D->in_fight(me) )
		return tell(me, pnoun(2, me)+"正在戰鬥中。\n");

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	if( !random(query("effect", ob)) )
	{
		msg("$ME興奮地鑽進$YOU裡，「啪！」的一聲支撐帳篷的鐵架居然斷掉了，看來是沒辦法再用了。\n", me, ob, 1);
		
		destruct(ob, 1);
		
		return;
	}

	msg("$ME舒服地躺在$YOU裡，打了個呵欠假寐一下(恢復 "+cost+" 體力)。\n", me, ob, 1);
	
	regen = me->stamina_regen()+REGEN;
	
	time = 2*cost/regen + 1;
	
	if( time <= 0 )
		tell(me, "休息完畢。\n");
	else
	{
		set_temp("rest_regen/stamina", REGEN, me);
		me->start_delay(REST_DELAY_KEY, time, pnoun(2, me)+"正在"+ob->query_idname()+"內休息。\n", "休息完畢。\n", bind((: delete_temp("rest_regen", $(me)), $(me)->set_stamina_full() :), me));
	}
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "睡覺休息。",
	"action":
		([
			"sleep"	: (: do_sleep :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"帳篷",
	
	//英文名稱
	"id":		"tent",

	//材料需求
	"material":	([ "man_made_fibers":1, "steel":1]),

	//技能需求
	"skill":	([ "adventure-fac":10, "technology":10 ]),

	//生產工廠
	"factory":	"adventure",
]);
