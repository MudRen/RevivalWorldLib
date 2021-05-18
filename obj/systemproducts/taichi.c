/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : taichi.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-12-20
 * Note   : �ӷ���
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
	string skill = "taichi";

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
	set_idname(HIW"tai "NOR WHT"chi"NOR, HIW"��"NOR WHT"��"NOR"��");

	if( this_object()->set_shadow_database() ) return;

	set("unit", "��");
	set("long", "�����ӷ��A�O�ͨ���C����ͥ|�H�A�|�H�ͤK���C");
	set("mass", 10);
	set("value", 1);

	actions = ([ "readbook" : (: do_readbook :) ]);
}
