/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_ancient_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 罥环臔得
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
	set_idname(HIW"zen "NOR HIM"ancient"NOR MAG" bracers"NOR, HIW"罥"NOR WHT""NOR HIM"环"NOR MAG"臔得"NOR);
	set_temp("status", HIW""NOR WHT"钢");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "捌眖砆框а环猌奔辅钢臔得\n");
	set("unit", "捌");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 12 ]));

	set(DEFEND, 15);
	set(BUFF_STR, 3);
	set(BUFF_PHY, 3);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_INT, 3);

	::setup_equipment(EQ_BRACERS, HIY"臔"NOR YEL"得"NOR);
}

