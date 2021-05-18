/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : steamed_roll.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : 饅頭
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <condition.h>

inherit STANDARD_OBJECT;
inherit FOOD;

void create()
{
	set_idname("steamed roll", "饅頭");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "一顆普通的饅頭。");
	set("unit", "顆");
	set("capacity", 100);
	set("mass", 400);
	set("value", 1000);
	set("badsell", 1);
	set("effect", 100);
}

string query_description()
{
	return "、每兩秒恢復 "+query("effect")+" 生命、飽食 "+query("capacity");
}

int do_eat(object me)
{	
	int effect = query("effect");
	string stance_type = query("combat/stance/type", me);
	
	if( stance_type == "medic" )
		effect = to_int(effect * (1. + me->query_skill_level("medic-stance")/500.));
	else if( stance_type == "medic-adv" )
		effect = to_int(effect * (1.2 + me->query_skill_level("medic-stance-adv")/500.));
		
	set_temp("steamed_roll_heal_effect", effect, me);
	
	msg("$ME大口的吃下$YOU，接下來每兩秒將恢復了生命值("HIR+effect+NOR")。\n", me, this_object(), 1);
	
	me->start_condition(STEAMED_ROLL_HEAL);
	
	return 1;
}
