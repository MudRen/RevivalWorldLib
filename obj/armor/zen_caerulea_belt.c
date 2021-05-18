/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_caerulea_belt.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 禪．藍鵲腰帶
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
	set_idname(HIW"zen "NOR HIB"caerulea"NOR BLU" belt"NOR, HIW"禪"NOR WHT"．"NOR HIB"藍鵲"NOR BLU"腰帶"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從臺灣藍鵲上掉落的史詩腰帶。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem6" : 12 ]));

	set(DEFEND, 9);
	set(BUFF_AGI, 6);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 30);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_BELT, HIY"腰"NOR YEL"帶"NOR);
}

