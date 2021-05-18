/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
 * Update :
 *  o 2012-02-12 Fix function special_attack call object undefined error By Sinji
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <condition.h>
#include <daemon.h>

inherit STANDARD_MODULE_PRODUCT;
inherit EQUIPMENT;

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU��A�����A";
}

float calculate_effect(int value)
{
	return pow(value, 0.5) / pow(2, 0.5);
}

void remove_hydra_stick_2nd_buff(object attacker)
{
	delete_temp(BUFF_ANTIFATAL_CHANCE);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	int hydra_stick_1st = query("hydra_stick_secret_art_1st");
	int hydra_stick_2nd = query("hydra_stick_secret_art_2nd");
	int sky_2nd = query("sky_2nd_secret_art");
	
	if( !random(15) )
	{
		object* targets = COMBAT_D->query_fighting_targets(attacker);
		int damage = to_int(range_random(30, 120) * pow(to_float(sizeof(targets)), 0.8) / sizeof(targets));
		
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
                   if( undefinedp(ob) ) continue;
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			damage = range_random(30, 120);

			msg("$ME�ϥX�j�d�������A��$YOU�y���Y���ˮ`�I\n", attacker, defender, 1);
			
			COMBAT_D->cause_damage(attacker, defender, damage);
		}
		
		return;
	}

	if( hydra_stick_1st > 0 )
	{
		if( !random(15) )
		{
			object* targets = COMBAT_D->query_fighting_targets(attacker);
			int damage = to_int(range_random(100, 500) * calculate_effect(hydra_stick_1st) * pow(to_float(sizeof(targets)), 0.8) / sizeof(targets));
	
			foreach(object ob in targets)
			{
				if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
	
				msg("$ME�@�ⴭ�_�۴ҡA�Ҽv���a�ƨ����u"HIW"�E�I"NOR WHT"�Ǽv"NOR"�v�ۦU�Ӥ�V��$YOU���h�A�y���Y���ˮ`�I\n", attacker, ob, 1);
				
				COMBAT_D->cause_damage(attacker, ob, damage);
			}
			
			return;
		}
	}

	if( hydra_stick_2nd > 0 )
	{
		if( !random(to_int(100. / calculate_effect(hydra_stick_2nd))) )
		{
			msg("$ME�o�ʡu"HIG"�u�@"NOR GRN"���D"NOR"�v�A�u�ɶ������ɨ��� "HIG"10%"NOR" ��������O�I\n", attacker, defender, 1);
			
			set_temp(BUFF_ANTIFATAL_CHANCE, 10);
			
			call_out((: remove_hydra_stick_2nd_buff($(attacker)) :), 60);
				
			attacker->reset_buff_cache();
			
			return;
		}
	}
	
	if( sky_2nd > 0 )
	{
		if( !random(to_int(100. / calculate_effect(sky_2nd))) )
		{
			object small_statue = new("/obj/npc/boss/small_statue", attacker);
			
			small_statue->move_to_environment(attacker);
			
			msg("\n$ME�C�n�e�p�I�ۡG�u"HIY"�h�a�I"NOR"�v�A�@�����۱q$ME���W���U�A�ۤƦ�$YOU�å[�J�԰��C\n\n", attacker, small_statue, 1);
			
			COMBAT_D->start_fight(small_statue, defender);
			
			return;
		}	
	}
}

string query_description()
{
	string description = "";

	int hydra_stick_1st = query("hydra_stick_secret_art_1st");
	int hydra_stick_2nd = query("hydra_stick_secret_art_2nd");
	int sky_2nd = query("sky_2nd_secret_art");

	if( hydra_stick_1st > 0 )
		description += sprintf("  %-20s - �����ˮ` "HIY"%d"NOR, HIW"���w��"NOR WHT"�Ҫk"NOR WHT"�D"HIW"�Ĥ@��"NOR, to_int(300 * calculate_effect(hydra_stick_1st)))+"\n";
	
	if( hydra_stick_2nd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"���w��"NOR WHT"�Ҫk"NOR WHT"�D"HIW"�ĤG��"NOR, 100./(100./calculate_effect(hydra_stick_2nd)))+"\n";

	if( sky_2nd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"�ѼC"NOR WHT"�S��"NOR, 100./(100./calculate_effect(sky_2nd)))+"\n";

	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "��");
	set("mass", 2000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "stick");

	delete("buff");

	set(DEFEND, 5);
	set(BUFF_PHY, 5);

	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);

	setup_inlay();
}
