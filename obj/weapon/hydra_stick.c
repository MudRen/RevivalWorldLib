/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra_stick.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 海德拉石棍
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
	set_idname(HIW"hydra"NOR WHT" stick"NOR, HIW"海德拉"NOR WHT"石棍"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一支從九頭蛇海德拉上掉落的巨大石棍。\n");
	set("unit", "個");
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
		int damage = to_int(range_random(100, 500) * pow(to_float(sizeof(targets)), 0.8) / sizeof(targets));

		foreach(object ob in targets)
		{
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			msg("$ME一手揚起石棍，棍影倏地化身為「"HIW"九截"NOR WHT"灰影"NOR"」自各個方向往$YOU擊去，造成嚴重傷害！\n", attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}
