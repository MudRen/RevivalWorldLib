/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ¥§´µ»L¥Ò
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
	set_idname(HIB"nessiteras"NOR MAG" leggings"NOR, HIB"¥§´µ"NOR MAG"»L¥Ò"NOR);
	set_temp("status", HIG"¶Ç"NOR GRN"©_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¥§´µ´ò¤ô©Ç¤W±¼¸¨ªº»L¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 4 ]));

	set(DEFEND, 6);
	set(BUFF_STR, 1);
	set(BUFF_INT, 1);
	set(BUFF_AGI, 1);

	set(BUFF_FULLSUIT+BUFF_ENERGY_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_LEGGINGS, HIY"»L"NOR YEL"¥Ò"NOR);
}

