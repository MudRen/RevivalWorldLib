/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : minotaur.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-08-18
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
#include <autokill.h>

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	500 		:	HIY"�g"NOR YEL"�Ԥh"HIY"�y"NOR YEL"�H"NOR,
	5000		:	HIY"�g"NOR YEL"�Ԥh"HIB"�g"NOR BLU"�P"NOR,
	50000		:	HIY"�g"NOR YEL"�Ԥh"HIR"��"NOR RED"��"NOR,
	100000		:	HIY"�g"NOR YEL"�Ԥh"HIM"��"NOR MAG"��"NOR,
	250000		:	HIY"�g"NOR YEL"�Ԥh"WHT"��"HIW"��"NOR,
]);

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
	TREASURE_D->get_treasure(this_object());
	BATTLEFIELD_D->add_npc(base_name(this_object()));
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
			return "$ME���ۿ��򬽬��a��$YOU�A�ӡA";
			break;
		case 1:
			return "$ME��$YOU���I��ļ��L�ӡA";
			break;
		case 2:
			return "$ME��q�@�n�Τ�y�r��$YOU���Y���A";
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
	

}

int modified_value(int value)
{
	int new_value = value - 30;

	if( new_value < 5 )
		new_value = 5;
		
	return new_value;
}
	
void init(object ob)
{
	int temp;

	if( COMBAT_D->in_fight(this_object()) )
		return;

	set("attr/str", modified_value(ob->query_str()));
	set("attr/phy", modified_value(ob->query_phy()));
	set("attr/int", modified_value(ob->query_int()));
	set("attr/agi", modified_value(ob->query_agi()));
	set("attr/cha", modified_value(ob->query_cha()));

	temp = to_int(pow(to_float(ob->query_stamina_max()), 0.93)) - 300;
	if( temp < 100 ) temp = 100;
	set("abi/stamina/max", temp);
	set("abi/stamina/cur", temp);

	temp = to_int(pow(to_float(ob->query_health_max()), 0.93)) - 300;
	if( temp < 100 ) temp = 100;
	set("abi/health/max", temp);
	set("abi/health/cur", temp);

	temp = to_int(pow(to_float(ob->query_energy_max()), 0.93)) - 300;
	if( temp < 100 ) temp = 100;
	set("abi/energy/max", temp);
	set("abi/energy/cur", temp);

	set("skills/unarmed/level", ob->query_skill_level("unarmed"));
	set("skills/dodge/level", ob->query_skill_level("dodge"));
}

void create()
{
	int temp;

	set_idname("berserker", "�g�Ԥh");

	set("unit", "��");
	set("age", range_random(25,35));
	set("gender", MALE_GENDER);
	
	set("attr/str", range_random(30,80));
	set("attr/phy", range_random(30,80));
	set("attr/int", range_random(30,80));
	set("attr/agi", range_random(30,80));
	set("attr/cha", range_random(30,80));

	temp = range_random(1000, 3000);
	set("abi/stamina/max", temp);
	set("abi/stamina/cur", temp);
	temp = range_random(1000, 3000);
	set("abi/health/max", temp);
	set("abi/health/cur", temp);
	temp = range_random(1000, 3000);
	set("abi/energy/max", temp);
	set("abi/energy/cur", temp);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);

	set("long",@TEXT
�@�@�O�j�֫p���g�Ԥh�A�⮳����A�~���r�ªZ�C
TEXT);

	set("autokill", AUTOKILL_IMMEDIATELY);

	startup_living();
}
