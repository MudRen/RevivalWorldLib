/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : gem6.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-07-27
 * Note	  : 紫色元素
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

int is_gem_object()
{
	return 1;
}

void create()
{
	set_idname("purple element", HIM"紫色"NOR MAG"元素"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從特殊的水晶中粹取出來的第七元素 － 「"HIM"紫"NOR"」。");
	set("unit", "顆");
	set("value", 7777);
	set("mass", 7);
	set("gemcode", 7);
}