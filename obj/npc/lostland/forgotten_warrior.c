/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : forgotten_warrior.c
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
	destruct(all_inventory(this_object()));

	TREASURE_D->get_treasure(this_object());
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{

}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	if( enemy->is_module_npc() )
	{
		object boss = find_player(query("boss", enemy) || "");
		
		if( !objectp(boss) )
			return 0;
			
		return valid_enemy(boss);
	}

	if( enemy->in_condition(LOSTSEAL) ) 
		return 0;

	return 1;	
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
/*
	switch(random(5))
	{
		case 0:
			return "$ME���c����$YOU�@�f�r�ӡA";
			break;
		case 1:
			return "$ME��$YOU���I��ļ��L�ӡA";
			break;
		case 2:
			return "$ME�Ϋ�L�r��$YOU���L���A";
			break;
		case 3:
			return "$ME�q�����ֳt����$YOU�A";
			break;
		case 4:
			return "$ME���R�r��$YOU����A";
			break;
	}
*/
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
varargs void special_attack(object attacker, object defender)
{
	if( !random(8) )
	{
		int damage;
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			damage = range_random(50,100);

			msg("$ME�ƨg�a�j�d���������A��$YOU�y���Y�����ˮ`�I�I�I\n"NOR, attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}

void create()
{
	set_idname("forgotten warrior", "�Q��Ѫ��Ԥh");

	set("unit", "��");
	set("age", 15);
	set("gender", MALE_GENDER);
	
	set("attr/str", 60);
	set("attr/phy", 60);
	set("attr/int", 60);
	set("attr/agi", 60);
	set("attr/cha", 60);

	set("abi/stamina/max", 1500);
	set("abi/stamina/cur", 1500);
	set("abi/health/max", 1500);
	set("abi/health/cur", 1500);
	set("abi/energy/max", 1500);
	set("abi/energy/cur", 1500);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);
	set("skills/fatalblow/level", 3);

	set("long",@TEXT
�@�@�Q��Ѫ��Ԥh�A�F��Q�T�D�b�o���I���j�a�̡A�u�ѤU��{���԰��N�ӡC
TEXT);

	set("autokill", 1);

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
