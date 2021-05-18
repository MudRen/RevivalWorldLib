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
	"/obj/materials/wood" : (: addn("effect", $1 * 10, $2) :),
]);

// 聽音響
void do_play(object me, string arg, object ob)
{
	object cd;
	object env = environment(ob);
	
	if( !env->is_module_room() || !query_temp("decorated", ob) )
		return tell(me, ob->query_idname()+"必須裝潢在建築物內才能使用。\n");
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, env->query_room_name(1)+"不是"+pnoun(2, me)+"的建築物，無法在這裡使用音響。\n");

	if( !arg || !objectp(cd = present(arg)) )
		return tell(me, pnoun(2, me)+"想要播放哪一張 CD？\n");

	if( !cd->is_compact_disk() )
		return tell(me, cd->query_idname()+"不是 CD。\n");

	if( !random(500) )
	{
		msg("$ME輕輕地按下 PLAY 鍵，不知如何突然「碰」的一聲，$YOU冒出陣陣黑煙，壞掉了！！\n", me, ob, 1);
		destruct(ob, 1);
		return;
	}
		
	cd->play_cd(me, query("effect", ob));
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "聆聽音響。",
	"action":
		([
			"play"	: (: do_play :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"音響",
	
	//英文名稱
	"id":		"audio",

	//材料需求
	"material":	([ "integrated_circuit":2, "refined_timber":5, "plastic":3, "wire":1 ]),

	//技能需求
	"skill":	([ "furniture-fac":90, "technology":90 ]),

	//生產工廠
	"factory":	"furniture",
]);
