/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_1st_statue.c
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
	10 		:	HIW"��"NOR WHT"�V"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIW"��"NOR WHT"�V"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIW"��"NOR WHT"�V"HIR"��"NOR RED"��"NOR,
	2000	:	HIW"��"NOR WHT"�V"HIM"��"NOR MAG"��"NOR,
	5000	:	HIW"��"NOR WHT"�V"WHT"��"HIW"��"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	broadcast(query_temp("location", me), this_object()->query_idname()+"���q��������M���U���F�L�ӡA�o�X�F�����T�æ��H�j�q���H�۸��U�I\n");
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
	object ob = new("/obj/inlay/sky_1st_secret_art");
	
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, SKY_1ST_STATUE);
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, SKY_1ST_STATUE);
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

void stop_angry(object attacker, object defender)
{
	if( !objectp(attacker) ) return;

	delete_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, attacker);
	delete_temp("buff/"+BUFF_COMBO_CHANCE, attacker);
	set_temp("buff/"+BUFF_FATAL_CHANCE, 10, attacker);

	attacker->reset_buff_cache();
	
	msg("\n$ME�����㺥�������U��...\n", attacker, defender, 1);
	msg("$ME�����㺥�������U��...\n", attacker, defender, 1);
	msg("$ME�����㺥�������U��...\n\n", attacker, defender, 1);
}

void start_angry(object attacker, object defender)
{
	if( !objectp(attacker) ) return;

	msg("\n$ME"HIY"���e�j���v�}�l�ҽk�_�ӡI�I�I�I\n"NOR, attacker, defender, 1);
	msg("$ME"HIY"���e�j���v�}�l�ҽk�_�ӡI�I�I�I\n"NOR, attacker, defender, 1);
	msg("$ME"HIY"���e�j���v�}�l�ҽk�_�ӡI�I�I�I\n\n"NOR, attacker, defender, 1);

	set_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, 100, attacker);
	set_temp("buff/"+BUFF_COMBO_CHANCE, 40, attacker);
	set_temp("buff/"+BUFF_FATAL_CHANCE, 50, attacker);

	attacker->reset_buff_cache();
	
	call_out( (: stop_angry :), 30, attacker, defender );
}

void prepare_angry(object attacker, object defender)
{
	msg("\n$ME�o�X��Ѽ@�q�G�u"HIY"�Z�H�����I�I�I"NOR"�v\n", attacker, defender, 1);
	msg("$ME�o�X��Ѽ@�q�G�u"HIY"�Z�H�����I�I�I"NOR"�v\n", attacker, defender, 1);
	msg("$ME�o�X��Ѽ@�q�G�u"HIY"�Z�H�����I�I�I"NOR"�v\n", attacker, defender, 1);
	
	msg(HIG+"\n�o�n�@�q���Hı�o���G�j�Ƥ����I�I\n"NOR, attacker, 0, 1);
	msg(HIG+"�o�n�@�q���Hı�o���G�j�Ƥ����I�I\n"NOR, attacker, 0, 1);
	msg(HIG+"�o�n�@�q���Hı�o���G�j�Ƥ����I�I\n\n"NOR, attacker, 0, 1);
	
	call_out( (: start_angry :), 5, attacker, defender );
}

// �S�����
void special_attack(object attacker, object defender)
{
	int health;
	int damage;

	if( !random(100) )
		shout(HIB"����"NOR"-"+AREA_D->query_area_name(environment(attacker)->query_area())+"�o�ͼ@�P���ܡA�}�}�`�I��q�ǨӾѰʤj�a...�C\n");

	if( !random(10) )
	{
		damage = range_random(200, 500);

		msg("$ME�ϥX�u"HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR"�v�A�j�q���۩�$YOU���Y�L�h�I�I\n"NOR, attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
	}

	health = query("abi/health/cur");
	
	if( health > 50000 )
	{
		delete_temp("healtimes");
		delete_temp("angry");
	}
	
	if( health < 30000 && addn_temp("healtimes", 1) <= 3 )
	{
		msg("\n�@�}�ѷn�a�ʡA�@�D"HIW"�ե�"NOR"�ۨ�L��y���j�۹����g�L�ӡA$ME�ͩR������_����("NOR RED"+"HIR"3333"NOR")�C\n", attacker, 0, 1);
		msg("�@�}�ѷn�a�ʡA�@�D"HIW"�ե�"NOR"�ۨ�L��y���j�۹����g�L�ӡA$ME�ͩR������_����("NOR RED"+"HIR"3333"NOR")�C\n", attacker, 0, 1);	
		msg("�@�}�ѷn�a�ʡA�@�D"HIW"�ե�"NOR"�ۨ�L��y���j�۹����g�L�ӡA$ME�ͩR������_����("NOR RED"+"HIR"3333"NOR")�C\n\n", attacker, 0, 1);	

		addn("abi/health/cur", 9999);
	}

	if( health < 15000 && addn_temp("angry", 1) <= 1 )
	{
		prepare_angry(attacker, defender);
	}
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"sky 1st "WHT"statue"NOR, HIW"��"NOR WHT"�V"NOR);

	set("unit", "�L");
	set("age", 650000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 300);
	set("attr/cha", 300);

	set("abi/stamina/max", 60000);
	set("abi/stamina/cur", 60000);
	set("abi/health/max", 60000);
	set("abi/health/cur", 60000);
	set("abi/energy/max", 60000);
	set("abi/energy/cur", 60000);
	set("job/name", HIW+"��������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);

	set_temp("buff/"+BUFF_FATAL_CHANCE, 10);
	set_temp("buff/"+BUFF_FATAL_POWER, 20);

	set("prestige", 1);// ���`�ᵹ�����a���n��

	set("long", "�@���j�ѯ���d�b�H�������j�۹��A�P��L��y�۹��æC�b�o���j�a�W�A�����樣�@��\n�ճ��b�۹����ݽL�ۡA������o�O�@�I�n���]�S���A���G���õۤ@�Ǥ����H�������K�C");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
