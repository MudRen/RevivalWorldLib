/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chi_hydra_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-10
 * Note   : ·¥¡D¤ýªÌ»L¥Ò
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
	set_idname(HIW"chi "NOR HIY"lord"NOR YEL" leggings"NOR, HIW"·¥"NOR WHT"¡D"NOR HIY"¤ý"NOR YEL"ªÌ"HIY"»L"NOR YEL"¥Ò"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ¥j¥N¤å¦r¤¤°O¸üªº¥v¸Ö»L¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 15 ]));

	set(DEFEND, 24);
	set(BUFF_STR, 3);
	set(BUFF_PHY, 3);
	set(BUFF_AGI, 3);
	set(BUFF_INT, 3);
	set(BUFF_CHA, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 3);
	set(BUFF_FULLSUIT+BUFF_PHY, 3);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);
	set(BUFF_FULLSUIT+BUFF_INT, 3);
	set(BUFF_FULLSUIT+BUFF_CHA, 3);
	set(BUFF_FULLSUIT+BUFF_COUNTERATTACK_CHANCE, 1);

	::setup_equipment(EQ_LEGGINGS, HIY"»L"NOR YEL"¥Ò"NOR);
}

