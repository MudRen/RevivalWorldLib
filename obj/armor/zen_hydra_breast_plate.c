/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_hydra_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ÁI¡D®ü¼w©Ô¯Ý¥Ò
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
	set_idname(HIW"zen "NOR HIREV HIW"hydra"WHT" breast plate"NOR, HIW"ÁI"NOR WHT"¡D"NOR HIREV HIW"®ü¼w©Ô"WHT"¯Ý¥Ò"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@°Æ±q¤EÀY³D®ü¼w©Ô¤W±¼¸¨ªº¥v¸Ö¯Ý¥Ò¡C\n");
	set("unit", "°Æ");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 12,
		"/obj/gem/gem1" : 12,
		"/obj/gem/gem2" : 12,
		"/obj/gem/gem3" : 12,
		"/obj/gem/gem4" : 12,
		"/obj/gem/gem5" : 12,
		"/obj/gem/gem6" : 12,
	]));

	set(DEFEND, 36);
	set(BUFF_PHY, 6);
	set(BUFF_INT, 6);
	set(BUFF_STR, 6);

	set(BUFF_FULLSUIT+BUFF_STAMINA_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_STR, 3);

	::setup_equipment(EQ_BREASTPLATE, HIY"¯Ý"NOR YEL"¥Ò"NOR);
}

