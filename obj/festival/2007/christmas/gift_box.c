/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gift_box.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : §��
 * Update :
 *  o 2003-00-00  


 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <location.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

void destruct_self()
{
	object me = this_player();
	set_temp("christmas", 0, me); // �B�J�k0
	tell( me,HIR"���ȥ��ѡA�Э��s�ӹL�C\n"NOR);
	destruct(this_object());
}

void create()
{
	set_idname("gift box", HIR"§"NOR+RED"��"HIW"��"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "��");
	set("long", "�t�ϦѤH�榫���F��C");
	set("mass", 1);
	set("value", 1);
	set("flag/no_amount", 1);

	if( clonep() )
		call_out( (: destruct_self :), 180 ); // �T�������S�e��N����
}
