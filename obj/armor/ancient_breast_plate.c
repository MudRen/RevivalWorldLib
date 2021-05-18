/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ancient_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 遠古胸甲
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
	set_idname(HIM"ancient"NOR MAG"breast plate"NOR, HIM"遠古"NOR MAG"胸甲"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從被遺忘的遠古武士上掉落的胸甲。\n");
	set("unit", "副");
	set("mass", 1000);
	set("value", 10000);
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
	]));

	set(DEFEND, 12);
	set(BUFF_STR, 2);
	set(BUFF_PHY, 2);
	set(BUFF_AGI, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_INT, 1);

	::setup_equipment(EQ_BREASTPLATE, HIY"胸"NOR YEL"甲"NOR);
}

