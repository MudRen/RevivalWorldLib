/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_nessiteras_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ÁI¡D¥§´µÅ@µÃ
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
	set_idname(HIW"zen "NOR HIB"nessiteras"NOR MAG" bracers"NOR, HIW"ÁI"NOR WHT"¡D"NOR HIB"¥§´µ"NOR MAG"Å@µÃ"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¥§´µ´ò¤ô©Ç¤W±¼¸¨ªº¥v¸ÖÅ@µÃ¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 12 ]));

	set(DEFEND, 15);
	set(BUFF_STR, 3);
	set(BUFF_INT, 3);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_ENERGY_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_BRACERS, HIY"Å@"NOR YEL"µÃ"NOR);
}

