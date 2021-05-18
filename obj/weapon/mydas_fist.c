/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mydas_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �������M
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
	set_idname(HIG"mydas"NOR GRN" fist"NOR, HIG"����"NOR GRN"���M"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӱq�����t�W�������S���M�C\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 2,
		"/obj/gem/gem1" : 2,
		"/obj/gem/gem2" : 2,
		"/obj/gem/gem3" : 2,
		"/obj/gem/gem4" : 2,
		"/obj/gem/gem5" : 2,
		"/obj/gem/gem6" : 2,
	]));

	set("skill", "unarmed");

	set(ATTACK, 5);
	set(BUFF_STR, 1);
	set(BUFF_PHY, 3);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU���������A";
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	if( !random(15) )
	{
		int combo = range_random(4, 8);
		int damage = range_random(15, 25)*combo;

		for(int i=0;i<combo;i++)
			msg("$ME�ϥX�g�ɳs���A�s��$YOU�i����������I("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
