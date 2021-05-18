/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_charisma.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : �y�O�ĤY
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
	set_idname("charisma pill", "�y�O�ĤY");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�@�ɥi�H�Ψӵu�ɶ��z�o�X�j�j�y�O���ĤY(1 ����)�C");
	set("unit", "��");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
}

int do_eat(object me)
{
	msg("$ME�]�U�F$YOU�A�P������y�O�L���R�K�C\n", me, this_object(), 1);

	me->start_condition(POWER_CHARISMA);

	return 1;
}
