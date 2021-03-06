/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : sky_2nd_secret_art.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-30
 * Note	  : 天劍特攻
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"sky 2nd "NOR WHT"secret art"NOR, HIW"天劍"NOR WHT"特攻"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從天劍身上取得的特攻秘技。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
