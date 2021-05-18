/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rabbit_stick.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-22
 * Note   : ��߷o�l
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
	set_idname("rabbit stick", HIC"���"NOR+CYN"�o"HIW"�l"NOR);
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
	if( !random(100) )
	{
		msg("$ME�|�_�⤤��"+this_object()->query_idname()+"�A��������$YOU���ḣ�A��$YOU�y���w�t�I\n", attacker, defender, 1);

		defender->start_delay("moon_rabbit", 3, pnoun(2, defender)+"���b�w�t���C\n", pnoun(2, defender)+"����F�w�t�C\n");
	}

	return 0;
}
