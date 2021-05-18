/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : monster_gall.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-25
 * Note	  : 神獸石膽
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR YEL"monster "HIY"gall"NOR, HIW"古"NOR WHT"物．"NOR YEL"神獸"HIY"石膽"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一塊斷層深處取出的一個膽囊化石，似乎是製作武器的好物品。");
	set("unit", "塊");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
