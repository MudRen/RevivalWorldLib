/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bagua.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : ¤K¨ö
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
	set_idname(HIM"ba"NOR MAG"gua", HIM"¤K"NOR MAG"¨ö"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "°®¡B¾_¡B¦á¡B§I¡B´S¡B©[¡BÂ÷¡B§¢¡C\n");
	set("unit", "Áû");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 130);
	set(BUFF_ENERGY_REGEN, 35);
	set("buff/status", HIM"¤K"NOR MAG"¨ö"NOR);
	
	::setup_equipment(EQ_EYE, HIG"²´"NOR GRN"·ú"NOR);
}
