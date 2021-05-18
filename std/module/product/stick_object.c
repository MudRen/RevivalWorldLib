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
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU橫劈攻擊，";
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
// 武器自動特殊攻擊
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

			msg("$ME使出大範圍橫掃揮擊，對$YOU造成嚴重傷害！\n", attacker, defender, 1);
			
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
	
				msg("$ME一手揚起石棍，棍影倏地化身為「"HIW"九截"NOR WHT"灰影"NOR"」自各個方向往$YOU擊去，造成嚴重傷害！\n", attacker, ob, 1);
				
				COMBAT_D->cause_damage(attacker, ob, damage);
			}
			
			return;
		}
	}

	if( hydra_stick_2nd > 0 )
	{
		if( !random(to_int(100. / calculate_effect(hydra_stick_2nd))) )
		{
			msg("$ME發動「"HIG"守護"NOR GRN"之道"NOR"」，短時間內提升角色 "HIG"10%"NOR" 防暴擊能力！\n", attacker, defender, 1);
			
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
			
			msg("\n$ME低聲悶雷呼喊：「"HIY"去吧！"NOR"」，一塊巨石從$ME身上落下，幻化成$YOU並加入戰鬥。\n\n", attacker, small_statue, 1);
			
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
		description += sprintf("  %-20s - 平均傷害 "HIY"%d"NOR, HIW"海德拉"NOR WHT"棍法"NOR WHT"．"HIW"第一式"NOR, to_int(300 * calculate_effect(hydra_stick_1st)))+"\n";
	
	if( hydra_stick_2nd > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIW"海德拉"NOR WHT"棍法"NOR WHT"．"HIW"第二式"NOR, 100./(100./calculate_effect(hydra_stick_2nd)))+"\n";

	if( sky_2nd > 0 )
		description += sprintf("  %-20s - 發動機率 "HIY"%.2f%%"NOR, HIW"天劍"NOR WHT"特攻"NOR, 100./(100./calculate_effect(sky_2nd)))+"\n";

	if( description != "" )
		description = "\n\n" + description +"\n";
		
	return description;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "根");
	set("mass", 2000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "stick");

	delete("buff");

	set(DEFEND, 5);
	set(BUFF_PHY, 5);

	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);

	setup_inlay();
}
