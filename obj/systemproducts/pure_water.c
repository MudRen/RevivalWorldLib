/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : purewater.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : 清水
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>

inherit STANDARD_OBJECT;
inherit DRINK;

void create()
{
	set_idname("pure water","清水");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "一杯普通的清水。");
	set("unit", "杯");
	set("capacity", 50);
	set("mass", 400);
	set("value", 1000);
	set("badsell", 1);
	set("effect", 100);
}

string query_description()
{
	return "、恢復 "+query("effect")+" 體力、飽食 "+query("capacity");
}

int do_drink(object me)
{	
	int effect = query("effect");
	string stance_type = query("combat/stance/type", me);
	
	if( stance_type == "medic" )
		effect = to_int(effect * (1. + me->query_skill_level("medic-stance")/500.));
	else if( stance_type == "medic-adv" )
		effect = to_int(effect * (1.2 + me->query_skill_level("medic-stance-adv")/500.));
		
	me->earn_stamina(effect);
	
	msg("$ME咕嚕咕嚕地喝下$YOU，恢復了些許體力值("HIG+effect+NOR")。\n", me, this_object(), 1);
	
	return 1;
}
