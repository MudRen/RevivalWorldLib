/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_deadly_contract.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-06
 * Note   : 失落的狂暴藥丸
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
	set_idname(HIW"lost "NOR WHT"berserk pill"NOR, HIW"失落的"NOR WHT"狂暴藥丸"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒傳說中的狂暴藥丸，可以長時間連續擊出致命一擊，在這世界上非常的稀有。");
	set("unit", "粒");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
}

int do_eat(object me)
{
	msg("$ME吞下了$YOU，進入了驚人的狂暴狀態。\n", me, this_object(), 1);

	me->start_condition(DEADLY_CONTRACT);

	return 1;
}
