/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : poseidon_cloak.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-11-10
 * Note   : �i��V�ܭ�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIC"po"HIR"sei"HIG"don"HIY" cloak", HIC"�i"HIR"��"HIG"�V"HIY"�ܭ�");
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�i��V���ǩ_�ܭ��A�֦����Z���O�q�C\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 100000);
	set("flag/no_amount", 1);
	set("special", 1);

	set(ATTACK, 25);
	set(BUFF_STR, 25);
	
	::setup_equipment(EQ_CLOAK, HIY"�ǩ_"NOR YEL"�ܭ�"NOR);
}
