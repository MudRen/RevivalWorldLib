/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_purification.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : 淨化藥丸
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
	set_idname("purification pill", "淨化藥丸");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一粒可以用來淨化身上汙穢的藥丸，並恢復部份體力、生命、精神值。");
	set("unit", "粒");
	set("capacity", 100);
	set("mass", 10);
	set("value", 5000000);
	set("badsell", 1);
}

int do_eat(object me)
{	
	me->earn_stamina(250);
	me->earn_health(250);
	me->earn_energy(250);
	
	msg("$ME吞下了$YOU，身上散發出一陣"HIW"白光"NOR"，將身上的汙穢淨化，並恢復部份體力、生命、精神值。。\n", me, this_object(), 1);

	if( me->in_condition(DIE) )
		me->stop_condition(DIE);

	return 1;
}
