/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ancient_blade.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 遠古神刀
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
	set_idname(HIM"ancient"NOR MAG" blade"NOR, HIM"遠古"NOR MAG"神刀"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個從被遺忘的遠古武士上掉落的神刀。\n");
	set("unit", "把");
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
		"/obj/inlay/ancient_blade_secret_art_1st" : 1,
		"/obj/inlay/ancient_blade_secret_art_2nd" : 1,
	]));

	set("skill", "blade");

	set(ATTACK, 20);
	set(BUFF_STR, 6);
	
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
	// BOSS 不能被 stun
	if( !random(50) && !defender->is_boss() )
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

	if( !random(15) )
	{
		int damage = range_random(100, 500);

		if( userp(defender) && random(3) )
			return;
		
		msg("$ME身後突然出現「"HIW"遠古"NOR WHT"武士"NOR"」的身影，瞬間"+this_object()->query_idname()+"刀光四射，揮出兇猛攻擊，對$YOU造成嚴重傷害！\n", attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
