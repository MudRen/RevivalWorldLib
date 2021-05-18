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
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU揮拳攻擊，";
}

float calculate_effect(int value)
{
	return pow(value, 0.5) / pow(2, 0.5);
}

array swordart = ({ ({HIW"連"NOR, 50}), ({HIR"破"NOR, 60}), ({HIY"斷"NOR, 70}), ({HIM"易"NOR, 80}), ({HIB"勢"NOR, 90}), ({HIG"散"NOR, 100}), ({YEL"退"NOR, 110}), ({GRN"截"NOR, 120}), ({MAG"瞬"NOR, 130}), ({HIC"揚"NOR, 140}), ({WHT"隱"NOR, 150}) });

void remove_attack_buff(object attacker)
{
	delete_temp(ATTACK);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

//
// 武器自動特殊攻擊
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
			msg("$ME使出狂暴連擊，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
			
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
				
				art += damagetype[0]+( i < 5 ? "、":"");
			}
					
			msg("$ME發出「"HIG"迴風"NOR GRN"六劍"NOR"」，使出劍訣「"+art+"」對$YOU造成驚人的傷害！\n", attacker, defender, 1);

			if( damage < 400 )
			{
				msg("$ME大喝一聲，再次發動「"HIG"迴風"NOR GRN"六劍"NOR"」！！！！\n", attacker, defender, 1);
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
		msg("$ME發動「"HIC"霸道"NOR CYN"之劍"NOR"」，短時間內武器提升 "HIY"30"NOR" 點攻擊力！\n", attacker, defender, 1);
		
		set_temp(ATTACK, 30);
		
		call_out((: remove_attack_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
	}
	
	// 500
	if( nessiteras_fist_2nd > 0 )
	{
		// BOSS 不能被 stun
		if( !random(50) )
		{
			int time;
			int damage = range_random(400, 600) * calculate_effect(nessiteras_fist_2nd);
	
			if( userp(defender) && random(5) )
				;
			else
			{
				msg("$ME使出「"HIY"破天"NOR YEL"擊"NOR"」，"+this_object()->query_idname()+"直接命中$YOU的頭部！\n", attacker, defender, 1);
		
				if( !defender->is_boss() && !random(3) )
				{
					if( userp(defender) )
						time = 1;
					else
						time = 2;
		
					defender->start_delay("nessiteras_fist", time, pnoun(2, defender)+"尚未從「"HIY"破天"NOR YEL"擊"NOR"」中恢復。\n", pnoun(2, defender)+"從「"HIY"破天"NOR YEL"擊"NOR"」中恢復了。\n");
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
					msg("$ME使出狂暴連擊「"HIB"狂"NOR BLU"嵐"NOR"」，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
					
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
			msg("一陣天搖地動，一道"HIW"白光"NOR"直射過來，對$YOU造成傷害並使$ME生命瞬間恢復了一些("NOR RED"+"HIR"666"NOR")。\n", attacker, defender, 1);	
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
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIY"王者"NOR YEL"之劍"NOR WHT"．"HIW"第一式"NOR, 100./(100./calculate_effect(lord_sword_secret_art_1st)))+"\n";
		
	if( lord_sword_secret_art_2nd > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIY"王者"NOR YEL"之劍"NOR WHT"．"HIW"第二式"NOR, 100./(500./calculate_effect(lord_sword_secret_art_2nd)))+"\n";
		
	if( nessiteras_fist_1st > 0 )
		description += sprintf("  %-20s - 平均連擊 "HIY"%d"NOR, HIB"尼斯"NOR MAG"拳法"NOR WHT"．"HIW"第一式"NOR, to_int(10 * calculate_effect(nessiteras_fist_1st)))+"\n";
	
	if( nessiteras_fist_2nd > 0 )
		description += sprintf("  %-20s - 平均傷害 "HIY"%d"NOR, HIB"尼斯"NOR MAG"拳法"NOR WHT"．"HIW"第二式"NOR, to_int(500 * calculate_effect(nessiteras_fist_2nd)))+"\n";
	
	if( sky_1st > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIW"天禽"NOR WHT"特攻"NOR, 100./(100./calculate_effect(sky_1st)))+"\n";

	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "件");
	set("mass", 500);
	set("value", query_value());
	set("flag/no_amount", 1);
	
	set("skill", "sword");
	
	delete("buff");
	
	set(SPEED, 5);
	set(BUFF_AGI, 5);
	set(BUFF_STR, 5);

	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);

	setup_inlay();
}
