/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_agility.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : 敏捷藥丸
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
	set_idname("agility pill", "敏捷藥丸");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒可以用來短時間爆發出強大敏捷的藥丸(1 分鐘)。");
	set("unit", "粒");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
}

int do_eat(object me)
{
	msg("$ME吞下了$YOU，感到全身敏捷無限充沛。\n", me, this_object(), 1);

	me->start_condition(POWER_AGILITY);

	return 1;
}
