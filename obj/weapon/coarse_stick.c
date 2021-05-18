/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_stick.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-12
 * Note   : 粗製木棍
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
	set_idname("coarse stick", "粗製木棍");
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個稀有的粗製木棍。\n");
	set("unit", "個");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 1,
		"/obj/gem/gem1" : 1,
	]));

	set("skill", "stick");
	set(DEFEND, 2);
	set(BUFF_PHY, 3);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU揮砍攻擊，";
}

//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
	if( !random(15) )
	{
		object* targets = COMBAT_D->query_fighting_targets(attacker);
		int damage = to_int(range_random(40, 80) * pow(to_float(sizeof(targets)), 0.8) / sizeof(targets));
		
		foreach(object ob in targets)
		{
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			msg("$ME使出大範圍橫掃揮擊，對$YOU造成傷害！\n", attacker, defender, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}
