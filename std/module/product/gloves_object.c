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

	set("unit", "¥ó");
	set("mass", 500);
	set("value", query_value());
	set("flag/no_amount", 1);

	delete("buff");

	set(DEFEND, 5);
	set(BUFF_HEALTH_MAX, 10);

	::setup_equipment(EQ_GLOVES, HIY"¤â"NOR YEL"®M"NOR);

	setup_inlay();
}
