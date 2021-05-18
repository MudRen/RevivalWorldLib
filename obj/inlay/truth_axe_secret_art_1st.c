/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : truth_axe_secret_art_1st.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : 真理之斧．第一式
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"truth"NOR WHT" axe "NOR WHT"secret art "HIW"1st"NOR, HIW"真理"NOR WHT"之斧"NOR WHT"．"HIW"第一式"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從真理之斧上拆解出來的斧法秘笈。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
