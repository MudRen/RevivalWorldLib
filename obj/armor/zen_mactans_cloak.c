/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_mactans_cloak.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ÁI¡D¹è°ü©Ü­·
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
	set_idname(HIW"zen "NOR WHT"mactans "NOR HIW"cloak"NOR, HIW"ÁI"NOR WHT"¡D"NOR WHT"¹è°ü"NOR HIW"©Ü­·"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¶Â¹è°ü¤W±¼¸¨ªº¥v¸Ö©Ü­·¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem3" : 6 ]));

	set(DEFEND, 6);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_CLOAK, HIY"©Ü"NOR YEL"­·"NOR);
}

