/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_nebulosa_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ÁI¡D¶³°\Å@µÃ
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
	set_idname(HIW"zen "NOR HIY"nebulosa"NOR YEL" bracers"NOR, HIW"ÁI"NOR WHT"¡D"NOR HIY"¶³°\"NOR YEL"Å@µÃ"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¶³°\¤W±¼¸¨ªº¥v¸ÖÅ@µÃ¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 9 ]));

	set(DEFEND, 9);
	set(BUFF_STR, 6);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 30);
	set(BUFF_FULLSUIT+BUFF_STR, 3);

	::setup_equipment(EQ_BRACERS, HIY"Å@"NOR YEL"µÃ"NOR);
}

