/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : eye_claw.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-01-16
 * Note	  : 百眼龍爪
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIM"eye"NOR MAG" claw"NOR, HIW"古"NOR WHT"物．"HIM"百眼"NOR MAG"龍爪"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一隻傳說中擁有上百個眼睛的巨龍身上取得的龍爪，似乎是製作裝備的好物品。");
	set("unit", "支");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
