/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_purification.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : �b���ĤY
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
	set_idname("purification pill", "�b���ĤY");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�@�ɥi�H�ΨӲb�ƨ��W��©���ĤY�A�ë�_������O�B�ͩR�B�믫�ȡC");
	set("unit", "��");
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
	
	msg("$ME�]�U�F$YOU�A���W���o�X�@�}"HIW"�ե�"NOR"�A�N���W����©�b�ơA�ë�_������O�B�ͩR�B�믫�ȡC�C\n", me, this_object(), 1);

	if( me->in_condition(DIE) )
		me->stop_condition(DIE);

	return 1;
}
