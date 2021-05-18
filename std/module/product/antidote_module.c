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

string* available_poisons = ({ POISON_GIANT_SPIDER, MAGNETIC_STONE });

// 注射解毒劑
void do_inject(object me, string arg, object ob)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要幫誰注射"+ob->query_idname()+"？\n");
		
	if( arg == "me" )
		target = me;
	else if( !objectp(target = present(arg)) )
		return tell(me, "這裡沒有 "+arg+" 這個人。\n");
		
	if( !target->is_living() )
		return tell(me, pnoun(2, me)+"無法幫"+target->query_idname()+"注射"+ob->query_idname()+"。\n");
		
	msg("$ME拿起"+ob->query_idname()+"往"+(me==target?"$ME":"$YOU")+"手臂上注射進去。\n", me, target==me?0:target, 1);
	
	foreach(string poison in available_poisons)
	{
		if( target->in_condition(poison) )
			target->stop_condition(poison);
	}

	destruct(ob, 1);
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"inject"	: (: do_inject :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"解毒劑",
	
	//英文名稱
	"id":		"antidote",

	//材料需求
	"material":	([ "distilled_water":10, "/obj/farming/product/herb":20, "glass":1 ]),

	//技能需求
	"skill":	([ "adventure-fac":70, "technology":70 ]),

	//生產工廠
	"factory":	"adventure",
]);
