/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : thibetanus_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ¶Âºµ¯Ý¥Ò
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
	set_idname(GRN"thibetanus "HIG"breast plate"NOR, GRN"¶Âºµ"HIG"¯Ý¥Ò"NOR);
	set_temp("status", HIG"¶Ç"NOR GRN"©_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q»OÆW¶Âºµ¤W±¼¸¨ªº¯Ý¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 2,
		"/obj/gem/gem1" : 2,
		"/obj/gem/gem2" : 2,
		"/obj/gem/gem3" : 2,
		"/obj/gem/gem4" : 2,
		"/obj/gem/gem5" : 2,
		"/obj/gem/gem6" : 2,
	]));

	set(DEFEND, 5);
	set(BUFF_STR, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_BREASTPLATE, HIY"¯Ý"NOR YEL"¥Ò"NOR);
}

