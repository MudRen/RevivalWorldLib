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

//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
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

		return;
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
				return;
	
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
			
			return;
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
				return;
	
			for(int i=0;i<combo;i++)
				msg("$ME使出狂暴連擊「"HIB"狂"NOR BLU"嵐"NOR"」，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
				
			COMBAT_D->cause_damage(attacker, defender, damage);

			return;
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
			
			return;
		}
	}
}

string query_description()
{
	string description = "";

	int nessiteras_fist_1st = query("nessiteras_fist_secret_art_1st");
	int nessiteras_fist_2nd = query("nessiteras_fist_secret_art_2nd");
	int sky_1st = query("sky_1st_secret_art");
	
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
	
	set("skill", "unarmed");
	
	delete("buff");
	
	set(SPEED, 5);
	set(BUFF_AGI, 5);

	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);

	setup_inlay();
}
