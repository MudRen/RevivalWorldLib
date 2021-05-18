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

#include <inherit.h>

inherit STANDARD_MODULE_PRODUCT;

string query_description()
{
	return "¡B«ì´_®ÄªG "+query("effect");
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "±i");
	set("mass", 60000);
	set("value", query_value());
	set("flag/no_amount", 1);
	set("effect", 90);

	setup_inlay();
}
