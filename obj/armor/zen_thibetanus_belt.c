/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_thibetanus_belt.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ÁI¡D¶Âºµ¸y±a
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
	set_idname(HIW"zen "NOR GRN"thibetanus"HIG" belt"NOR, HIW"ÁI"NOR WHT"¡D"NOR GRN"¶Âºµ"HIG"¸y±a"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q»OÆW¶Âºµ¤W±¼¸¨ªº¥v¸Ö¸y±a¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem6" : 6 ]));

	set(DEFEND, 6);
	set(BUFF_STR, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 3);

	::setup_equipment(EQ_BELT, HIY"¸y"NOR YEL"±a"NOR);
}

