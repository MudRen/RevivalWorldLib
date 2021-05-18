/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : namay.c
 * Author : Clode@RevivalWorld
 * Date   : 2013-03-04
 * Note   : 伊邪那美
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
	set_idname(HIM"n"NOR MAG"a"HIM"m"NOR MAG"ay", HIM"伊"NOR MAG"邪"HIM"那"NOR MAG"美"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "立地之道曰柔與剛。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 180);
	set(BUFF_ENERGY_REGEN, 50);
	set(BUFF_SALE_ABILITY, 3);

	set("buff/status", HIM"伊"NOR MAG"邪"HIM"那"NOR MAG"美"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
