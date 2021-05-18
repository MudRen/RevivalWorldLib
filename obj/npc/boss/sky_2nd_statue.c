/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_2nd_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-23
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
#include <buff.h>

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	10 		:	HIW"��"NOR WHT"�C"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIW"��"NOR WHT"�C"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIW"��"NOR WHT"�C"HIR"��"NOR RED"��"NOR,
	2000	:	HIW"��"NOR WHT"�C"HIM"��"NOR MAG"��"NOR,
	5000	:	HIW"��"NOR WHT"�C"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	broadcast(query_temp("location", me), this_object()->query_idname()+"���q��������M���U���F�L�ӡA���H�j�q���H�۸��U�I\n");
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
	object ob = new("/obj/inlay/sky_2nd_secret_art");
	
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, SKY_2ND_STATUE);	
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, SKY_2ND_STATUE);
}

// �����ɪ��T��
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME�|�_�e�j�ۮ���$YOU�r�O���h�A";
			break;
		case 1:
			return "$ME��q���U�ļ�$YOU�A";
			break;
		case 2:
			return "$ME��_�U���۸}��$YOU�Ҧb��m�r�O��U�A";
			break;
		case 3:
			return "$ME���ʥ��j�����u���U����$YOU�A";
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
int special_attack(object attacker, object defender)
{
	if( !random(100) )
		shout(HIB"����"NOR"-"+AREA_D->query_area_name(environment(attacker)->query_area())+"�o�ͼ@�P���ܡA�}�}�`�I��q�ǨӾѰʤj�a...�C\n");
		
	if( !random(25) )
	{
		object small_statue = new("/obj/npc/boss/small_statue");
		
		small_statue->move_to_environment(attacker);
		
		msg("\n$ME�C�n�e�p�I�ۡG�u"HIY"�h�a�I"NOR"�v�A�@�����۱q$ME���W���U�A�ۤƦ�$YOU�å[�J�԰��C\n\n", attacker, small_statue, 1);
		
		COMBAT_D->start_fight(small_statue, defender);
	}
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"sky 2nd "WHT"statue"NOR, HIW"��"NOR WHT"�C"NOR);

	set("unit", "�L");
	set("age", 650000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 350);
	set("attr/cha", 300);

	set("abi/stamina/max", 100000);
	set("abi/stamina/cur", 100000);
	set("abi/health/max", 100000);
	set("abi/health/cur", 100000);
	set("abi/energy/max", 100000);
	set("abi/energy/cur", 100000);
	set("job/name", HIW+"��������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);

	set_temp("buff/"+BUFF_FATAL_CHANCE, 10);
	set_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, 10);

	set("prestige", 1);// ���`�ᵹ�����a���n��

	set("long", "�@���j�ѯ���d�b�H�������j�۹��A�P��L��y�۹��æC�b�o���j�a�W�A�����樣�@��\n�ճ��b�۹����ݽL�ۡA������o�O�@�I�n���]�S���A���G���õۤ@�Ǥ����H�������K�C");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
