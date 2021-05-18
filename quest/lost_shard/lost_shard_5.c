/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lost_shard_5.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-08-16
 * Note	  : �򥢪����j�Z���H��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void destruct_self()
{
	msg("�ܮz��$ME�T���_���ɶ����_�ʡA���}�Ƭ������C\n", this_object(), 0, 1);
	
	destruct(this_object());
}

void create()
{
	set_idname(NOR YEL"lost"HIY" shard"NOR, HIY"���j�Z���H��"NOR YEL"�u��v"NOR);

	if( clonep() )
		call_out( (: destruct_self :), 300 );

	if( this_object()->set_shadow_ob() ) return;

	set("disassemble", ([ 
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
	]));

	set("long", "�򥢪����j�Z���H���w�u��v�A���H���D�`�ܮz�A�Y��b���W�Ӥ[�i��|�l�a�C");
	set("unit", "��");
	set("value", 1000);
	set("mass", 100);
	set("flag/no_amount", 1);
}
