/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : letter.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-08-18
 * Note	  : �p�C�̩Ҽ��g���H
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname("letter", "�p�C�̩Ҽ��g���H");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�p�C�̼g���@��ѹD�h���H�A�H�ʸ̭��٩�۩Ҧ������Ӫ����j�Z���H���C");
	set("unit", "��");
	set("value", 100);
	set("mass", 10);
}
