/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : deep_sea_thorn.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-24
 * Note	  : 深海刺棘
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR BLU"deep sea"HIB" thorn"NOR, HIW"古"NOR WHT"物．"NOR BLU"深海"HIB"刺棘"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "考古學家從一塊斷層深處取出的化石，佈滿堅硬卻又強韌的刺棘，似乎是製作裝備的好物品。");
	set("unit", "塊");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
