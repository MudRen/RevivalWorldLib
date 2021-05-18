/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : gem4.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-07-27
 * Note	  : �Ŧ⤸��
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
	set_idname("blue element", HIC"�Ŧ�"NOR CYN"����"NOR );

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q�S������������X�Ӫ��Ĥ����� �� �u"HIC"��"NOR"�v�C");
	set("unit", "��");
	set("value", 7777);
	set("mass", 7);
	set("gemcode", 5);
}