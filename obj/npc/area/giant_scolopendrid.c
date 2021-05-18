/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : giant_scolopendrid.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-15
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

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	500 	:	"���G"HIY"�y"NOR YEL"�H"NOR,
	5000	:	"���G"HIB"�g"NOR BLU"�P"NOR,
	50000	:	"���G"HIR"��"NOR RED"��"NOR,
	200000	:	"���G"HIM"��"NOR MAG"��"NOR,
	500000	:	"���G"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;

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
	TREASURE_D->get_treasure(this_object());

	if( !random(100000/COMBAT_D->total_value(this_object())) )
	{
		object ob;
		string* random_drop =
		({
			"/obj/weapon/coarse_fist",
			"/obj/weapon/coarse_blade",
			"/obj/weapon/coarse_stick",
			"/obj/armor/coarse_breast_plate",
			"/obj/armor/coarse_leggings",
			"/obj/armor/coarse_helm",
			"/obj/armor/coarse_gloves",
			"/obj/armor/coarse_cloak",			
			"/obj/armor/coarse_bracers",
			"/obj/armor/coarse_boots",
			"/obj/armor/coarse_belt",
		});
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	}
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	int probability = 3200;
	
	foreach(object attacker in attackers)
	{
		if( !objectp(attacker) ) continue;
			
		switch(query("combat/defeat/"+replace_string(base_name(this_object()), "/", "#"), attacker))
		{
			case 500..4999: probability = 3000; break;
			case 5000..49999:  probability = 2800; break;
			case 50000..199999: probability = 2600; break;
			case 200000..499999: probability = 2400; break;
			case 500000..MAX_INT: probability = 2200; break;
			default: probability = 3200; break;
		}
		
		break;
	}
	
	if( !random(probability) )
	{
		object ob = new("/quest/monster_fossil/scolopendrid_fossil");
		ob->move(this_object());
	}
	
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
			return "$ME��$YOU��V�Ĩ�@�f�r�ӡA";
			break;
		case 1:
			return "$ME�ֳt«�����䪺�B�����ñq�t�@�Ӥ�V��$YOU���y���r�ӡA";
			break;
		case 2:
			return "$ME�Q�Φh���}�N$YOU�x��i������A";
			break;
		case 3:
			return "$ME�q�����ֳt����$YOU�A";
			break;
		case 4:
			return "$ME�N����j�ۦ�$YOU����i������A";
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
	if( !random(20) )
	{
		msg("$ME��M�Q�X�@�}�����u"HIY"�P��"NOR YEL"����"NOR"�v�A$YOU���ֽ��P��@�}��h�I�I�I\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, range_random(50, 100));
	}
}

void create()
{
	set_idname("giant scolopendrid", "�����G");

	set("unit", "��");
	set("age", 4);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 50);
	set("attr/phy", 50);
	set("attr/int", 50);
	set("attr/agi", 50);
	set("attr/cha", 50);

	set("abi/stamina/max", 500);
	set("abi/stamina/cur", 500);
	set("abi/health/max", 500);
	set("abi/health/cur", 500);
	set("abi/energy/max", 500);
	set("abi/energy/cur", 500);

	set("skills/unarmed/level", 50);
	set("skills/dodge/level", 50);

	set("long", "�@�@�����j���G�A�o�򥨤j�����ߨϤH�򰩮��M�C");

	if( !random(1000) )
	{
		set_idname(HIY"giant"NOR YEL" scolopendrid"NOR, HIY"��"NOR YEL"���G"NOR);
		
		MAPNPC_D->become_named_npc(this_object());
	}
	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
