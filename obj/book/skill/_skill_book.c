/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _skill_book.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-08
 * Note   : �ޯ��
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <inherit.h>
inherit STANDARD_OBJECT;

#include <skill.h>
#include <ansi.h>

#define BOOK_EXP	200
#define COST_EXP	100000

mapping actions;
	
void do_readbook(object me, string arg)
{
	int exp;
	string skill = query("skill");
	mapping baseskill = query("baseskill");
	string skill_type_name;
	
	foreach(string sk, int level in baseskill)
	{
		if( me->query_skill_level(sk) < level )
		{
			if( me->is_module_npc() )
				me->do_command("say �ڪ��u"+(SKILL(sk))->query_idname()+"�v���Ť��� "+level+"�A�L�k�A�ѡu"+(SKILL(skill))->query_idname()+"�v�C");
			else
				return tell(me, pnoun(2, me)+"���u"+(SKILL(sk))->query_idname()+"�v���Ť��� "+level+"�A�L�k�A�ѡu"+(SKILL(skill))->query_idname()+"�v�C\n");
		}
	}

	if( me->query_skill_level(skill) == (SKILL(skill))->max_level(me) )
		return tell(me, pnoun(2, me)+"���u"+(SKILL(skill))->query_idname()+"�v�w�F��̰����šC\n");

	if( (SKILL(skill))->query_skill_type() == COMBAT_SKILL )
	{
		if( !me->cost_combat_exp(COST_EXP) )
		{
			if( me->is_module_npc() )
				me->do_command("say �ڪ��԰��g��Ȥ��� "+COST_EXP+"�A�L�k�ǲߦ��ޯ�C");
			else
				return tell(me, pnoun(2, me)+"���԰��g��Ȥ��� "+COST_EXP+"�A�L�k�ǲߦ��ޯ�C\n");	
		}
			
		skill_type_name = "�԰�";
	}
	else
	{
		if( !me->cost_social_exp(COST_EXP) )
		{
			if( me->is_module_npc() )
				me->do_command("say �ڪ����|�g��Ȥ��� "+COST_EXP+"�A�L�k�ǲߦ��ޯ�C");
			else
				return tell(me, pnoun(2, me)+"�����|�g��Ȥ��� "+COST_EXP+"�A�L�k�ǲߦ��ޯ�C\n");
		}
			
		skill_type_name = "���|";
	}

	exp = BOOK_EXP * me->query_int();

 	msg("$ME�}�l�{�uŪ�_$YOU�A�u��$ME�ŵM�}�Ԧ������_���I�Y�A���G�q���Y�o��F�ܦh���F��C\n", me, this_object(), 1);

	msg("$ME�ӶO "+COST_EXP+" �I��"+skill_type_name+"�g��ȨӶi��ǲߡA�@�o�� "+exp+" �I���u"+(SKILL(skill))->query_idname()+"�v�g��ȡC\n", me, 0, 1);
	me->add_skill_exp(skill, exp);
	me->save();
	msg("$YOU�b$ME�\Ū����K�Ƭ��H���C\n", me, this_object(), 1);

	destruct(this_object(), 1);
}

void create()
{
	string skill = query("skill");
	
	if( !skill ) return;

	set_idname(HIG+(SKILL(skill))->query_id()+NOR GRN" book"+NOR, HIG+(SKILL(skill))->query_name()+NOR GRN"�޳N��"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	actions = ([ "readbook" : (: do_readbook :) ]);

	if( this_object()->set_shadow_database() ) return;
	
	set("long", "�@�������}�����޳N�ѡA�γ\�z��q���ǲߨ�@�ǯS��ޯ�C\n\n  "+query("note", find_object(SKILL(skill))));
	set("unit", "��");
	set("mass", 300);
	set("value", 10000);
	set("badsell", 1);
}
