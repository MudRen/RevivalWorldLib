/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : minotaur.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-08-18
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <condition.h>
#include <autokill.h>

inherit STANDARD_PERSON;

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("bite "+me->query_id(1));
}

int target_gone(object me)
{

}

/*
void reply_get_object(object me, object ob, mixed amount)
{

}
*/

// ���`�ɪ��ƥ�
void reply_die()
{
	TREASURE_D->get_treasure(this_object());
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return 1;	
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME�������b���򬽬��a��$YOU�A�ӡA";
			break;
		case 1:
			return "$ME��$YOU���I��ļ��L�ӡA";
			break;
		case 2:
			return "$ME��q�@�n�Τ�y�r��$YOU���Y���A";
			break;
	}
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
varargs int special_attack(object attacker, object defender)
{
	

}

void create()
{
	int temp;

	set_idname("minotaur", "���Y��");

	set("unit", "��");
	set("age", range_random(25,35));
	set("gender", MALE_GENDER);
	
	set("attr/str", range_random(40,80));
	set("attr/phy", range_random(40,80));
	set("attr/int", range_random(40,80));
	set("attr/agi", range_random(40,80));
	set("attr/cha", range_random(40,80));

	temp = range_random(2000, 5000);
	set("abi/stamina/max", temp);
	set("abi/stamina/cur", temp);
	temp = range_random(2000, 5000);
	set("abi/health/max", temp);
	set("abi/health/cur", temp);
	temp = range_random(2000, 5000);
	set("abi/energy/max", temp);
	set("abi/energy/cur", temp);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);

	set("long",@TEXT
�@�@�O�j�֫p�����Y�ǡA�⮳���b����A�~��u��C
TEXT);

	set("autokill", AUTOKILL_IMMEDIATELY);

	startup_living();
}
