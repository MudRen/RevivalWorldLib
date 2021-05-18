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
#include <use.h>
#include <condition.h>

inherit STANDARD_MODULE_PRODUCT;

string query_description()
{
	return "¡B®gµ{ "+query("distance")+" ®æ¡B¶Ë®`½d³ò¥b®| "+query("range");
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "­Ó");
	set("mass", 1000);
	set("value", query_value());
	set("distance", 2);
	set("range", 1);

	setup_inlay();
}
