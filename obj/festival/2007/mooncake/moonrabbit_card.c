/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moonrabbit_card.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-28
 * Note   : ��ߥd��
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

void do_prayrabbit(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��֨ϥΤ��a�ѪšH\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( !userp(target) )
		return tell(me, pnoun(2, me)+"�u��缾�a�ϥΤ�ߥd�į�C\n");

	msg("$ME�j�ۤ@�n�G½�}�л\�b�i���W�������d�u"HIC"��"NOR+CYN"��"HIW"�d"NOR"�v�A$YOU�i��o��ߪ����֡I�I\n", me, target, 1);

	target->start_condition(MOONCAKE_2007);
}

void create()
{
	set_idname("moonrabbit card", HIC"��"NOR+CYN"��"HIW"�d"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "�i");
	set("long", "���j�ɥN�����߯��֪��d���A�㦳���_���O�q�C");
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

	actions = ([ "prayrabbit" : (: do_prayrabbit :) ]);
}
