/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : neofelis_nebulosa.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ���\
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
	10 		:	HIY"��"NOR YEL"�\"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIY"��"NOR YEL"�\"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIY"��"NOR YEL"�\"HIR"��"NOR RED"��"NOR,
	2000	:	HIY"��"NOR YEL"�\"HIM"��"NOR MAG"��"NOR,
	5000	:	HIY"��"NOR YEL"�\"WHT"��"HIW"��"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_4");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_bracers" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_nebulosa_breast_plate",
				"/obj/armor/zen_nebulosa_leggings",
				"/obj/armor/zen_nebulosa_helm",
				"/obj/armor/zen_nebulosa_gloves",
				"/obj/armor/zen_nebulosa_cloak",			
				"/obj/armor/zen_nebulosa_bracers",
				"/obj/armor/zen_nebulosa_boots",
				"/obj/armor/zen_nebulosa_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/nebulosa_fist",
				"/obj/armor/nebulosa_breast_plate",
				"/obj/armor/nebulosa_leggings",
				"/obj/armor/nebulosa_helm",
				"/obj/armor/nebulosa_gloves",
				"/obj/armor/nebulosa_cloak",			
				"/obj/armor/nebulosa_bracers",
				"/obj/armor/nebulosa_boots",
				"/obj/armor/nebulosa_belt",
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
			return "$ME���e�Ĩ�A����$YOU�A";
			break;
		case 2:
			return "$ME�@�f�r��$YOU���L���A";
			break;
		case 3:
			return "$ME�αj�����j�L�r��$YOU���I���A";
			break;
		case 4:
			return "$ME�r�O�a��$YOU���r�����A";
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

	if( !random(10) )
	{
		msg("$ME�q�s�@�n�A�ϥX�u"HIY"���t"NOR YEL"����"NOR"�v�y��$YOU�Y�����ˮ`�I�I\n"NOR, attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, range_random(200, 400));

		return;
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
	set_idname(HIY"neofelis"NOR YEL" nebulosa"NOR, HIY"��"NOR YEL"�\"NOR);

	set("unit", "��");
	set("age", 20);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 150);
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

	set("long", "�@�@�����c�����\�C");

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
