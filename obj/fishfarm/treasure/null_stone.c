/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : null_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 無虛石
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
	set_idname(HIR"null "NOR RED"stone"NOR, HIR"無虛"NOR RED"石"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一顆充滿稜角的暗紅色石塊，若不小心拿取則可能因此劃破皮肉，石頭四週似乎環\n繞著陰寒的魅影，持續散發出冰冷的氣息。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 12000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 45);
	set(BUFF_ENERGY_REGEN, 5);
	set("buff/status", HIR"無虛"NOR RED"石"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
