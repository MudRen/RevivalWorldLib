/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_sword.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-20
 * Note   : 斷天劍
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
	set_idname("sky sword", HIW"斷"NOR WHT"天"HIW"劍"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一把可以漂浮在空中的巨劍。\n");
	set("unit", "把");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 5);
	set(BUFF_STAMINA_REGEN, 5);
	set("buff/status", HIW"斷"NOR WHT"天"HIW"劍"NOR);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}


//
// 武器自動特殊攻擊
//
void special_attack(object attacker, object defender)
{
	int damage = range_random(80,100);

	if( !random(5) )
	{
		msg("$ME手上的"+this_object()->query_idname()+"突然發出一道白光射向天際並垂直下劈，造成$YOU的嚴重砍傷。\n", attacker, defender, 1);
		COMBAT_D->cause_damage(attacker, defender, damage);
	}
}
