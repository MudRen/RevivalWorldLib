/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : ancient_blade_secret_art_1st.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : 遠古刀法．第一式
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIM"ancient "NOR MAG"blade "NOR WHT"secret art "HIW"1st"NOR, HIM"遠古"NOR MAG"刀法"NOR WHT"．"HIW"第一式"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從遠古神刀上拆解出來的刀法秘笈。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
