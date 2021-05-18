/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 粗製拳套
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
	set_idname("coarse fist", "粗製拳套");
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個稀有的粗製拳套。\n");
	set("unit", "個");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ 
		"/obj/gem/gem2" : 1,
		"/obj/gem/gem3" : 1,
	]));

	set("skill", "unarmed");
	set(SPEED, 2);
	set(BUFF_AGI, 3);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU揮拳攻擊，";
}

//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
	if( !random(15) )
	{
		int combo = range_random(2, 4);
		int damage = range_random(15, 25)*combo;

		for(int i=0;i<combo;i++)
			msg("$ME使出狂暴連擊，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
			
		return;
	}
}
