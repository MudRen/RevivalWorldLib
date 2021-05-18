/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-12
 * Note   : �ʻs��M
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("coarse blade", "�ʻs��M");
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӵ}�����ʻs��M�C\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ 
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
		"/obj/gem/gem6" : 1,
	]));

	set("skill", "blade");
	set(ATTACK, 2);
	set(BUFF_STR, 3);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME�����"+this_object()->query_name()+"��$YOU��������A";
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	if( !random(15) )
	{
		int damage = range_random(30, 90);

		msg("$ME�ϥX�P�R����A��$YOU�y���ˮ`�I\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
