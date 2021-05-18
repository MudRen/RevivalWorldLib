/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gigas_gloves.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 赤蠵手套
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
	set_idname(HIR"gigas "NOR RED"gloves"NOR, HIR"赤蠵"NOR RED"手套"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從赤蠵龜上掉落的手套。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem2" : 3 ]));

	set(DEFEND, 3);
	set(BUFF_PHY, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_PHY, 1);

	::setup_equipment(EQ_GLOVES, HIY"手"NOR YEL"套"NOR);
}

