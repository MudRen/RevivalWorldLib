/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : thibetanus_gloves.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 黑熊手套
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
	set_idname(GRN"thibetanus"HIG" gloves"NOR, GRN"黑熊"HIG"手套"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從臺灣黑熊上掉落的手套。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem2" : 2 ]));

	set(DEFEND, 2);
	set(BUFF_STR, 1);

	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_GLOVES, HIY"手"NOR YEL"套"NOR);
}

