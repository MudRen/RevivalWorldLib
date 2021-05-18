/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_poseidon_cloak.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-11-10
 * Note   : �I�D�i��V�ܭ�
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
	set_idname(HIW"zen "NOR HIC"po"HIR"sei"HIG"don"HIY" cloak", HIW"�I"NOR WHT"�D"NOR HIC"�i"HIR"��"HIG"�V"HIY"�ܭ�");
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�i��V���ǩ_�ܭ��A�֦����Z���O�q�C\n");
	set("unit", "��");
	set("mass", 100);
	set("value", 100000);
	set("flag/no_amount", 1);
	set("special", 1);

	set(ATTACK, 75);
	set(BUFF_STR, 75);
	
	::setup_equipment(EQ_CLOAK, HIY"�ǩ_"NOR YEL"�ܭ�"NOR);
}
