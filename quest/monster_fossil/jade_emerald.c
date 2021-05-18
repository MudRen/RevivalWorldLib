/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : jade_emerald.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-25
 * Note	  : 白玉翡翠
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"jade "HIG"emerald"NOR, HIW"古"NOR WHT"物．"HIW"白玉"HIG"翡翠"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一塊斷層深處取出的一個翡翠，似乎是製作防具的好物品。");
	set("unit", "塊");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
