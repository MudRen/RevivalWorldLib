/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lostland_key_3.c
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
	set_idname(NOR MAG"lostland"HIM" key 3rd"NOR, NOR MAG"消失的文明"HIM"鑰匙"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("disassemble", ([ 
		"/obj/gem/gem3" : 1,
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
		"/obj/gem/gem6" : 1,
	]));
	
	set("long", "一把通往消失的文明的鑰匙。");
	set("unit", "把");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
