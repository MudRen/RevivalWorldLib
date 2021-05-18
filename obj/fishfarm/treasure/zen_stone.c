/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-05-19
 * Note   : Ã¢ÁI¥Û
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

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIM"zen "NOR MAG"stone"NOR, HIM"Ã¢"NOR MAG"ÁI"HIM"¥Û"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¤@­Ó·¥¬°µ}¦³ªº»·¥j¥ÛÀY¡C\n");
	set("unit", "Áû");
	set("mass", 0);
	set("value", 200000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 85);
	set(BUFF_ENERGY_REGEN, 15);
	set("buff/status", HIM"Ã¢"NOR MAG"ÁI"HIM"¥Û"NOR);
	
	::setup_equipment(EQ_EYE, HIG"²´"NOR GRN"·ú"NOR);
}
