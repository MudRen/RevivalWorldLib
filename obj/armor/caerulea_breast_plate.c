/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : caerulea_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ÂÅÄN¯Ý¥Ò
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
	set_idname(HIB"caerulea"NOR BLU" breast plate"NOR, HIB"ÂÅÄN"NOR BLU"¯Ý¥Ò"NOR);
	set_temp("status", HIG"¶Ç"NOR GRN"©_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q»OÆWÂÅÄN¤W±¼¸¨ªº¯Ý¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 3,
		"/obj/gem/gem1" : 3,
		"/obj/gem/gem2" : 3,
		"/obj/gem/gem3" : 3,
		"/obj/gem/gem4" : 3,
		"/obj/gem/gem5" : 3,
		"/obj/gem/gem6" : 3,
	]));

	set(DEFEND, 8);
	set(BUFF_AGI, 5);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_BREASTPLATE, HIY"¯Ý"NOR YEL"¥Ò"NOR);
}

