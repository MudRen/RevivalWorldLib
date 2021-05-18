/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lostland_key.c
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
	set_idname(NOR MAG"lostland"HIM" key 1st"NOR, NOR MAG"遠古刀塚"HIM"鑰匙"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("disassemble", ([ 
		"/obj/gem/gem0" : 1,
		"/obj/gem/gem1" : 1,
		"/obj/gem/gem2" : 1,
		"/obj/gem/gem3" : 1,
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
		"/obj/gem/gem6" : 1,
	]));
	
	set("long", "一把通往遠古刀塚的鑰匙，這把鑰匙是利用六個遠古武器碎片所組合起來的。");
	set("unit", "把");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
