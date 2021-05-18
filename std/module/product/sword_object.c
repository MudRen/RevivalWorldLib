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
#include <daemon.h>

inherit STANDARD_MODULE_PRODUCT;
inherit EQUIPMENT;

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU���������A";
}

float calculate_effect(int value)
{
	return pow(value, 0.5) / pow(2, 0.5);
}

array swordart = ({ ({HIW"�s"NOR, 50}), ({HIR"�}"NOR, 60}), ({HIY"�_"NOR, 70}), ({HIM"��"NOR, 80}), ({HIB"��"NOR, 90}), ({HIG"��"NOR, 100}), ({YEL"�h"NOR, 110}), ({GRN"�I"NOR, 120}), ({MAG"��"NOR, 130}), ({HIC"��"NOR, 140}), ({WHT"��"NOR, 150}) });

void remove_attack_buff(object attacker)
{
	delete_temp(ATTACK);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	int lord_sword_secret_art_1st = query("lord_sword_secret_art_1st");
	int lord_sword_secret_art_2nd = query("lord_sword_secret_art_2nd");
	int nessiteras_fist_1st = query("nessiteras_fist_secret_art_1st");
	int nessiteras_fist_2nd = query("nessiteras_fist_secret_art_2nd");
	int sky_1st = query("sky_1st_secret_art");
	
	// 160
	if( !random(15) )
	{
		int combo = range_random(2, 6);
		int damage = range_random(15, 25)*combo;

		for(int i=0;i<combo;i++)
			msg("$ME�ϥX�g�ɳs���A�s��$YOU�i����������I("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
			
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	if( lord_sword_secret_art_1st > 0 && !random(to_int(100. / calculate_effect(lord_sword_secret_art_1st))) )
	{
		int again = 0;
		int damage = 0;
		int total_damage = 0;
		string art;
		array damagetype;
		
		do
		{
			damage = 0;
			art = "";
			
			for(int i=0;i<6;++i)
			{
				damagetype = swordart[random(sizeof(swordart))];
				damage += damagetype[1];
				
				art += damagetype[0]+( i < 5 ? "�B":"");
			}
					
			msg("$ME�o�X�u"HIG"�j��"NOR GRN"���C"NOR"�v�A�ϥX�C�Z�u"+art+"�v��$YOU�y����H���ˮ`�I\n", attacker, defender, 1);

			if( damage < 400 )
			{
				msg("$ME�j�ܤ@�n�A�A���o�ʡu"HIG"�j��"NOR GRN"���C"NOR"�v�I�I�I�I\n", attacker, defender, 1);
				again = 1;
			}
			else
			{
				again = 0;
			}
			
			total_damage += damage;
				
		} while (again);
		
		COMBAT_D->cause_damage(attacker, defender, total_damage);
		
		if( !objectp(defender) || defender->is_dead() ) return;
	}
	
	if( lord_sword_secret_art_2nd > 0 && !random(to_int(500. / calculate_effect(lord_sword_secret_art_2nd))) )
	{
		msg("$ME�o�ʡu"HIC"�Q�D"NOR CYN"���C"NOR"�v�A�u�ɶ����Z������ "HIY"30"NOR" �I�����O�I\n", attacker, defender, 1);
		
		set_temp(ATTACK, 30);
		
		call_out((: remove_attack_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
	}
	
	// 500
	if( nessiteras_fist_2nd > 0 )
	{
		// BOSS ����Q stun
		if( !random(50) )
		{
			int time;
			int damage = range_random(400, 600) * calculate_effect(nessiteras_fist_2nd);
	
			if( userp(defender) && random(5) )
				;
			else
			{
				msg("$ME�ϥX�u"HIY"�}��"NOR YEL"��"NOR"�v�A"+this_object()->query_idname()+"�����R��$YOU���Y���I\n", attacker, defender, 1);
		
				if( !defender->is_boss() && !random(3) )
				{
					if( userp(defender) )
						time = 1;
					else
						time = 2;
		
					defender->start_delay("nessiteras_fist", time, pnoun(2, defender)+"�|���q�u"HIY"�}��"NOR YEL"��"NOR"�v����_�C\n", pnoun(2, defender)+"�q�u"HIY"�}��"NOR YEL"��"NOR"�v����_�F�C\n");
				}
				
				COMBAT_D->cause_damage(attacker, defender, damage);
			}
			
			if( !objectp(defender) || defender->is_dead() ) return;
		}
	}

	// 300
	if( nessiteras_fist_1st > 0 )
	{
		if( !random(15) )
		{
			int combo = range_random(4, 16) * calculate_effect(nessiteras_fist_1st);
			int damage = range_random(25, 35) * combo;
			
			if( userp(defender) && random(3) )
				;
			else
			{
				for(int i=0;i<combo;i++)
					msg("$ME�ϥX�g�ɳs���u"HIB"�g"NOR BLU"�P"NOR"�v�A�s��$YOU�i����������I("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
					
				COMBAT_D->cause_damage(attacker, defender, damage);
			}
			
			if( !objectp(defender) || defender->is_dead() ) return;
		}
	}

	// 666
	if( sky_1st > 0 )
	{
		if( !random(to_int(100. / calculate_effect(sky_1st))) )
		{
			msg("�@�}�ѷn�a�ʡA�@�D"HIW"�ե�"NOR"���g�L�ӡA��$YOU�y���ˮ`�è�$ME�ͩR������_�F�@��("NOR RED"+"HIR"666"NOR")�C\n", attacker, defender, 1);	
			attacker->earn_health(666);
			
			COMBAT_D->cause_damage(attacker, defender, 666);
			
			if( !objectp(defender) || defender->is_dead() ) return;
		}
	}
}

string query_description()
{
	string description = "";

	int lord_sword_secret_art_1st = query("lord_sword_secret_art_1st");
	int lord_sword_secret_art_2nd = query("lord_sword_secret_art_2nd");
	int nessiteras_fist_1st = query("nessiteras_fist_secret_art_1st");
	int nessiteras_fist_2nd = query("nessiteras_fist_secret_art_2nd");
	int sky_1st = query("sky_1st_secret_art");
	
	if( lord_sword_secret_art_1st > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIY"����"NOR YEL"���C"NOR WHT"�D"HIW"�Ĥ@��"NOR, 100./(100./calculate_effect(lord_sword_secret_art_1st)))+"\n";
		
	if( lord_sword_secret_art_2nd > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIY"����"NOR YEL"���C"NOR WHT"�D"HIW"�ĤG��"NOR, 100./(500./calculate_effect(lord_sword_secret_art_2nd)))+"\n";
		
	if( nessiteras_fist_1st > 0 )
		description += sprintf("  %-20s - �����s�� "HIY"%d"NOR, HIB"����"NOR MAG"���k"NOR WHT"�D"HIW"�Ĥ@��"NOR, to_int(10 * calculate_effect(nessiteras_fist_1st)))+"\n";
	
	if( nessiteras_fist_2nd > 0 )
		description += sprintf("  %-20s - �����ˮ` "HIY"%d"NOR, HIB"����"NOR MAG"���k"NOR WHT"�D"HIW"�ĤG��"NOR, to_int(500 * calculate_effect(nessiteras_fist_2nd)))+"\n";
	
	if( sky_1st > 0 )
		description += sprintf("  %-20s - �o�ʾ��v "HIY"%.2f%%"NOR, HIW"�ѸV"NOR WHT"�S��"NOR, 100./(100./calculate_effect(sky_1st)))+"\n";

	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "��");
	set("mass", 500);
	set("value", query_value());
	set("flag/no_amount", 1);
	
	set("skill", "sword");
	
	delete("buff");
	
	set(SPEED, 5);
	set(BUFF_AGI, 5);
	set(BUFF_STR, 5);

	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);

	setup_inlay();
}
