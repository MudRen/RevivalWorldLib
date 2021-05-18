/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dark_plum.c
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

inherit STANDARD_OBJECT;
inherit FOOD;

void create()
{
	set_idname("dark plum", "酸梅");
	
	if( this_object()->set_shadow_database() ) return;
	
	set("long", "一粒普通的酸梅。");
	set("unit", "粒");
	set("capacity", 50);
	set("mass", 400);
	set("value", 1000);
	set("badsell", 1);
	set("effect", 100);
}

string query_description()
{
	return "、恢復 "+query("effect")+" 精神、飽食 "+query("capacity");
}

int do_eat(object me)
{
	int effect = query("effect");
	string stance_type = query("combat/stance/type", me);
	
	if( stance_type == "medic" )
		effect = to_int(effect * (1. + me->query_skill_level("medic-stance")/500.));
	else if( stance_type == "medic-adv" )
		effect = to_int(effect * (1.2 + me->query_skill_level("medic-stance-adv")/500.));
		
	me->earn_energy(effect);
	
	msg("$ME大口的吃下$YOU，恢復了些許精神值("HIB+effect+NOR")。\n", me, this_object(), 1);
	
	return 1;
}
