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
#include <buff.h>
#include <feature.h>
#include <equipment.h>

inherit STANDARD_MODULE_PRODUCT;
inherit EQUIPMENT;

string query_description()
{
	return "、河流與海洋地形快速穿越";
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "艘");
	set("mass", 120000);
	set("value", query_value());
	set("flag/no_amount", 1);

	delete("buff");

	set(BUFF_LOADING_MAX, 180);
	set(BUFF_SLOT_MAX, 10);
	set(BUFF_STAMINA_REGEN, 2);
	set(BUFF_HEALTH_REGEN, 2);
	set(BUFF_ENERGY_REGEN, 2);

	if( clonep(this_object()) )
        	set("buff/status", this_object()->query_name());

	::setup_equipment(EQ_BOAT, HIC"小"NOR CYN"船"NOR);

	setup_inlay();
}

