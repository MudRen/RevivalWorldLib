/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : kill.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : fight ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <npc.h>
#include <secure.h>

inherit COMMAND;

string help = @HELP
�P�H�M�@���Ԫ����O

kill '�ؼ�'		- �չϱ������
kill -all		- �չϦP�ɱ����Ҧ����䪺�ͪ�
kill -unlock		- �Ѱ��԰���w
HELP;

private void do_command(object me, string arg)
{
	object enemy;
	object *enemies = allocate(0);
	object *assisters;
	
	//if( !wizardp(me) )
	//	return tell(me, "�԰��t�δ��դ��A�Ȯɰ���ϥΦ����O�C\n");

	if( wizardp(me) && SECURE_D->level_num(me->query_id(1)) < ADMIN )
		return tell(me, "���� "+SECURE_D->level_name(me->query_id(1))+" �H�U�Ův�T��i�����԰��A�קK�v�T�C�����šC\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�M�־԰��H\n");
		
	if( me->is_npc() && query("job/type", me) != SOLDIER )
	{
		me->do_command("say �ڤ��O�x�H�A�L�k�԰�");
		return;
	}

	assisters = filter_array(present_objects(me), (: query_temp("assisting", $1) == $(me) && !COMBAT_D->in_fight($(me), $1) && !$1->is_faint() && !$1->is_dead() && $(me)->is_in_my_group($1) :));

	if( arg == "-unlock" )
	{
		if( !query("killlock", me) )
			return tell(me, pnoun(2, me)+"�å��B�b�԰���w���A�C\n");

		delete("killlock", me);
		return tell(me, pnoun(2, me)+"�N�԰���w�Ѱ��C\n");
	}

	if( query("killlock", me) )
		return tell(me, pnoun(2, me)+"�������Ѱ��԰���w(kill -unlock)�~��A�i������C\n");

	if( me->is_fatigue_full() )
	{
		if( me->is_npc() )
			return me->do_command("say �ڹ�b�O�ӯh�ҤF�A�L�k�A�i��԰��C\n");
		else
			return tell(me, pnoun(2, me)+"��b�O�ӯh�ҤF�A�L�k�A�i��԰��C\n");
	}

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( arg == "-all" )
	{
		function fp;
		
		if( BATTLEFIELD_D->in_battle(me) )
		{
			fp = 
			(: 
				$1 != $(me) && 
				!wizardp($1) &&
				$1->is_living() && 
				!$1->is_module_npc() && 
				!$1->is_dead() && 
				!query("no_fight", $1) &&
				!$(me)->is_in_my_group($1) &&
				$1->valid_enemy($(me))
			:);
		}
		else
		{
			fp = 
			(: 
				$1 != $(me) && 
				!wizardp($1) &&
				!userp($1) && 
				$1->is_living() && 
				!$1->is_module_npc() && 
				!$1->is_dead() && 
				!query("no_fight", $1) &&
				$1->valid_enemy($(me)) &&
				!(COMBAT_D->in_fight($1) && !$(me)->is_in_my_group(COMBAT_D->query_fighting_targets($1)[0]) && query("boss", COMBAT_D->query_fighting_targets($1)[0]) != $(me)->query_id(1) )
			:);
		}
		enemies = filter_array(present_objects(me), fp);

		if( sizeof(enemies) )
		foreach(enemy in enemies)
		{
			msg(HIR"$ME"HIR"���$YOU"HIR"�j�ۡG�u�h���a�I�I�v�C\n"NOR, me, enemy, 1);

			COMBAT_D->start_fight(me, enemy);
		}
		else
		{
			if( me->is_npc() )
				me->do_command("say �o�̨S������ڥi�H�i��������ؼСC\n");
			else
				return tell(me, "�o�̨S������i�H�i��������ؼСC\n");
		}

		// ���U����
		foreach(object other_assister in assisters)
		{
			if( other_assister->is_fatigue_full() || (other_assister->is_module_npc() && query("job/type", other_assister) != SOLDIER) )
				continue;

			msg(HIR"$ME"HIR"�}�l���U$YOU"HIR"�i������I�I\n"NOR, other_assister, me, 1);
	
			foreach(enemy in enemies)
			{
				if( !enemy->valid_enemy(other_assister) ) continue;
					
				COMBAT_D->start_fight(other_assister, enemy);
			}
		}

		return;
	}
	else if( wizardp(me) && arg == "-stop" )
	{
		if( !COMBAT_D->in_fight(me) )
			return tell(me, pnoun(2, me)+"�èS���b�԰����C\n");
		
		msg("$ME����԰��C\n", me, 0, 1);
		COMBAT_D->stop_fight(me);
		return;
	}

	if( !objectp(enemy = present(arg, environment(me))) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");

	if( enemy == me )
		return tell(me, pnoun(2, me)+"����M�ۤv�԰��C\n");

	if( !BATTLEFIELD_D->in_battle(enemy) && userp(enemy) )
		return tell(me, "�ثe�L�k�������a����C\n");

	if( enemy->is_module_npc() )
		return tell(me, "�ثe�L�k�������u����C\n");
		
	if( !enemy->is_living() )
		return tell(me, enemy->query_idname()+"���O�ͪ��A"+pnoun(2, me)+"�S��k�M"+pnoun(3, enemy)+"�԰��C\n");
		
	if( enemy->is_dead() )
		return tell(me, enemy->query_idname()+"�w�g���F�C\n");

	if( !enemy->valid_enemy(me) || !me->valid_enemy(enemy) )
		return tell(me, pnoun(2, me)+"�ثe�L�k�P"+enemy->query_idname()+"�i��԰��C\n");

	if( query("no_fight", enemy) )
		return tell(me, pnoun(2, me)+"�L�k��"+enemy->query_idname()+"�i������C\n");

	if( !BATTLEFIELD_D->in_battle(me) && COMBAT_D->in_fight(enemy) && !me->is_in_my_group(COMBAT_D->query_fighting_targets(enemy)[0]) && query("boss", COMBAT_D->query_fighting_targets(enemy)[0]) != me->query_id(1) )
	{
		if( me->is_npc() )
			return me->do_command("say "+enemy->query_idname()+"���b�P�L�H�i��԰��A�ڵL�k���J�C\n");
		else
			return tell(me, enemy->query_idname()+"���b�P�L�H�i��԰��A"+pnoun(2, me)+"�L�k���J�C\n");
	}

	msg(HIR"$ME"HIR"���$YOU"HIR"�j�ۡG�u�h���a�I�I�v�C\n"NOR, me, enemy, 1);
	msg(HIR"$ME"HIR"�Q�n����$YOU�I�I\n"NOR, enemy, me, 1);
	
	COMBAT_D->start_fight(me, enemy);
	
	// ��l����� 1000
	enemy->calculate_combat_hate(me, 1000);
	
	enemies = filter_array(present_objects(me), (: $1 != $(me) && $1 != $(enemy) && base_name($1) == base_name($(enemy)) && !$1->is_faint() && !$1->is_dead() :));

	// �������
	foreach(object other_enemy in enemies)
	{
		msg(HIR"$ME"HIR"�Q�n����$YOU�I�I\n"NOR, other_enemy, me, 1);
		COMBAT_D->start_fight(other_enemy, me);
		enemies |= ({ other_enemy });
	}
	
	enemies |= ({ enemy });

	// ���U����
	foreach(object other_assister in assisters)
	{
		if( other_assister->is_fatigue_full() || (other_assister->is_module_npc() && query("job/type", other_assister) != SOLDIER) )
			continue;

		msg(HIR"$ME"HIR"�}�l���U$YOU"HIR"�i������I�I\n"NOR, other_assister, me, 1);

		foreach(enemy in enemies)
		{
			if( !enemy->valid_enemy(other_assister) ) continue;
				
			COMBAT_D->start_fight(other_assister, enemy);
		}
	}
}
