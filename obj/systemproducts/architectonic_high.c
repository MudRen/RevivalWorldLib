/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : architectonic_high.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-23
 * Note   : ���j�ؿv
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <skill.h>

inherit STANDARD_OBJECT;

#define BOOK_EXP	200
#define COST_EXP	100000

mapping actions;

void do_readbook(object me, string arg)
{
	int exp;
	string skill = "architectonic-high";

	if( me->is_module_npc() )
	{
		me->do_command("say �ڵL�k�ǲߦ��ޯ�C");
		return;
	}

	exp = BOOK_EXP * me->query_int();

	msg("$ME�J�Ӿ\Ū$YOU�A�@�o�� "+exp+" �I���u"+(SKILL(skill))->query_idname()+"�v�g��ȡC\n", me, this_object(), 1);
	me->add_skill_exp(skill, exp);
	me->save();
	msg("$YOU�b$ME�\Ū����K�Ƭ��H���C\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	set_idname(WHT"great "HIW"architectonic"NOR, WHT"���j"HIW"�ؿv"NOR YEL"�j�y"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "��");
	set("long", "���j�ѯ���d�b�H�����j�y�A���Y�O���ۦU�د��G�_�ު��ؿv�޳N�C");
	set("mass", 10);
	set("value", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);
}
