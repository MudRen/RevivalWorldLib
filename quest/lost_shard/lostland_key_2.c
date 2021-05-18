/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lostland_key_2.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-08-18
 * Note	  : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR MAG"lostland"HIM" key 2nd"NOR, NOR MAG"遠古海島"HIM"鑰匙"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("disassemble", ([ 
		"/obj/gem/gem0" : 1,
		"/obj/gem/gem1" : 1,
		"/obj/gem/gem2" : 1,
	]));
	
	set("long", "一把通往遠古海島的鑰匙。");
	set("unit", "把");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
