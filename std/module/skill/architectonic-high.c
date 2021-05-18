/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : architectonic-high.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-01-23
 * Note   : ���j�ؿv
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
	set("name", 	"���j�ؿv");

	// �ޯ�^��
	set("id",		"architectonic-high");

	// �ޯ��Y��
	//set("title/50",   HIC"��"NOR CYN"��"HIC"��"NOR CYN"��"NOR);
	//set("title/100",  HIW"��"NOR WHT"��"HIW"�D"NOR WHT"�_"NOR);
	
	// �ޯ����
	set("note",		"���j�ѯ���d�b�H�����j�y�A���Y�O���ۦU�د��G�_�ު��ؿv�޳N");
	
	// ����ɥN
	set("age",		2);
	
	// �ޯ����
	set("type",		SOCIAL_SKILL);
}

// ����i�ǲ߹�H, �i�Ψӳ]�w�U�ؾǲ߱���
varargs int allowable_learn(object ob)
{
	if( undefinedp(ob) )
		return PLAYER_SKILL;
	else
	{
		if( ob->is_npc() )
			return 0;
		else
			return 1;
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
	if( ob->is_module_npc() )
		return 100;
	else
		return 100;
}

// �i�]�w���P���󤧸g��ȭ���
float exp_multiple()
{
	return 1;
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
	return 1;
}

// �S��аO
string query_special_type()
{
	return HIW"�v"NOR WHT"��"NOR;
}

