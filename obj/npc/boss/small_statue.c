/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_2nd_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-23
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
#include <buff.h>
#include <autokill.h>

inherit STANDARD_MONSTER;

string ownerid;

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	
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

}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	if( !query("boss") )
		return 1;

	if( userp(enemy) || enemy->is_module_npc() )
		return 0;
	
	return 1;	
}

int is_summon_mob()
{
	return stringp(query("boss"));
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME�|�_�e�p�ۮ���$YOU�r�O���h�A";
			break;
		case 1:
			return "$ME��q���U�ļ�$YOU�A";
			break;
		case 2:
			return "$ME��_�@���۸}��$YOU�Ҧb��m�r�O��U�A";
			break;
		case 3:
			return "$ME���ʤp�p�����u���U����$YOU�A";
			break;
		case 4:
			return "$ME�r�O�a��$YOU�ļ������A";
			break;
	}
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
void special_attack(object attacker, object defender)
{

}

void destruct_self()
{
	msg("\n$ME��M�H���@�a�A�Ƭ��Фg...�C\n\n", this_object(), 0, 1);	
	
	destruct(this_object());
}

private void do_follow_assist(object owner)
{
	if( objectp(owner) )
	{
		do_command("follow "+owner->query_id(1));
		do_command("assist "+owner->query_id(1));

		foreach(object enemy in COMBAT_D->query_fighting_targets(owner))
			COMBAT_D->start_fight(this_object(), enemy);
	}
}

varargs void create(object owner)
{
	int has_owner = objectp(owner);

	if( has_owner )
	{
		set_idname(WHT"small statue"NOR, WHT"�p�۹�"NOR);
		set("long", "  "+owner->query_name()+"�l��X�Ӫ��p�۹��C");
		
		set("boss", owner->query_id(1));
		
		set_temp("protectors", ({ this_object() }) | (query_temp("protectors", owner)||allocate(0)), owner);

		set("skills/defend-stance-adv/level", 100);
		set("combat/stance/type", "defend-adv");
	
		call_out( (: do_follow_assist :), 0, owner);
	}
	else
	{
		set_idname(HIW"small "WHT"statue"NOR, HIW"�p"NOR WHT"�۹�"NOR);
		set("long", "�@�ѼC�l��X�Ӫ��p�۹��C");
	}
	
	set("unit", "�L");
	set("age", 1500);
	set("gender", MALE_GENDER);
	
	set("attr/str", has_owner ? to_int(owner->query_str()*0.4) : 200);
	set("attr/phy", has_owner ? to_int(owner->query_phy()*0.4) : 200);
	set("attr/int", has_owner ? to_int(owner->query_int()*0.4) : 200);
	set("attr/agi", has_owner ? to_int(owner->query_agi()*0.4) : 200);
	set("attr/cha", has_owner ? to_int(owner->query_cha()*0.4) : 200);

	set("abi/stamina/max", has_owner ? owner->query_stamina_max() : 5000);
	set("abi/stamina/cur", has_owner ? owner->query_stamina_max() : 5000);
	set("abi/health/max", has_owner ? owner->query_health_max() : 5000);
	set("abi/health/cur", has_owner ? owner->query_health_max() : 5000);
	set("abi/energy/max", has_owner ? owner->query_energy_max() : 5000);
	set("abi/energy/cur", has_owner ? owner->query_energy_max() : 5000);
	set("job/name", HIW+"��"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	
	if( clonep() )
		call_out( (: destruct_self :), has_owner ? 30 : 60 );

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
