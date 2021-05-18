/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ancient_blade.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ���j���M
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
	set_idname(HIM"ancient"NOR MAG" blade"NOR, HIM"���j"NOR MAG"���M"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӱq�Q��Ѫ����j�Z�h�W���������M�C\n");
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
		"/obj/inlay/ancient_blade_secret_art_1st" : 1,
		"/obj/inlay/ancient_blade_secret_art_2nd" : 1,
	]));

	set("skill", "blade");

	set(ATTACK, 20);
	set(BUFF_STR, 6);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU��������A";
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	// BOSS ����Q stun
	if( !random(50) && !defender->is_boss() )
	{
		int time;

		if( userp(defender) && random(5) )
			return;

		msg("$ME�ϥX�u"HIR"�Ϥ�"NOR RED"�M�k"NOR"�v�A"+this_object()->query_idname()+"���M�I��������$YOU���ḣ�A��$YOU�y���w�t�I\n", attacker, defender, 1);

		if( userp(defender) )
			time = 1;
		else
			time = 3;
			
		defender->start_delay("forgotten_ancient_knight", time, pnoun(2, defender)+"���b�w�t���C\n", pnoun(2, defender)+"����F�w�t�C\n");
		
		return;
	}

	if( !random(15) )
	{
		int damage = range_random(100, 500);

		if( userp(defender) && random(3) )
			return;
		
		msg("$ME�����M�X�{�u"HIW"���j"NOR WHT"�Z�h"NOR"�v�����v�A����"+this_object()->query_idname()+"�M���|�g�A���X���r�����A��$YOU�y���Y���ˮ`�I\n", attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
