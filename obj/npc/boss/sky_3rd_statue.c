/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_3rd_statue.c
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
	10 		:	HIW"��"NOR WHT"��"HIY"�y"NOR YEL"�H"NOR,
	100		:	HIW"��"NOR WHT"��"HIB"�g"NOR BLU"�P"NOR,
	1000	:	HIW"��"NOR WHT"��"HIR"��"NOR RED"��"NOR,
	2000	:	HIW"��"NOR WHT"��"HIM"��"NOR MAG"��"NOR,
	5000	:	HIW"��"NOR WHT"��"WHT"��"HIW"��"NOR,
]);

private mapping fightart = allocate_mapping(0);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void stop_stun(object attacker, object defender)
{
	if( !objectp(attacker) || !objectp(defender) || !COMBAT_D->in_fight(attacker, defender) )
		return;
		
	COMBAT_D->stop_fight(attacker, defender);
	msg("\n$ME�ש�^���L�ӡA����F��$YOU������...\n\n"NOR, attacker, defender, 1);
	
	map_delete(fightart, attacker);
	map_delete(fightart, 0);
}

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
	object ob = new("/obj/inlay/sky_3rd_secret_art");
	
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
	
	foreach(object attacker, object *defenders in fightart)
	{
		if( !objectp(ob) || !arrayp(defenders) ) continue;	
		
		foreach(object defender in defenders)
		{
			if( !objectp(defender) ) continue;
				
			stop_stun(attacker, defender);
		}
	}
}

// �Q���Ѯɪ��ƥ�
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, SKY_3RD_STATUE);	
	::reply_defeat(attackers);
}

// �ˬd�O�_���X�k�԰��ؼ�
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, SKY_3RD_STATUE);
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

// �S������
void special_attack(object attacker, object defender)
{
	if( !random(100) )
		shout(HIB"����"NOR"-"+AREA_D->query_area_name(environment(attacker)->query_area())+"�o�ͼ@�P���ܡA�}�}�`�I��q�ǨӾѰʤj�a...�C\n");
		
	if( !random(15) )
	{
		msg("\n$ME����@���A�j�q�u"WHT"�Q�¸H��"NOR"�v�ۦa�U«�X���ۦb$YOU���W�A$YOU�y�������o�C�A"HIG"���r�I�I\n\n"NOR, attacker, defender, 1);
		
		defender->start_condition(MAGNETIC_STONE, 60, 1, attacker);

		return;
	}

	if( !random(30) )
	{
		object *enemies = COMBAT_D->query_fighting_targets(attacker);
		object enemy;
		
		if( sizeof(enemies) > 0 )
		{		
			msg("\n$ME�¤Ѥj�q�G�u"HIY"�������q�I�I�I"NOR"�v�A�E�M�_�i���e$YOU�դ��A$YOU�Q�_�o�Y�����ȡI�I\n"NOR, attacker, defender, 1);
			msg("$ME�¤Ѥj�q�G�u"HIY"�������q�I�I�I"NOR"�v�A�E�M�_�i���e$YOU�դ��A$YOU�Q�_�o�Y�����ȡI�I\n"NOR, attacker, defender, 1);	
			msg("$ME�¤Ѥj�q�G�u"HIY"�������q�I�I�I"NOR"�v�A�E�M�_�i���e$YOU�դ��A$YOU�Q�_�o�Y�����ȡI�I\n"NOR, attacker, defender, 1);	

			enemy = enemies[random(sizeof(enemies))];
			
			msg("$ME"HIR"�����I�I����M�}�l��$YOU"HIR"�i������I�I\n\n"NOR, defender, enemy, 1);	
			
			COMBAT_D->start_fight(defender, enemy);
			defender->start_delay("sky_3rd_statue", 1, pnoun(2, defender)+"�ثe�Y������A�L�k������ʧ@");
			
			if( !arrayp(fightart[defender]) )
				fightart[defender] = ({ enemy });
			else
				fightart[defender] += ({ enemy });

			call_out( (: stop_stun :), range_random(2, 5), defender, enemy );
		}
	}
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"sky 3rd "WHT"statue"NOR, HIW"��"NOR WHT"��"NOR);

	set("unit", "�L");
	set("age", 650000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 350);
	set("attr/int", 300);
	set("attr/agi", 300);
	set("attr/cha", 300);

	set("abi/stamina/max", 80000);
	set("abi/stamina/cur", 80000);
	set("abi/health/max", 80000);
	set("abi/health/cur", 80000);
	set("abi/energy/max", 80000);
	set("abi/energy/cur", 80000);
	set("job/name", HIW+"��������"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);

	set_temp("buff/"+BUFF_FATAL_CHANCE, 10);
	set_temp("buff/"+BUFF_ANTIFATAL_CHANCE, 20);
	
	set("prestige", 1);// ���`�ᵹ�����a���n��

	set("long", "�@���j�ѯ���d�b�H�������j�۹��A�P��L��y�۹��æC�b�o���j�a�W�A�����樣�@��\n�ճ��b�۹����ݽL�ۡA������o�O�@�I�n���]�S���A���G���õۤ@�Ǥ����H�������K�C");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}