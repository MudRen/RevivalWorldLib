/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hoi_bow.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-22
 * Note   : �Z���g��}
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
	set_idname("hoi bow", HIR"�Z��"NOR+RED"�g��"HIW"�}"NOR);
	set_temp("status", HIB"20"NOR BLU"07 "HIB"��"NOR BLU"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "����o�񪺬�����§���C\n");
	set("unit", "��");
	set("mass", 10);
	set("value", 1);
	set("badsell", 1);
	set("flag/no_amount", 1);
	
	set("skill", "stick");

	set(ATTACK, 1);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME�|�_�⤤"+this_object()->query_name()+"��$YOU�����A";
}

//
// �Z���۰ʯS�����
//
int special_attack(object attacker, object defender)
{
	if( !random(50) )
	{
		int damage = range_random(50, 100);

		msg("$ME���|�⤤��"+this_object()->query_idname()+"���ѪŤ@�g�A�@���������Ӷ��q�ѤW�ƤF�U�ӡA��$YOU�y���Y���ˮ`�I\n", attacker, defender, 1);

		return damage;
	}

	return 0;
}
