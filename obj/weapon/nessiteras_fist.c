/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_fist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 尼斯拳套
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
	set_idname(HIB"nessiteras"NOR MAG" fist"NOR, HIB"尼斯"NOR MAG"拳套"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個從尼斯湖水怪上掉落的特殊拳套。\n");
	set("unit", "個");
	set("mass", 1000);
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
		"/obj/inlay/nessiteras_fist_secret_art_1st" : 1,
		"/obj/inlay/nessiteras_fist_secret_art_2nd" : 1,
	]));

	set("skill", "unarmed");

	set(SPEED, 20);
	set(BUFF_AGI, 6);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU出拳攻擊，";
}

//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
	// BOSS 不能被 stun
	if( !random(50) )
	{
		int time;
		int damage = range_random(400, 600);

		if( userp(defender) && random(5) )
			return;

		msg("$ME使出「"HIY"破天"NOR YEL"擊"NOR"」，"+this_object()->query_idname()+"直接命中$YOU的頭部！\n", attacker, defender, 1);

		if( !defender->is_boss() )
		{
			if( userp(defender) )
				time = 1;
			else
				time = 2;

			defender->start_delay("nessiteras_fist", time, pnoun(2, attacker)+"尚未從「"HIY"破天"NOR YEL"擊"NOR"」中恢復。\n", pnoun(2, attacker)+"從「"HIY"破天"NOR YEL"擊"NOR"」中恢復了。\n");
		}
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}

	if( !random(15) )
	{
		int combo = range_random(4, 16);
		int damage = range_random(25, 35)*combo;
		
		if( userp(defender) && random(3) )
			return;

		for(int i=0;i<combo;i++)
			msg("$ME使出狂暴連擊「"HIB"狂"NOR BLU"嵐"NOR"」，連續往$YOU進行全身攻擊！("YEL"COMBO "HIY+(i+1)+NOR")\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
