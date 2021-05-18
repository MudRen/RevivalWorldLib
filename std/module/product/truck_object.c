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
	return "";
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "½ø");
	set("mass", 240000);
	set("value", query_value());
	set("flag/no_amount", 1);

	delete("buff");

	set(BUFF_LOADING_MAX, 600);
	set(BUFF_SLOT_MAX, 10);

	if( clonep(this_object()) )
        	set("buff/status", this_object()->query_name());

	::setup_equipment(EQ_VEHICLE, HIC"¥d"NOR CYN"¨®"NOR);

	setup_inlay();
}

