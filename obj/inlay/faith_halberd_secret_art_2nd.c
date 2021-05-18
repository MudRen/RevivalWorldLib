/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : faith_halberd_secret_art_2nd.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : 信念之戟．第二式
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIG"faith"NOR GRN" halberd "NOR WHT"secret art "HIW"2nd"NOR, HIG"信念"NOR GRN"之戟"NOR WHT"．"HIW"第二式"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "從信念之戟上拆解出來的戟法秘笈。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
