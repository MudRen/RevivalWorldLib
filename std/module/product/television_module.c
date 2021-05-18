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

mapping inlay_effect =
([
	"/obj/materials/metal" : (: addn("effect", $1 * 30, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 30, $2) :),
});

// 看電視
void do_watch(object me, string arg, object ob)
{
	object env = environment(ob);
	
	if( !env->is_module_room() || !query_temp("decorated", ob) )
		return tell(me, ob->query_idname()+"必須裝潢在建築物內才能觀看。\n");
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, env->query_room_name(1)+"不是"+pnoun(2, me)+"的建築物，無法在這裡看電視。\n");

	if( !random(500) )
	{
		msg("$ME興奮地打開$YOU，不知如何突然「碰」的一聲，$YOU冒出陣陣黑煙，壞掉了！！\n", me, ob, 1);
		destruct(ob, 1);
		return;
	}
		
	msg("$ME打開"+ob->query_idname()+"欣賞各種節目，對時事的瞭解加深了。\n", me, 0, 1);
	me->start_condition(TELEVISION, query("effect", ob));
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "觀看電視節目。",
	"action":
		([
			"watch"	: (: do_watch :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"電視機",
	
	//英文名稱
	"id":		"television",

	//材料需求
	"material":	([ "integrated_circuit":1, "glass":5, "mold":1, "wire":1 ]),

	//技能需求
	"skill":	([ "furniture-fac":70, "technology":70 ]),

	//生產工廠
	"factory":	"furniture",
]);
