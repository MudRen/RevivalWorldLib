/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_fist.c
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
	set_idname(HIB"nessiteras"NOR MAG" fist"NOR, HIB"����"NOR MAG"���M"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӱq��������ǤW�������S���M�C\n");
	set("unit", "��");
	set("mass", 1000);
	set("value", 100000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 4,
		"/obj/gem/gem1" : 4,
		"/obj/gem/gem2" : 4,
		"/obj/gem/gem3" : 4,
		"/obj/gem/gem4" : 4,
		"/obj/gem/gem5" : 4,
		"/obj/gem/gem6" : 4,
		"/obj/inlay/nessiteras_fist_secret_art_1st" : 1,
		"/obj/inlay/nessiteras_fist_secret_art_2nd" : 1,
	]));

	set("skill", "unarmed");

	set(SPEED, 20);
	set(BUFF_AGI, 6);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU�X�������A";
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	// BOSS ����Q stun
	if( !random(50) )
	{
		int time;
		int damage = range_random(400, 600);

		if( userp(defender) && random(5) )
			return;

		msg("$ME�ϥX�u"HIY"�}��"NOR YEL"��"NOR"�v�A"+this_object()->query_idname()+"�����R��$YOU���Y���I\n", attacker, defender, 1);

		if( !defender->is_boss() )
		{
			if( userp(defender) )
				time = 1;
			else
				time = 2;

			defender->start_delay("nessiteras_fist", time, pnoun(2, attacker)+"�|���q�u"HIY"�}��"NOR YEL"��"NOR"�v����_�C\n", pnoun(2, attacker)+"�q�u"HIY"�}��"NOR YEL"��"NOR"�v����_�F�C\n");
		}
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}

	if( !random(15) )
	{
		int combo = range_random(4, 16);
		int damage = range_random(25, 35)*combo;
		
		if( userp(defender) && random(3) )
			return;

		for(int i=0;i<combo;i++)
			msg("$ME�ϥX�g�ɳs���u"HIB"�g"NOR BLU"�P"NOR"�v�A�s��$YOU�i����������I("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
