/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mydas_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 綠蠵拳套
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
	set_idname(HIG"mydas"NOR GRN" fist"NOR, HIG"綠蠵"NOR GRN"拳套"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個從綠蠵龜上掉落的特殊拳套。\n");
	set("unit", "個");
	set("mass", 100);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 2,
		"/obj/gem/gem1" : 2,
		"/obj/gem/gem2" : 2,
		"/obj/gem/gem3" : 2,
		"/obj/gem/gem4" : 2,
		"/obj/gem/gem5" : 2,
		"/obj/gem/gem6" : 2,
	]));

	set("skill", "unarmed");

	set(ATTACK, 5);
	set(BUFF_STR, 1);
	set(BUFF_PHY, 3);
	
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
		int combo = range_random(4, 8);
		int damage = range_random(15, 25)*combo;

		for(int i=0;i<combo;i++)
			msg("$ME使出狂暴連擊，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
