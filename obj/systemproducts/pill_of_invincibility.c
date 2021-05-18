/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pill_of_invincibility.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : �L���ĤY
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
	set_idname("invincibility pill", "�L���ĤY");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�@�ɥi�H�Ψө�ܩҦ��������ĤY(10 ����)�C");
	set("unit", "��");
	set("capacity", 0);
	set("mass", 10);
	set("value", 100000000);
	set("badsell", 1);
	set("coldtime", 20);
}

int do_eat(object me)
{
	msg("$ME�]�U�F$YOU�A����Ţ�n�ۺ���"HIG"���"NOR"�A�Ҧ��������L�k�I�J�C\n", me, this_object(), 1);

	me->start_condition(INVINCIBILITY);

	return 1;
}
