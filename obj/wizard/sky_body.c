/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_body.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-20
 * Note   : 斷天甲
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
	set_idname("sky body", HIW"斷"NOR WHT"天"HIW"甲"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "斷天甲。\n");
	set("unit", "把");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(DEFEND, 100);
	set(BUFF_STR, 5);
	set(BUFF_STAMINA_REGEN, 5);

	set("buff/status", HIW"斷"NOR WHT"天"HIW"甲"NOR);
	
	::setup_equipment(EQ_BREASTPLATE, HIY"胸"NOR YEL"甲"NOR);
}

//
// 防具自動特殊防禦
//
void special_defend(object attacker, object defender)
{
	int damage = range_random(10,20);

	if( !random(2) )
	{
		msg("$ME被$YOU身上的"+this_object()->query_name()+"尖銳的鋒刺劃了一下，造成輕微的傷害。\n", attacker, defender, 1);
		COMBAT_D->cause_damage(attacker, defender, damage);
	}
}