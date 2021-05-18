/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ursus_thibetanus_formosanus.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �O�W�º�
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
	10 		:	GRN"��"NOR HIG"��"HIY"�y"NOR YEL"�H"NOR,
	100		:	GRN"��"NOR HIG"��"HIB"�g"NOR BLU"�P"NOR,
	1000	:	GRN"��"NOR HIG"��"HIR"��"NOR RED"��"NOR,
	2000	:	GRN"��"NOR HIG"��"HIM"��"NOR MAG"��"NOR,
	5000	:	GRN"��"NOR HIG"��"WHT"��"HIW"��"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_6");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_belt" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_thibetanus_breast_plate",
				"/obj/armor/zen_thibetanus_leggings",
				"/obj/armor/zen_thibetanus_helm",
				"/obj/armor/zen_thibetanus_gloves",
				"/obj/armor/zen_thibetanus_cloak",			
				"/obj/armor/zen_thibetanus_bracers",
				"/obj/armor/zen_thibetanus_boots",
				"/obj/armor/zen_thibetanus_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/thibetanus_fist",
				"/obj/armor/thibetanus_breast_plate",
				"/obj/armor/thibetanus_leggings",
				"/obj/armor/thibetanus_helm",
				"/obj/armor/thibetanus_gloves",
				"/obj/armor/thibetanus_cloak",			
				"/obj/armor/thibetanus_bracers",
				"/obj/armor/thibetanus_boots",
				"/obj/armor/thibetanus_belt",
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
			return "$ME���R�ۥi�Ȫ�������$YOU�r�O�����A";
			break;
		case 1:
			return "$ME�ֳt���e«��$YOU���y���r�ӡA";
			break;
		case 2:
			return "$ME�@�f�r��$YOU���V���A";
			break;
		case 3:
			return "$ME�q�����ֳt����$YOU�A";
			break;
		case 4:
			return "$ME�r�O�a��$YOU�Ĩ�����A";
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
	if( !random(10) )
	{
		msg("$ME�q�s�@�n�A�ϥX�u"HIG"�u��"NOR GRN"���v"NOR"�v�y��$YOU�Y�����ˮ`�I�I\n"NOR, attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, range_random(100, 200));
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
	set_idname(GRN"ursus thibetanus"NOR HIG" formosanus"NOR, GRN"�O�W"NOR HIG"�º�"NOR);

	set("unit", "��");
	set("age", 20);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 150);
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

	set("long", "�@�@�����c���O�W�º��A�����ܶ¦�ʤ�A���ߧ��ӡA�|�ϲʵu�A�e��ϳ��㦳���k�C");

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
