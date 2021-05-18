/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : urocissa_caerulea.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �O�W���N
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

mapping defeat_titles =
([
	10 		:	HIB"��"NOR BLU"�N"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIB"��"NOR BLU"�N"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIB"��"NOR BLU"�N"HIR"��"NOR RED"��"NOR,
	2000	:	HIB"��"NOR BLU"�N"HIM"��"NOR MAG"��"NOR,
	5000	:	HIB"��"NOR BLU"�N"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("arc "+me->query_id(1));
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
	int is_named = query("named");
	object ob;

	if( !random(4) )
	{
		ob = new("/quest/lost_shard/lost_shard_5");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_boots" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_caerulea_breast_plate",
				"/obj/armor/zen_caerulea_leggings",
				"/obj/armor/zen_caerulea_helm",
				"/obj/armor/zen_caerulea_gloves",
				"/obj/armor/zen_caerulea_cloak",			
				"/obj/armor/zen_caerulea_bracers",
				"/obj/armor/zen_caerulea_boots",
				"/obj/armor/zen_caerulea_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/caerulea_fist",
				"/obj/armor/caerulea_breast_plate",
				"/obj/armor/caerulea_leggings",
				"/obj/armor/caerulea_helm",
				"/obj/armor/caerulea_gloves",
				"/obj/armor/caerulea_cloak",			
				"/obj/armor/caerulea_bracers",
				"/obj/armor/caerulea_boots",
				"/obj/armor/caerulea_belt",
			});
		}
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	}

	TREASURE_D->get_treasure(this_object());
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, BOSS_HUNTER);	
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, BOSS_HUNTER);
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME���R�ۥi�Ȫ����l��$YOU�r�O�����A";
			break;
		case 1:
			return "$ME���ĦV�U�A����$YOU�A";
			break;
		case 2:
			return "$ME�@�L�֩�$YOU���L���A";
			break;
		case 3:
			return "$ME�ίͻH�����a����$YOU���I���A";
			break;
		case 4:
			return "$ME�r�O�a��$YOU�֨�����A";
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
	if( !random(200) )
	{
		shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");
	}

	if( !random(20) )
	{
		int damage;
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			damage = range_random(80,120);

			msg("$ME���Ķ����A�ϥX�u"HIW"�U��"NOR WHT"���b"NOR"�v��$YOU�y���Y���ˮ`�I�I�I\n"NOR, attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}

int is_boss()
{
	return 2;
}

void auto_disappear()
{
	destruct(this_object());
}

void create()
{
	set_idname(HIB"urocissa "NOR BLU"caerulea"NOR, HIB"�O�W"NOR BLU"���N"NOR);

	set("unit", "��");
	set("age", 50);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
	set("attr/int", 100);
	set("attr/agi", 150);
	set("attr/cha", 20);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 15000);
	set("abi/health/cur", 15000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);
	set("job/name", HIW+"����"+NOR);

	set("skills/unarmed/level", 130);
	set("skills/dodge/level", 130);
	set("skills/fatalblow/level", 3);

	set("long", "�@�@���Ф�G�R���O�W���N�C");

	set("autokill", 1);

	if( !random(15) && clonep() )
	{
		MAPNPC_D->become_named_npc(this_object());
		call_out((: auto_disappear :), 3600 + random(1800));
	}

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
