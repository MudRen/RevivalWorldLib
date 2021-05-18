/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lord_sword_secret_art_2nd.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : 王者之劍．第二式
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIY"lord"NOR YEL" sword "NOR WHT"secret art "HIW"2nd"NOR, HIY"王者"NOR YEL"之劍"NOR WHT"．"HIW"第二式"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從極．王者之劍上拆解出來的劍法秘笈。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
