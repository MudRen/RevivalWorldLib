/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : eagle_feather.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-01-16
 * Note	  : 疾鷹羽翼
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIC"eagle"NOR CYN" feather"NOR, HIW"古"NOR WHT"物．"HIC"疾鷹"NOR CYN"羽翼"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一隻擅長於疾風中飛行的老鷹身上取下的羽翼，似乎是製作裝備的好物品。");
	set("unit", "支");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
