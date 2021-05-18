/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : raw_milk.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : �ͨ�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("raw milk","�ͨ�");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�����X�Ӫ��ͨšA�L�k���ΡC\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 4000);
}
