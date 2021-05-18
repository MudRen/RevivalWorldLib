/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra_stick.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ���w�ԥ۴�
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
#include <condition.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIW"hydra"NOR WHT" stick"NOR, HIW"���w��"NOR WHT"�۴�"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@��q�E�Y�D���w�ԤW���������j�۴ҡC\n");
	set("unit", "��");
	set("mass", 10000);
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
		"/obj/inlay/hydra_stick_secret_art_1st" : 1,
		"/obj/inlay/hydra_stick_secret_art_2nd" : 1,
	]));

	set("skill", "stick");

	set(DEFEND, 20);
	set(BUFF_PHY, 6);
	
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
	if( !random(15) )
	{
		object* targets = COMBAT_D->query_fighting_targets(attacker);
		int damage = to_int(range_random(100, 500) * pow(to_float(sizeof(targets)), 0.8) / sizeof(targets));

		foreach(object ob in targets)
		{
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			msg("$ME�@�ⴭ�_�۴ҡA�Ҽv���a�ƨ����u"HIW"�E�I"NOR WHT"�Ǽv"NOR"�v�ۦU�Ӥ�V��$YOU���h�A�y���Y���ˮ`�I\n", attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}
