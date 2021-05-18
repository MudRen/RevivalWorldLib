/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : purewater.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : �M��
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
	set_idname("pure water","�M��");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�M���q���M���C");
	set("unit", "�M");
	set("capacity", 50);
	set("mass", 400);
	set("value", 1000);
	set("badsell", 1);
	set("effect", 100);
}

string query_description()
{
	return "�B��_ "+query("effect")+" ��O�B���� "+query("capacity");
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
	
	msg("$ME�B�P�B�P�a�ܤU$YOU�A��_�F�ǳ\��O��("HIG+effect+NOR")�C\n", me, this_object(), 1);
	
	return 1;
}
