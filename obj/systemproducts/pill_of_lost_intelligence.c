/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_lost_intelligence.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-06
 * Note   : 失落的智商藥丸
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <feature.h>

inherit STANDARD_OBJECT;
inherit FOOD;

void create()
{
	set_idname(HIW"lost "NOR WHT"intelligence pill"NOR, HIW"失落的"NOR WHT"智商藥丸"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒傳說中的智商藥丸，可以長時間爆發出強大的智商，在這世界上非常的稀有。");
	set("unit", "粒");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
}

int do_eat(object me)
{
	msg("$ME吞下了$YOU，感到全身智商無限充沛。\n", me, this_object(), 1);

	me->start_condition(LOST_INTELLIGENCE);

	return 1;
}
