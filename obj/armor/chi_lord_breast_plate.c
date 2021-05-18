/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chi_hydra_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-10
 * Note   : ·¥¡D¤ýªÌ¯Ý¥Ò
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
	set_idname(HIW"chi "NOR HIY"lord"NOR YEL" breast plate"NOR, HIW"·¥"NOR WHT"¡D"NOR HIY"¤ý"NOR YEL"ªÌ"HIY"¯Ý"NOR YEL"¥Ò"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ¥j¥N¤å¦r¤¤°O¸üªº¥v¸Ö¯Ý¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 15,
		"/obj/gem/gem1" : 15,
		"/obj/gem/gem2" : 15,
		"/obj/gem/gem3" : 15,
		"/obj/gem/gem4" : 15,
		"/obj/gem/gem5" : 15,
		"/obj/gem/gem6" : 15,
	]));

	set(DEFEND, 45);
	set(BUFF_STR, 6);
	set(BUFF_PHY, 6);
	set(BUFF_AGI, 6);
	set(BUFF_INT, 6);
	set(BUFF_CHA, 6);

	set(BUFF_FULLSUIT+BUFF_STR, 3);
	set(BUFF_FULLSUIT+BUFF_PHY, 3);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);
	set(BUFF_FULLSUIT+BUFF_INT, 3);
	set(BUFF_FULLSUIT+BUFF_CHA, 3);
	set(BUFF_FULLSUIT+BUFF_ANTIFATAL_CHANCE, 1);
	
	::setup_equipment(EQ_BREASTPLATE, HIY"¯Ý"NOR YEL"¥Ò"NOR);
}

