/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : latrodectus_mactans.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-15
 * Note   : �¹��
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
	10 		:	WHT"��"NOR BLU"���"HIY"�y"NOR YEL"�H"NOR,
	100		:	WHT"��"NOR BLU"���"HIB"�g"NOR BLU"�P"NOR,
	1000	:	WHT"��"NOR BLU"���"HIR"��"NOR RED"��"NOR,
	2000	:	WHT"��"NOR BLU"���"HIM"��"NOR MAG"��"NOR,
	5000	:	WHT"��"NOR BLU"���"WHT"��"HIW"��"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_3");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_cloak" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_mactans_breast_plate",
				"/obj/armor/zen_mactans_leggings",
				"/obj/armor/zen_mactans_helm",
				"/obj/armor/zen_mactans_gloves",
				"/obj/armor/zen_mactans_cloak",			
				"/obj/armor/zen_mactans_bracers",
				"/obj/armor/zen_mactans_boots",
				"/obj/armor/zen_mactans_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/mactans_fist",
				"/obj/armor/mactans_breast_plate",
				"/obj/armor/mactans_leggings",
				"/obj/armor/mactans_helm",
				"/obj/armor/mactans_gloves",
				"/obj/armor/mactans_cloak",			
				"/obj/armor/mactans_bracers",
				"/obj/armor/mactans_boots",
				"/obj/armor/mactans_belt",
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
			return "$ME�S�X�i�Ȫ��r����$YOU�@�f�r�ӡA";
			break;
		case 1:
			return "$ME�ֳt���e«��$YOU���y���r�ӡA";
			break;
		case 2:
			return "$ME�Q�Φh���}�N$YOU�x��i������A";
			break;
		case 3:
			return "$ME�q�����ֳt����$YOU�A";
			break;
		case 4:
			return "$ME�q���B������$YOU���Y���Q�X�r�G�A";
			break;
	}
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
varargs void special_attack(object attacker, object defender)
{
	if( !random(30) )
	{
		msg("$ME�ϥX�u"HIC"�K��"NOR CYN"�j��"NOR"�v�b$YOU�����W���X�ƹD�˲��I�I�I\n", attacker, defender, 1);
		defender->start_condition(POISON_GIANT_SPIDER, 60, 5, attacker);
	}
}

int is_boss()
{
	return 1;
}

void auto_disappear()
{
	destruct(this_object());
}

void create()
{
	set_idname(WHT"latrodectus "NOR BLU"mactans"NOR, WHT"��"NOR BLU"���"NOR);

	set("unit", "��");
	set("age", 20);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
	set("attr/int", 100);
	set("attr/agi", 100);
	set("attr/cha", 20);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 7000);
	set("abi/health/cur", 7000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);
	set("job/name", HIW+"��"+NOR);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);
	set("skills/fatalblow/level", 3);

	set("long", "�@�@�������@�r���j��A�H�ɷǳƶi������C");

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
