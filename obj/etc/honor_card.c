/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : honor_card.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-02-09
 * Note   : �aģ�d��
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

void do_pray(object me, string arg)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��֨ϥκaģ�d���H\n");

 	if(!objectp( target = present(arg, environment(me))) )
		return tell( me,"�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( !userp(target) && !target->is_module_npc() )
		return tell(me, pnoun(2, me)+"�u��缾�a�έ��u�ϥκaģ�d���C\n");

	msg("$ME�j�ۤ@�n�G�ϥ��]�k�d�u"HIY"�a"NOR+YEL"ģ"HIY"�d"NOR YEL"��"NOR"�v�A$YOU��o�u�۪����֡I�I\n", me, target, 1);

	target->start_condition(HONOR);
}

void create()
{
	set_idname("honor card", HIY"�a"NOR+YEL"ģ"HIY"�d"NOR YEL"��"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "�i");
	set("long", "�H�x�C�����̰����aģ�A�㦳���_���O�q�A�����U�h�i�ĪG�U�j�C");
	set("mass", 10);
	set("value", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);

	::setup_equipment(EQ_ETC, HIG"�d"NOR GRN"��"NOR);

	actions = ([ "pray" : (: do_pray :) ]);
}
