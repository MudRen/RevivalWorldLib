/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
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

inherit STANDARD_MODULE_PRODUCT;
inherit EQUIPMENT;

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "¥u");
	set("mass", 100);
	set("value", query_value());
	set("flag/no_amount", 1);

	delete("buff");

	set(BUFF_STAMINA_REGEN, 1);
	set(BUFF_HEALTH_REGEN, 1);
	set(BUFF_ENERGY_REGEN, 1);

	::setup_equipment(EQ_FINGER, HIG"§Ù"NOR GRN"«ü"NOR);

	setup_inlay();
}
