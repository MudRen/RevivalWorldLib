/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_rhombopteryx.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : ���������
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
	10 		:	HIB"����"NOR MAG"��"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIB"����"NOR MAG"��"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIB"����"NOR MAG"��"HIR"��"NOR RED"��"NOR,
	2000	:	HIB"����"NOR MAG"��"HIM"��"NOR MAG"��"NOR,
	5000	:	HIB"����"NOR MAG"��"WHT"��"HIW"��"NOR,
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
		random_drop = ({ "/obj/armor/chi_lord_breast_plate" });
	}
	else if( !random(is_named ? 4 : 20) )
	{
		random_drop =
		({
			"/obj/weapon/nessiteras_fist",
			"/obj/armor/zen_nessiteras_breast_plate",
			"/obj/armor/zen_nessiteras_leggings",
			"/obj/armor/zen_nessiteras_helm",
			"/obj/armor/zen_nessiteras_gloves",
			"/obj/armor/zen_nessiteras_cloak",			
			"/obj/armor/zen_nessiteras_bracers",
			"/obj/armor/zen_nessiteras_boots",
			"/obj/armor/zen_nessiteras_belt",
		});
	}
	else
	{
		random_drop =
		({
			"/obj/weapon/nessiteras_fist",
			"/obj/armor/nessiteras_breast_plate",
			"/obj/armor/nessiteras_leggings",
			"/obj/armor/nessiteras_helm",
			"/obj/armor/nessiteras_gloves",
			"/obj/armor/nessiteras_cloak",			
			"/obj/armor/nessiteras_bracers",
			"/obj/armor/nessiteras_boots",
			"/obj/armor/nessiteras_belt",
		});
	}

	ob = new(random_drop[random(sizeof(random_drop))]);
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
	
	if( !random(2) )
	{
		broadcast(environment(this_object()), this_object()->query_idname()+"���{�l���������Ӫ��ݪ��b�o��...\n");
		ob = new("/quest/lost_shard/lostland_key_3");
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
	switch(random(5))
	{
		case 0:
			return "$ME���R�ۥi�Ȫ�������$YOU�r�O�����A";
			break;
		case 1:
			return "$ME��q���U�ļ�$YOU�A";
			break;
		case 2:
			return "$ME��_���}��$YOU�Ҧb��m�r�O��U�A";
			break;
		case 3:
			return "$ME�H�U��O�q�N���ڱ���$YOU�A";
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
	int time;
	int damage;

	if( !random(100) )
	{
		if( !random(2) )
			shout(HIB"����"NOR"-�j�a�]"+attacker->query_idname()+"����q�_�ʤF�@�U�C\n");

		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) )
				continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			time = range_random(5, 10);

			damage = range_random(50, 200);

			msg("$ME�i�}�j�L��$YOU�j�q�A�ϥX�u"HIR"�_�j"NOR RED"�व"NOR"�v�A��$YOU�y���u�ȷw�t("HIY+time+NOR" ��)�A�óy���Y�����ˮ`�I�I�I\n"NOR, attacker, ob, 1);
			
			ob->start_delay("nessiteras_rhombopteryx", time, pnoun(2, ob)+"�]���u"HIR"�_�j"NOR RED"�व"NOR"�v���v�T�ӷw�t���C\n", pnoun(2, ob)+"����F�w�t�C\n");
				
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
		
		attacker->start_delay("nessiteras_rhombopteryx", 5, pnoun(2, attacker)+"�]���u"HIR"�_�j"NOR RED"�व"NOR"�v���v�T�ӷw�t���C\n", pnoun(2, attacker)+"����F�w�t�C\n");
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(HIB"nessiteras"NOR MAG" rhombopteryx"NOR, HIB"������"NOR MAG"����"NOR);

	set("unit", "��");
	set("age", 600);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 250);
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

	set("long", "�@���æb�����`�B�����ǡA��������l�M��p���Y���A�ݤW�h������������@�ت����Ͱʪ��A�ӫܹ����C�d�h�U�~�e���������j����ʪ�--�D�V�s�C");

	set("autokill", 1);

	startup_living();
	
	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
}

int remove()
{
	destruct(all_inventory(this_object()));
}
