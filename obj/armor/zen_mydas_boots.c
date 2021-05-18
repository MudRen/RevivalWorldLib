/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_mydas_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ÁI¡Dºñø¿¾c¤l
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
	set_idname(HIW"zen "NOR HIG"mydas "NOR GRN"boots"NOR, HIW"ÁI"NOR WHT"¡D"NOR HIG"ºñø¿"NOR GRN"¾c¤l"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±qºñø¿Àt¤W±¼¸¨ªº¥v¸Ö¾c¤l¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 6 ]));

	set(DEFEND, 6);
	set(BUFF_PHY, 3);

	set(BUFF_FULLSUIT+BUFF_PHY, 3);

	::setup_equipment(EQ_BOOTS, HIY"¾c"NOR YEL"¤l"NOR);
}

