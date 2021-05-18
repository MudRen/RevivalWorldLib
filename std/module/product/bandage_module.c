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
#include <npc.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;

mapping inlay_effect =
([
	"/obj/farming/product/cotton" : (: addn("effect", ($1 > 100 ? 100 : $1) * 2, $2) :),
]);

array enhancement = 
({ 
	(: addn("effect", $1 * 2, $2) :),
});

// 包紮繃帶
void do_heal(object me, string arg, object ob)
{
	string stance_type = query("combat/stance/type", me);
	object *enemies;
	object target;
	float effect;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要幫誰包紮"+ob->query_idname()+"？\n");
		
	if( me->is_module_npc() && query("job/type", me) != SOLDIER )
		return me->do_command("say 我不是軍人無法替人包紮\n");
		
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		if( me->is_module_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}
	
	if( query("killlock", me) )
		return tell(me, pnoun(2, me)+"目前處在戰鬥鎖定狀態，無法進行包紮。\n");

	if( arg == "me" )
		target = me;
	else if( !objectp(target = present(arg)) )
		return tell(me, "這裡沒有 "+arg+" 這個人。\n");
		
	if( !target->is_living() )
	{
		if( me->is_module_npc() )
			return me->do_command("say 我無法幫"+target->query_idname()+"包紮"+ob->query_idname()+"。\n");
		else
			return tell(me, pnoun(2, me)+"無法幫"+target->query_idname()+"包紮"+ob->query_idname()+"。\n");
	}

	if( !me->is_in_my_group(target) )
	{
		if( me->is_module_npc() )
			return me->do_command("say 我不在"+target->query_idname()+"的隊伍裡，無法幫"+pnoun(3, target)+"進行包紮。\n");
		else
			return tell(me, pnoun(2, me)+"不在"+target->query_idname()+"的隊伍裡，無法幫"+pnoun(3, target)+"進行包紮。\n");
	}

	if( !me->cost_energy(50) )
	{
		if( me->is_module_npc() )
			return me->do_command("say 我沒有足夠的精神再進行包紮了。\n");
		else
			return tell(me, pnoun(2, me)+"沒有足夠的精神再進行包紮了。\n");
	}

	effect = to_float(query("effect", ob));

	if( stance_type == "medic" )
		effect *= 1. + me->query_skill_level("medic-stance")/500.;
	else if( stance_type == "medic-adv" )
		effect *= 1.2 + me->query_skill_level("medic-stance-adv")/500.;

	target->earn_health(to_int(effect));

	msg("$ME將"+ob->query_idname()+"包紮在$YOU的傷口上，使$YOU恢復了 "HIY+to_int(effect)+NOR" 點的生命值。\n", me, target, 1);
	
	destruct(ob, 1);
	
	enemies = COMBAT_D->query_fighting_targets(target);
	
	if( !sizeof(enemies) )
		return;
		
	foreach(ob in enemies)
	{
		if( !COMBAT_D->in_fight(me, ob) && ob->valid_enemy(me) && !ob->is_in_my_group(me) )
		{
			msg(HIR"$ME"HIR"想要殺死$YOU！！\n"NOR, ob, me, 1);
			COMBAT_D->start_fight(me, ob);
		}
		
		ob->calculate_combat_hate(me, to_int(effect)/2);
	}
}

// 設定物件動作
nosave mapping action_info =
([
	"help"	: "尚未建構說明。",
	"action":
		([
			"heal"		: (: do_heal :),
		]),
]);

// 設定物件資料
nosave mapping product_info = 
([	
	//中文名稱
	"name":		"繃帶",
	
	//英文名稱
	"id":		"bandage",

	//材料需求
	"material":	([ "/obj/farming/product/cotton":10, "plastic":2 ]),

	//技能需求
	"skill":	([ "adventure-fac":50, "technology":50 ]),

	//生產工廠
	"factory":	"adventure",
]);
