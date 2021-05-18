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
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME手持著"+this_object()->query_name()+"往$YOU揮砍攻擊，";
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
	int ancient_blade_1st = query("ancient_blade_secret_art_1st");
	int ancient_blade_2nd = query("ancient_blade_secret_art_2nd");
	int sky_3rd = query("sky_3rd_secret_art");

	// 90
	if( !random(15) )
	{
		int damage = range_random(30, 150);

		msg("$ME使出致命揮砍，對$YOU造成嚴重傷害！\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);

		return;
	}
	
	if( ancient_blade_2nd > 0 )
	{
		if( !random(to_int(50. / calculate_effect(ancient_blade_2nd))) && !defender->is_boss() )
		{
			int time;
	
			if( userp(defender) && random(5) )
				return;
	
			msg("$ME使出「"HIR"反手"NOR RED"刀法"NOR"」，"+this_object()->query_idname()+"的刀背狠狠擊中$YOU的後腦，使$YOU造成暈眩！\n", attacker, defender, 1);
	
			if( userp(defender) )
				time = 1;
			else
				time = 3;
				
			defender->start_delay("forgotten_ancient_knight", time, pnoun(2, defender)+"正在暈眩中。\n", pnoun(2, defender)+"停止了暈眩。\n");
			
			return;
		}
	}

	// 300
	if( ancient_blade_1st > 0 )
	{
		if( !random(15) )
		{
			int damage = range_random(100, 500) * calculate_effect(ancient_blade_1st);
	
			if( userp(defender) && random(3) )
				return;
			
			msg("$ME身後突然出現「"HIW"遠古"NOR WHT"武士"NOR"」的身影，瞬間"+this_object()->query_idname()+"刀光四射，揮出兇猛攻擊，對$YOU造成嚴重傷害！\n", attacker, defender, 1);

			COMBAT_D->cause_damage(attacker, defender, damage);
			
			return;
		}
	}
	
	// 800x6
	if( sky_3rd > 0 )
	{
		if( !random(to_int(100. / calculate_effect(sky_3rd))) )
		{
			msg("\n$ME雙手一揮，大量「"WHT"烏黑碎石"NOR"」自地下竄出附著在$YOU身上，$YOU臉色瞬間發青，"HIG"有毒！！\n\n"NOR, attacker, defender, 1);
			defender->start_condition(MAGNETIC_STONE, 6, 1, attacker);
	
			return;
		}
	}
}

string query_description()
{
	string description = "";

	int ancient_blade_1st = query("ancient_blade_secret_art_1st");
	int ancient_blade_2nd = query("ancient_blade_secret_art_2nd");
	int sky_3rd = query("sky_3rd_secret_art");

	if( ancient_blade_1st > 0 )
		description += sprintf("  %-20s - 平均傷害 "HIY"%d"NOR, HIM"遠古"NOR MAG"刀法"NOR WHT"．"HIW"第一式"NOR, to_int(300 * calculate_effect(ancient_blade_1st)))+"\n";
	
	if( ancient_blade_2nd > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIM"遠古"NOR MAG"刀法"NOR WHT"．"HIW"第二式"NOR, 100./(50./calculate_effect(ancient_blade_2nd)))+"\n";
	
	if( sky_3rd > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIW"天魁"NOR WHT"特攻"NOR, 100./(100./calculate_effect(sky_3rd)))+"\n";
		
	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "把");
	set("mass", 1000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "blade");
	
	delete("buff");
	
	set(ATTACK, 5);
	set(BUFF_STR, 5);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);

	setup_inlay();
}
