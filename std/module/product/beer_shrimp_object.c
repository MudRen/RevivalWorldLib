/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>

inherit STANDARD_MODULE_PRODUCT;
inherit FOOD;

string query_description()
{
	return "、一秒後恢復 "+query("effect")+"% 生命、飽食 "+query("capacity");
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "盤");
	set("capacity", 400);
	set("mass", 200);
	set("value", query_value());
	set("effect", 20);

	setup_inlay();
}

int do_eat(object me)
{
	int effect;
	string stance_type = query("combat/stance/type", me);
	
	effect = me->query_health_max() * query("effect") / 100;
	
	if( stance_type == "medic" )
		effect = to_int(effect * (1. + me->query_skill_level("medic-stance")/500.));
	else if( stance_type == "medic-adv" )
		effect = to_int(effect * (1.2 + me->query_skill_level("medic-stance-adv")/500.));
		
	msg("$ME大口的吃下$YOU，稍待一秒後將恢復生命值("HIR+effect+NOR")。\n", me, this_object(), 1);
	
	call_out( (: objectp($(me)) && $(me)->earn_health($(effect)) :), 1);
	
	return 1;
}
