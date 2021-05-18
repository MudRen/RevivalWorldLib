/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : forgotten_ancient_knight.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
 * Note   : �Q��Ѫ����j�Z�h
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
	10 		:	WHT"�Z"HIW"�h"HIY"�y"NOR YEL"�H"NOR,
	100		:	WHT"�Z"HIW"�h"HIB"�g"NOR BLU"�P"NOR,
	1000	:	WHT"�Z"HIW"�h"HIR"��"NOR RED"��"NOR,
	2000	:	WHT"�Z"HIW"�h"HIM"��"NOR MAG"��"NOR,
	5000	:	WHT"�Z"HIW"�h"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("hmm "+me->query_id(1));
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
	int num = environment(this_object())->query_num();
	object ob;
	string* random_drop;
	
	CHANNEL_D->channel_broadcast("combat", this_object()->query_idname()+"�R�R�a���b"+AREA_D->query_area_idname("lostland", num)+"���j�a�W�A�Q�A�G���F��Ƭ�"HIR"�C"HIY"�m"HIG"��"HIC"�~"NOR"�A�v������...");
	
	if( !random(is_named ? 8 : 40) )
	{
		random_drop = ({ "/obj/armor/chi_lord_leggings" });
	}
	else if( !random(is_named ? 4 : 20) )
	{
		random_drop =
		({
			"/obj/weapon/ancient_blade",
			"/obj/armor/zen_ancient_breast_plate",
			"/obj/armor/zen_ancient_leggings",
			"/obj/armor/zen_ancient_helm",
			"/obj/armor/zen_ancient_gloves",
			"/obj/armor/zen_ancient_cloak",			
			"/obj/armor/zen_ancient_bracers",
			"/obj/armor/zen_ancient_boots",
			"/obj/armor/zen_ancient_belt",
		});
	}
	else
	{
		random_drop =
		({
			"/obj/weapon/ancient_blade",
			"/obj/armor/ancient_breast_plate",
			"/obj/armor/ancient_leggings",
			"/obj/armor/ancient_helm",
			"/obj/armor/ancient_gloves",
			"/obj/armor/ancient_cloak",			
			"/obj/armor/ancient_bracers",
			"/obj/armor/ancient_boots",
			"/obj/armor/ancient_belt",
		});
	}

	ob = new(random_drop[random(sizeof(random_drop))]);
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
	
	if( !random(2) )
	{
		broadcast(environment(this_object()), NOR WHT"����ǨӲӲӦջy�G"NOR"�u�i�̭̭n�p�ߧo�A���j���c�]�N�b�U�체�e�F�A�@�Ѧ����ڧ������@...�v\n");
		ob = new("/quest/lost_shard/lostland_key_2");
		ob->move(this_object());
	}
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, LOSTSEAL);	
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, LOSTSEAL);
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	/*
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
	*/
}

// ���m�ɪ��T��
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// �S�����
void special_attack(object attacker, object defender)
{
	int time;

	if( !random(200) )
	{
		shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");
	}

	if( !random(100) )
	{
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) ) continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			time = range_random(1, 5);

			msg("$ME�@�����V�a���A�ϥX�u"HIY"�U��"NOR YEL"���a"NOR"�v�A��$YOU�y���u�ȷw�t("+time+" ��)�A�óy�����L���ˮ`�I�I�I\n"NOR, attacker, ob, 1);
			
			ob->start_delay("forgotten_ancient_knight", time, pnoun(2, ob)+"�]���u"HIY"�U��"NOR YEL"���a"NOR"�v���v�T�ӷw�t���C\n", pnoun(2, ob)+"����F�w�t�C\n");
			
			COMBAT_D->cause_damage(attacker, ob, 50);
		}
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(NOR WHT"forgotten"HIW" ancient knight"NOR, NOR WHT"�Q��Ѫ�"HIW"���j�Z�h"NOR);

	set("unit", "��");
	set("age", 50);
	set("gender", MALE_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 250);
	set("attr/cha", 300);

	set("abi/stamina/max", 30000);
	set("abi/stamina/cur", 30000);
	set("abi/health/max", 30000);
	set("abi/health/cur", 30000);
	set("abi/energy/max", 30000);
	set("abi/energy/cur", 30000);
	set("job/name", HIW+"������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 6);

	set("prestige", 1);// ���`�ᵹ�����a���n��

	set("long", "�@�@��Q��Ѫ����j�Z�h�A�F�����F�A�G�A�N�Ѥ��u�s�b�ۦ�{�B�O���B����C");

	set("autokill", 1);

	startup_living();
	
	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
}

int remove()
{
	destruct(all_inventory(this_object()));
}
