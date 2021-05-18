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
	return "$ME�����"+this_object()->query_name()+"��$YOU��������A";
}

float calculate_effect(int value)
{
	return pow(value, 0.5) / pow(2, 0.5);
}

string axe_message = @TEXT
                           ..
                           ;:
                          .::.
                         . ::`.                      `;.
                        :  ::  :                      ; :.
                       :   ::   :                    .;  :.
                      :    ::    :                   ::   :.
                     :     ::     :                .::    ::
                     :     ::     :               .::      ::
                     `.    ::    .               .::       ::
                      `.   ::   .               .::         ::
                        `. :: .               .::           ::
                         :    :            .:::             :;
                  _--.   :....:       ...::::               :::
              __-`    `-.:....:.---:::::::                  ::;
    ___----           ..............                        ::;
--==___===============..............                        ::;
       ----___          .........:::::..                    ::;
              ``-_    .  :\\//: ```:::::::..                :;
                  `--    ://\\:        ``:::::.             :;
                         :\\//:            ``:::.           :;
                         ://\\:               `::.         :;
                         :\\//:                :::.        :;
                         ://\\:                :::;       :;
                         :\\//:                ::::      :;
                         ://\\:                :::      :;
                         :\\//:                :::      :
                         ://\\:               :::      :
                         :\\//:               :::     :
                         ://\\:               ;;     :
                         :\\//:               ;     :
                         ://\\:              ;     :
                         [____]              ;    :
                         [____]             ;    :
                         [____]            .    :
                         [____]           .    :
                         [____]          .   .:`
                         [____]         .    :
                         :\\//:         :    :
                         ://\\:         `.   `.
                         :\\//:          `.   `.
                         ://\\:            `.   ;
                         [____]              `..
                         [____]
                         [____]
                         [____] 
                         [____]
                         [____]
                         :....:
                       .-  :: `-.
                     .     ::  ..`.
                    /      ::  `   \
                   :       ::       ;
                   ::::::::::::::::::
                   :       ::       ;
                    \      ::      /
                     `.    ::    .
                       `-..::..-
