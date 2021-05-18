/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dark_plum.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : �C�Y
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
	set_idname("dark plum", "�ı�");
	
	if( this_object()->set_shadow_database() ) return;
	
	set("long", "�@�ɴ��q���ı��C");
	set("unit", "��");
	set("capacity", 50);
	set("mass", 400);
	set("value", 1000);
	set("badsell", 1);
	set("effect", 100);
}

string query_description()
{
	return "�B��_ "+query("effect")+" �믫�B���� "+query("capacity");
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
	
	msg("$ME�j�f���Y�U$YOU�A��_�F�ǳ\�믫��("HIB+effect+NOR")�C\n", me, this_object(), 1);
	
	return 1;
}
