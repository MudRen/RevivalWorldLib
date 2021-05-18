/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_invincibility.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : 無敵藥丸
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
	set_idname("invincibility pill", "無敵藥丸");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒可以用來抵抗所有攻擊的藥丸(10 秒鐘)。");
	set("unit", "粒");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
	set("coldtime", 20);
}

int do_eat(object me)
{
	msg("$ME吞下了$YOU，身旁籠罩著漫漫"HIG"綠光"NOR"，所有攻擊都無法侵入。\n", me, this_object(), 1);

	me->start_condition(INVINCIBILITY);

	return 1;
}