TEXT;

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	int truth_axe_secret_art_1st = query("truth_axe_secret_art_1st");
	int truth_axe_secret_art_2nd = query("truth_axe_secret_art_2nd");
	int ancient_blade_1st = query("ancient_blade_secret_art_1st");
	int ancient_blade_2nd = query("ancient_blade_secret_art_2nd");
	int sky_3rd = query("sky_3rd_secret_art");

	// 90
	if( !random(15) )
	{
		int damage = range_random(30, 150);

		msg("$ME�ϥX�P�R����A��$YOU�y���Y���ˮ`�I\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	if( truth_axe_secret_art_1st > 0 && !random(to_int(1000. / calculate_effect(truth_axe_secret_art_1st))) )
	{
		int energy = attacker->query_energy_cur();
		int ratio = range_random(5, 10);
		int damage;
		
		if( energy < 20 )
			damage = ratio;
		else
			damage = ratio * energy / 20;
		
		if( userp(defender) && random(3) )
			;
		else
		{
			msg("$ME�j�ܤ@�n�A�ӶO "HIC+ratio+NOR CYN"%"NOR" ���믫�o�ʡu"WHT"�u�z"NOR WHT"���q"NOR"�v��$YOU�y���������ˮ`�I�I\n", attacker, defender, 1);
			
			if( energy > damage )
				attacker->cost_energy(damage / 5);
			else
				attacker->cost_energy(energy - 1);
				
			COMBAT_D->cause_damage(attacker, defender, damage);
		}
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	if( truth_axe_secret_art_2nd > 0 && !random(to_int(300. / calculate_effect(truth_axe_secret_art_2nd))) )
	{
		int damage = range_random(1000, 2000);
		
		if( userp(defender) && random(3) )
			;
		else
		{
			msg("$ME�o�ʡu"HIW"�u�z"NOR WHT"�g��"NOR"�v\n"HIW+axe_message+NOR"��$YOU�y���Y���ˮ`�I�I\n", attacker, defender, 1);	
		
			COMBAT_D->cause_damage(attacker, defender, damage);
		}
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	if( ancient_blade_2nd > 0 )
	{
		if( !random(to_int(50. / calculate_effect(ancient_blade_2nd))) && !defender->is_boss() )
		{
			int time;
	
			if( userp(defender) && random(5) )
				;
			else
			{
				msg("$ME�ϥX�u"HIR"�Ϥ�"NOR RED"�M�k"NOR"�v�A"+this_object()->query_idname()+"���M�I��������$YOU���ḣ�A��$YOU�y���w�t�I\n", attacker, defender, 1);
		
				if( userp(defender) )
					time = 1;
				else
					time = 3;
					
				defender->start_delay("forgotten_ancient_knight", time, pnoun(2, defender)+"���b�w�t���C\n", pnoun(2, defender)+"����F�w�t�C\n");
			}
		}
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}

	// 300
	if( ancient_blade_1st > 0 )
	{
		if( !random(15) )
		{
			int damage = range_random(100, 500) * calculate_effect(ancient_blade_1st);
	
			if( userp(defender) && random(3) )
				;
			else
			{
				msg("$ME�����M�X�{�u"HIW"���j"NOR WHT"�Z�h"NOR"�v�����v�A����"+this_object()->query_idname()+"�M���|�g�A���X���r�����A��$YOU�y���Y���ˮ`�I\n", attacker, defender, 1);
	
				COMBAT_D->cause_damage(attacker, defender, damage);	
			}
		}
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	// 800x6
	if( sky_3rd > 0 )
	{
		if( !random(to_int(100. / calculate_effect(sky_3rd))) )
		{
			msg("\n$ME����@���A�j�q�u"WHT"�Q�¸H��"NOR"�v�ۦa�U«�X���ۦb$YOU���W�A$YOU�y�������o�C�A"HIG"���r�I�I\n\n"NOR, attacker, defender, 1);
			defender->start_condition(MAGNETIC_STONE, 6, 1, attacker);
		}
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
}

string query_description()
{
	string description = "";

	int truth_axe_secret_art_1st = query("truth_axe_secret_art_1st");
	int truth_axe_secret_art_2nd = query("truth_axe_secret_art_2nd");
	int ancient_blade_1st = query("ancient_blade_secret_art_1st");
	int ancient_blade_2nd = query("ancient_blade_secret_art_2nd");
	int sky_3rd = query("sky_3rd_secret_art");

	if( truth_axe_secret_art_1st > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"�u�z"NOR WHT"����"NOR WHT"�D"HIW"�Ĥ@��"NOR, 100./(1000./calculate_effect(truth_axe_secret_art_1st)))+"\n";
		
	if( truth_axe_secret_art_2nd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"�u�z"NOR WHT"����"NOR WHT"�D"HIW"�ĤG��"NOR, 100./(300./calculate_effect(truth_axe_secret_art_2nd)))+"\n";
		
	if( ancient_blade_1st > 0 )
		description += sprintf("  %-20s - �����ˮ` "HIY"%d"NOR, HIM"���j"NOR MAG"�M�k"NOR WHT"�D"HIW"�Ĥ@��"NOR, to_int(300 * calculate_effect(ancient_blade_1st)))+"\n";
	
	if( ancient_blade_2nd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIM"���j"NOR MAG"�M�k"NOR WHT"�D"HIW"�ĤG��"NOR, 100./(50./calculate_effect(ancient_blade_2nd)))+"\n";
	
	if( sky_3rd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"�ѻ�"NOR WHT"�S��"NOR, 100./(100./calculate_effect(sky_3rd)))+"\n";
		
	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "��");
	set("mass", 1000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "axe");
	
	delete("buff");
	
	set(ATTACK, 5);
	set(BUFF_STR, 5);
	set(BUFF_PHY, 5);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);

	setup_inlay();
}
