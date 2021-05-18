/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : aircraft-fac.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-11
 * Note   : ��ž���s?
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
	set("name", 		"��ž���s�@");

	// �ޯ�^��
	set("id",		"aircraft-fac");

	// �ޯ��Y��
	set("title/40",		NOR HIY"��ž����"NOR);
	set("title/100",	NOR HIY"��ž���v"NOR);
	
	// �ޯ����
	set("note",		"��ž���s�@�޳N(�ݤ@���Ͳ��`���ŹFLv100)");
	
	// ����ɥN
	set("age",		3);
	
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
			return 
			ob->query_skill_level("metalclassify")+
			ob->query_skill_level("waterclassify")+
			ob->query_skill_level("stoneclassify")+
			ob->query_skill_level("woodclassify")+
			ob->query_skill_level("fuelclassify") >= 100;
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
	return 100;
}

// �i�]�w���P���󤧸g��ȭ���
float exp_multiple()
{
	return 0.02;
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

// �S���аO
string query_special_type()
{
	return "";
}