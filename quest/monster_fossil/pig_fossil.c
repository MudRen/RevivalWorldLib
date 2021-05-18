/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : pig_fossil.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-24
 * Note	  : 野豬化石
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIC"fossil "NOR CYN"pig"NOR, NOR CYN"野豬"HIC"化石"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "這塊化石隱隱發出藍光，似乎有種神秘的力量封印其中，或許那位世外仙女知道這是什麼。");
	set("unit", "塊");
	set("value", 100);
	set("mass", 100);
	set("flag/no_amount", 1);
}
