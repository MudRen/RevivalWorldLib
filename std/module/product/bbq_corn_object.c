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
	return "�B��_ "+query("effect")+" �ͩR�B���� "+query("capacity");
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "��");
	set("capacity", 20);
	set("mass", 200);
	set("value", query_value());
	set("effect", 100);
	setup_inlay();
}

int do_eat(object me)
{
	int effect = query("effect");
	string stance_type = query("combat/stance/type", me);
	
	if( stance_type == "medic" )
		effect = to_int(effect * (1. + me->query_skill_level("medic-stance")/500.));
	else if( stance_type == "medic-adv" )
		effect = to_int(effect * (1.2 + me->query_skill_level("medic-stance-adv")/500.));
		
	me->earn_health(effect);
	
	msg("$ME�j�f���Y�U$YOU�A��_�F�ǳ\�ͩR��("HIR+effect+NOR")�C\n", me, this_object(), 1);
	
	return 1;
}
