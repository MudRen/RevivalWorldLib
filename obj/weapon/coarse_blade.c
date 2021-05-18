/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-12
 * Note   : 粗製木刀
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

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("coarse blade", "粗製木刀");
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個稀有的粗製木刀。\n");
	set("unit", "個");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ 
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
		"/obj/gem/gem6" : 1,
	]));

	set("skill", "blade");
	set(ATTACK, 2);
	set(BUFF_STR, 3);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME手持著"+this_object()->query_name()+"往$YOU揮砍攻擊，";
}

//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
	if( !random(15) )
	{
		int damage = range_random(30, 90);

		msg("$ME使出致命揮砍，對$YOU造成傷害！\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
