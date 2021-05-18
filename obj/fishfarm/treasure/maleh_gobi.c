/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : maleh_gobi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-05-04
 * Note   : 馬勒戈璧
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
	set_idname(HIR"maleh "NOR RED"gobi"NOR, HIR"馬勒"NOR RED"戈璧"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一個極為缺德的近代神化化石。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 500000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 100);
	set(BUFF_ENERGY_REGEN, 20);
	set("buff/status", HIR"馬勒"NOR RED"戈璧"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
