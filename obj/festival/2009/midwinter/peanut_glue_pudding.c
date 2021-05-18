/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : peanut_glue_pudding.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-22
 * Note   : ��ʹ���
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
	set_idname(YEL"peanut "HIW"glue pudding"NOR, YEL"���"HIW"����"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�V���������~�A�i�u�ɶ����ɭӤH�ӱ��P�ͩR�C");
	set("unit", "��");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100);
	set("badsell", 1);
	set("special", 1);
}

int do_eat(object me)
{
	msg("$ME�]�U�F$YOU�A�@�ѷx�N�q�餺�����ӥX�C\n", me, this_object(), 1);

	me->start_condition(PEANUT_GLUE_PUDDING);

	return 1;
}
