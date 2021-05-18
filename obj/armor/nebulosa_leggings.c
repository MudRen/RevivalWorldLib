/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nebulosa_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ¶³°\»L¥Ò
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
	set_idname(HIY"nebulosa"NOR YEL" leggings"NOR, HIY"¶³°\"NOR YEL"»L¥Ò"NOR);
	set_temp("status", HIG"¶Ç"NOR GRN"©_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¶³°\¤W±¼¸¨ªº»L¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 3 ]));

	set(DEFEND, 4);
	set(BUFF_STR, 3);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_LEGGINGS, HIY"»L"NOR YEL"¥Ò"NOR);
}

