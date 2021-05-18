/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ²Ê»s»L¥Ò
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
	set_idname("coarse leggings", "²Ê»s»L¥Ò");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æµ}¦³ªº²Ê»s»L¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 1 ]));

	set(DEFEND, 1);
	set(BUFF_STR, 1);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);

	::setup_equipment(EQ_LEGGINGS, HIY"»L"NOR YEL"¥Ò"NOR);
}

