/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : gem.c
 * Author :
 * Date	  : 2003-5-3
 * Note	  : Fish產物
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>

inherit __DIR__"_gem_object.c";

void create()
{
	set_idname( "diamond", HIW"鑽石"NOR );

	if( this_object()->set_shadow_ob() ) return;

	set("long", "第六元素 － 「靛」。");
	set("unit", "顆");
	set("value", 7777);
	set("mass", 77);
	set("gemcode", 6);
}