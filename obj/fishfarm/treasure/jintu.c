/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : jintu.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 淨土
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
	set_idname(HIY"jin"NOR YEL"tu", HIY"淨"NOR YEL"土"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "如來在然燈佛所，於法實無所得。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 1000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 115);
	set(BUFF_ENERGY_REGEN, 30);
	set("buff/status", HIY"淨"NOR YEL"土"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
