/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_card.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : �G���d��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

mapping actions;

void do_praydeer(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��֨ϥ��G���d�į�H\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( !userp(target) )
		return tell(me, pnoun(2, me)+"�u��缾�a�ϥ��G���d�į�C\n");

	msg("$ME�j�ۤ@�n�G�ϥ��]�k�d�u"HIR"�G"NOR+RED"��"HIW"�d"NOR"�v�A$YOU��o�t�ϦѤH�����֡I�I\n", me, target, 1);

	target->start_condition(CHRISTMAS_2007);
}

void create()
{
	set_idname("deer card", HIR"�G"NOR+RED"��"HIW"�d"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "�i");
	set("long", "���j�ɥN����t�ϦѤH���֪��d���A�㦳���_���O�q�C");
	set("mass", 10);
	set("value", 1);
	set("flag/no_amount", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);

	set(BUFF_STR, 1);
	set(BUFF_PHY, 1);
	set(BUFF_AGI, 1);
	set(BUFF_INT, 1);
	set(BUFF_CHA, 1);

	set(BUFF_STAMINA_REGEN, 5);
	set(BUFF_HEALTH_REGEN, 5);
	set(BUFF_ENERGY_REGEN, 5);

	::setup_equipment(EQ_ETC, HIG"�d"NOR GRN"��"NOR);

	actions = ([ "praydeer" : (: do_praydeer :) ]);
}
