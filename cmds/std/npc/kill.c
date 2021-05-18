/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : kill.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : fight 指令
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
與人決一死戰的指令

kill '目標'		- 試圖殺死對方
kill -all		- 試圖同時殺死所有身邊的生物
kill -unlock		- 解除戰鬥鎖定
HELP;

private void do_command(object me, string arg)
{
	object enemy;
	object *enemies = allocate(0);
	object *assisters;
	
	//if( !wizardp(me) )
	//	return tell(me, "戰鬥系統測試中，暫時停止使用此指令。\n");

	if( wizardp(me) && SECURE_D->level_num(me->query_id(1)) < ADMIN )
		return tell(me, "等級 "+SECURE_D->level_name(me->query_id(1))+" 以下巫師禁止進行任何戰鬥，避免影響遊戲平衡。\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"想要和誰戰鬥？\n");
		
	if( me->is_npc() && query("job/type", me) != SOLDIER )
	{
		me->do_command("say 我不是軍人，無法戰鬥");
		return;
	}

	assisters = filter_array(present_objects(me), (: query_temp("assisting", $1) == $(me) && !COMBAT_D->in_fight($(me), $1) && !$1->is_faint() && !$1->is_dead() && $(me)->is_in_my_group($1) :));

	if( arg == "-unlock" )
	{
		if( !query("killlock", me) )
			return tell(me, pnoun(2, me)+"並未處在戰鬥鎖定狀態。\n");

		delete("killlock", me);
		return tell(me, pnoun(2, me)+"將戰鬥鎖定解除。\n");
	}

	if( query("killlock", me) )
		return tell(me, pnoun(2, me)+"必須先解除戰鬥鎖定(kill -unlock)才能再進行攻擊。\n");

	if( me->is_fatigue_full() )
	{
		if( me->is_npc() )
			return me->do_command("say 我實在是太疲勞了，無法再進行戰鬥。\n");
		else
			return tell(me, pnoun(2, me)+"實在是太疲勞了，無法再進行戰鬥。\n");
	}

	//忙碌中不能下指令
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
			msg(HIR"$ME"HIR"對著$YOU"HIR"大喊：「去死吧！！」。\n"NOR, me, enemy, 1);

			COMBAT_D->start_fight(me, enemy);
		}
		else
		{
			if( me->is_npc() )
				me->do_command("say 這裡沒有任何我可以進行攻擊的目標。\n");
			else
				return tell(me, "這裡沒有任何可以進行攻擊的目標。\n");
		}

		// 輔助攻擊
		foreach(object other_assister in assisters)
		{
			if( other_assister->is_fatigue_full() || (other_assister->is_module_npc() && query("job/type", other_assister) != SOLDIER) )
				continue;

			msg(HIR"$ME"HIR"開始輔助$YOU"HIR"進行攻擊！！\n"NOR, other_assister, me, 1);
	
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
			return tell(me, pnoun(2, me)+"並沒有在戰鬥中。\n");
		
		msg("$ME停止戰鬥。\n", me, 0, 1);
		COMBAT_D->stop_fight(me);
		return;
	}

	if( !objectp(enemy = present(arg, environment(me))) )
		return tell(me, "這裡沒有 "+arg+" 這個人。\n");

	if( enemy == me )
		return tell(me, pnoun(2, me)+"不能和自己戰鬥。\n");

	if( !BATTLEFIELD_D->in_battle(enemy) && userp(enemy) )
		return tell(me, "目前無法攻擊玩家角色。\n");

	if( enemy->is_module_npc() )
		return tell(me, "目前無法攻擊員工角色。\n");
		
	if( !enemy->is_living() )
		return tell(me, enemy->query_idname()+"不是生物，"+pnoun(2, me)+"沒辦法和"+pnoun(3, enemy)+"戰鬥。\n");
		
	if( enemy->is_dead() )
		return tell(me, enemy->query_idname()+"已經死了。\n");

	if( !enemy->valid_enemy(me) || !me->valid_enemy(enemy) )
		return tell(me, pnoun(2, me)+"目前無法與"+enemy->query_idname()+"進行戰鬥。\n");

	if( query("no_fight", enemy) )
		return tell(me, pnoun(2, me)+"無法對"+enemy->query_idname()+"進行攻擊。\n");

	if( !BATTLEFIELD_D->in_battle(me) && COMBAT_D->in_fight(enemy) && !me->is_in_my_group(COMBAT_D->query_fighting_targets(enemy)[0]) && query("boss", COMBAT_D->query_fighting_targets(enemy)[0]) != me->query_id(1) )
	{
		if( me->is_npc() )
			return me->do_command("say "+enemy->query_idname()+"正在與他人進行戰鬥，我無法介入。\n");
		else
			return tell(me, enemy->query_idname()+"正在與他人進行戰鬥，"+pnoun(2, me)+"無法介入。\n");
	}

	msg(HIR"$ME"HIR"對著$YOU"HIR"大喊：「去死吧！！」。\n"NOR, me, enemy, 1);
	msg(HIR"$ME"HIR"想要殺死$YOU！！\n"NOR, enemy, me, 1);
	
	COMBAT_D->start_fight(me, enemy);
	
	// 初始仇恨值 1000
	enemy->calculate_combat_hate(me, 1000);
	
	enemies = filter_array(present_objects(me), (: $1 != $(me) && $1 != $(enemy) && base_name($1) == base_name($(enemy)) && !$1->is_faint() && !$1->is_dead() :));

	// 集體攻擊
	foreach(object other_enemy in enemies)
	{
		msg(HIR"$ME"HIR"想要殺死$YOU！！\n"NOR, other_enemy, me, 1);
		COMBAT_D->start_fight(other_enemy, me);
		enemies |= ({ other_enemy });
	}
	
	enemies |= ({ enemy });

	// 輔助攻擊
	foreach(object other_assister in assisters)
	{
		if( other_assister->is_fatigue_full() || (other_assister->is_module_npc() && query("job/type", other_assister) != SOLDIER) )
			continue;

		msg(HIR"$ME"HIR"開始輔助$YOU"HIR"進行攻擊！！\n"NOR, other_assister, me, 1);

		foreach(enemy in enemies)
		{
			if( !enemy->valid_enemy(other_assister) ) continue;
				
			COMBAT_D->start_fight(other_assister, enemy);
		}
	}
}
