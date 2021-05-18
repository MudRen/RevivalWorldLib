/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 天道
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

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIC"s"NOR CYN"k"HIC"y"NOR, HIC"天"NOR CYN"．"HIC"道"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "立天之道曰陰與陽。\n");
	set("unit", "株");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 145);
	set(BUFF_SOCIAL_EXP_BONUS, 100);
	set(BUFF_COMBAT_EXP_BONUS, 100);
	set(BUFF_GROW_ABILITY, 2);
	
	set("buff/status", HIC"天"NOR CYN"．"HIC"道"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"胸"NOR GRN"針"NOR);
}
