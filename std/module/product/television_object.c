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
	return "、效果持續時間 "+query("effect")+" 秒";
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "台");
	set("mass", 20000);
	set("value", query_value());
	set("flag/no_amount", 1);
	set("effect", 600);

	setup_inlay();
}
