/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : poseidon.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : �i��V(�� SHOPPING_D �X�ʲ��͡A��i��V�X�{�ɱN�ɭP���@�ɸg�ٿ���)
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
	10 		:	HIC"�i"HIR"��"HIG"�V"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIC"�i"HIR"��"HIG"�V"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIC"�i"HIR"��"HIG"�V"HIR"��"NOR RED"��"NOR,
	2000	:	HIC"�i"HIR"��"HIG"�V"HIM"��"NOR MAG"��"NOR,
	5000	:	HIC"�i"HIR"��"HIG"�V"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("say "+me->query_name()+"�x����ڤ��q�H");
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

	ob = new("/quest/epic_fishfarm/heart_of_ocean");
	ob->move(this_object());

	if( !random(is_named ? 10 : 50) )
	{
		if( !random(is_named ? 10 : 50) )
			ob = new("/obj/armor/zen_poseidon_cloak");
		else
			ob = new("/obj/armor/poseidon_cloak");

		ob->move(this_object());
	}

	if( !random(is_named ? 2 : 10) )
	{
		ob = new("/obj/weapon/faith_halberd");
		ob->move(this_object());
	}

	TREASURE_D->get_treasure(this_object());

	CHANNEL_D->channel_broadcast("news", this_object()->query_idname()+"�j�q�G�u�L�����H���I���ٷ|�A�^�Ӫ��I�v...�H��͢�������A�������v����_���R...");
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::reply_defeat(attackers);
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
			return "$ME�|�_�T�e�u��$YOU�r�O�����A";
			break;
		case 1:
			return "$ME��q���U�ļ�$YOU�A";
			break;
		case 2:
			return "$ME��_���}��$YOU�Ҧb��m�r�O��U�A";
			break;
		case 3:
			return "$ME�H�U��O�q�N���鱽��$YOU�A";
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
	int damage;

	if( !random(10) )
	{
		if( !random(2) )
			shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");

		damage = range_random(200, 500);

		msg("$ME�|�_���j���T�e�u�r�O�a���L�������A�u"HIC"���S"NOR CYN"���i"NOR"�v������J$YOU������A�y���Y���ˮ`�I�I\n"NOR, attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(HIC"po"HIR"sei"HIG"don"NOR, HIC"�i"HIR"��"HIG"�V"NOR);

	set("unit", "�L");
	set("age", 4000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
	set("attr/int", 250);
	set("attr/agi", 200);
	set("attr/cha", 200);

	set("abi/stamina/max", 50000);
	set("abi/stamina/cur", 50000);
	set("abi/health/max", 50000);
	set("abi/health/cur", 50000);
	set("abi/energy/max", 50000);
	set("abi/energy/cur", 50000);
	set("job/name", HIW+"������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 100);

	set("prestige", 1);// ���`�ᵹ�����a���n��

	set("long", "�@�i��V�A���饨�j�A����þ���ܷ��������A�z���������C�i��V���߫k�k�A�ӥB�n\n�ԡC��������L�Ҿ֦����v�O�A��L����ɮ����N�|�X�{�Ǫ��A�L���ʤT�e�u�N��ް_\n���S�M�a�_�C");

	set("autokill", 1);

	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
		
	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
