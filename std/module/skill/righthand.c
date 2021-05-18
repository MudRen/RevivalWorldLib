/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : righthand.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-08-17
 * Note   : �k���k��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <inherit.h>
#include <message.h>
#include <daemon.h>

inherit STANDARD_SKILL;

void create()
{
	// �ޯत��
	set("name", "�k���k��");

	// �ޯ�^��
	set("id",		"righthand");

	// �ޯ��Y��
	//set("title/100",	"");
	
	// �ޯ����
	set("note",		"���ư�¦�βy�V�m�A�v�T�Ҧ���{�A��⬰�k��A���̬��k��(1/3)");
	
	// ����ɥN
	set("age",		3);

	// �ޯ����
	set("type",		SOCIAL_SKILL);
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( objectp(ob) )
	{
		if( ob->query_skill_level("lefthand") > 0 || ob->query_skill_level("twohands") > 0 )
			return 0;
		else
			return 1;
	}

	if( undefinedp(ob) )
		return NPC_SKILL;
	else
	{
		if( ob->is_npc() )
			return 1;
		else
			return 0;
	}
}

// �i�۳]�C�ӵ��Żݭn���g���, �Ϊ����ϥ� SKILL_D �w�]�ƾǦ�
int level_exp(int lv)
{
	return SKILL_D->query_level_exp(lv);
}

// �i�]�w�U�س̰����ű���, 
int max_level(object ob)
{
	return 999;
}

// �i�]�w���P���󤧸g��ȭ���
float exp_multiple()
{
	return 0.1;
}

// �ɯŤ��e���ƥ�
void after_level_up(object ob, int newlevel)
{

}

// ���Ť��᪺�ƥ�
void before_level_down(object ob, int oldlevel)
{
	
}

// �g��ȼW�[�᪺�ƥ�
void added_exp(object ob, float exp)
{

}

// �O�_�u��ۧھǲ�
int self_learning()
{
	return 0;
}

// �S��аO
string query_special_type()
{
	return "";
}
