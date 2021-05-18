/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lava_meteor.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-24
 * Note	  : 熔岩隕石
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR RED"lava "HIR"meteor"NOR, HIW"古"NOR WHT"物．"NOR RED"熔岩"HIR"隕石"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一塊斷層深處取出的隕石，隕石內部似乎還有灼熱熔岩在流動，似乎是製作武器的好物品。");
	set("unit", "塊");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
