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
#include <feature.h>
#include <delay.h>

inherit STANDARD_MODULE_PRODUCT;

int remove()
{
	object env = environment();
	
	if( objectp(env) )
		env->remove_delay(FISHING_DELAY_KEY);
}

void leave(object ob)
{
	if( objectp(ob) )
		ob->remove_delay(FISHING_DELAY_KEY);
}

string query_description()
{
	return "¡B­@¤[«× "+query_temp("original_endurance")+"¡B³½¬ñµ¥¯Å "+query("rod_level");
}

varargs void create(string file, string type)
{
	::create(file, type);

	set_temp("endurance", 100000);
	set_temp("original_endurance", 100000);
	set("unit", "®Ú");
	set("mass", 100);
	set("value", query_value());
	set("rod_level", 40);

	setup_inlay();
}
