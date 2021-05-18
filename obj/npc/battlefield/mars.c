/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mars.c
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

void do_command(string cmd);

mapping defeat_titles =
([
	1 		:	HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR,
	20		:	HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR,
	100		:	HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR,
	500		:	HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR,
	1000	:	HIW"��"NOR WHT"��"HIW"�~"NOR WHT"��"HIW"��"NOR,
]);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("shout");
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
	destruct(all_inventory(this_object()));
		
	if( !random(30) )
	{
		object ob = new("/obj/weapon/truth_axe");
		ob->move(this_object());
	}
	
	CHANNEL_D->channel_broadcast("combat", this_object()->query_idname()+"�G�A�̳o�ǷM�����H�����n�H���o�˴N�i�H���ѧ�...");
		
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
			return "$ME�������b���򬽬��a��$YOU�A�ӡA";
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
	if( !random(50) )
	{
		if( defender->in_condition(INVINCIBILITY) )
			defender->stop_condition(INVINCIBILITY);
	}
}

void equip_special_weapon()
{
	string id;
	object ob;
	string *weapons = allocate(0);
	
	weapons |= map(get_dir("/product/fist/"), (: "/product/fist/"+$1[0..<3] :));
	weapons |= map(get_dir("/product/stick/"), (: "/product/stick/"+$1[0..<3] :));
	weapons |= map(get_dir("/product/blade/"), (: "/product/blade/"+$1[0..<3] :));	
	
	ob = new(weapons[random(sizeof(weapons))]);
	
	id = ob->query_id(1);
	
	ob->move(this_object());
	do_command("equip "+id);
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"ma"NOR WHT"rs"NOR, HIW"��"NOR WHT"��"NOR);

	set("unit", "��");
	set("age", range_random(25,35));
	set("gender", MALE_GENDER);
	
	set("attr/str", 1600);
	set("attr/phy", 1600);
	set("attr/int", 1600);
	set("attr/agi", 1600);
	set("attr/cha", 1600);

	set("abi/stamina/max", 150000);
	set("abi/stamina/cur", 150000);
	
	set("abi/health/max", 150000);
	set("abi/health/cur", 150000);
	
	set("abi/energy/max", 150000);
	set("abi/energy/cur", 150000);

	set("skills/unarmed/level", 100);
	set("skills/dodge/level", 100);

	set("skills/attack-stance-adv/level", 100);
	set("skills/defend-stance-adv/level", 100);
	set("skills/speed-stance-adv/level", 100);
	
	set("job/name", HIW+"��������"+NOR);
	
	set("combat/stance/type", ({ "attack-adv", "defend-adv", "speed-adv" })[random(3)]);
	
	equip_special_weapon();

	set("long",@TEXT
�@�@���~�b�Գ������U�ॢ�ͩR���Ԥh��z�Ҿ��E�������ԯ��C
TEXT);

	startup_living();
}
