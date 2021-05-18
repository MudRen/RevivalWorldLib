/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : blainville's_beaked_whale.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-08-18
 * Note   : ��������H
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

inherit STANDARD_MONSTER;

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
			
			damage = range_random(50, 100);

			msg("$ME�ƨg�a�j�d���������A��$YOU�y���Y�����ˮ`�I�I�I\n"NOR, attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}

void create()
{
	set_idname("blainville's beaked whale", "��������H");

	set("unit", "��");
	set("age", 58);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 200);
	set("attr/phy", 200);
	set("attr/int", 200);
	set("attr/agi", 200);
	set("attr/cha", 200);

	set("abi/stamina/max", 10000);
	set("abi/stamina/cur", 10000);
	set("abi/health/max", 10000);
	set("abi/health/cur", 10000);
	set("abi/energy/max", 10000);
	set("abi/energy/cur", 10000);

	set("skills/unarmed/level", 150);
	set("skills/dodge/level", 150);
	set("skills/fatalblow/level", 3);

	set("long",@TEXT
�@�@��������H��������H��ʧ��Y���説�A�U�E���j�����_�A���ʬƥi���X
�W�E�����u�A�U�E�������@��j�������Y�X��~�A���ʦ��馹�@�S�x����A��
�H�h�S���~�S������ �C
TEXT);

	set("autokill", 1);

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
