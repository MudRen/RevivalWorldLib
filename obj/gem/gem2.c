/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : gem2.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-07-27
 * Note	  : ���⤸��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

int is_gem_object()
{
	return 1;
}

void create()
{
	set_idname("yellow element", HIY"����"NOR YEL"����"NOR );

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q�S������������X�Ӫ��ĤT���� �� �u"HIY"��"NOR"�v�C");
	set("unit", "��");
	set("value", 7777);
	set("mass", 7);
	set("gemcode", 3);
}