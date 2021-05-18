/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_extreme_silence.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-06
 * Note   : 失落的謐靜藥丸
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
	set_idname(HIW"lost "NOR WHT"silence pill"NOR, HIW"失落的"NOR WHT"謐靜藥丸"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒傳說中的謐靜藥丸，可以長時間減少休息所需要時間，在這世界上非常的稀有。");
	set("unit", "粒");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
}

int do_eat(object me)
{
	msg("$ME吞下了$YOU，心靈進入絕對的安寧。\n", me, this_object(), 1);

	me->start_condition(EXTREME_SILENCE);

	return 1;
}
