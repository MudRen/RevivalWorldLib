/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_fear_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-16
 * Note   : 戰爭恐懼石碑
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <feature.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

mapping actions;

void delay_destruct()
{
	destruct(this_object());
}

void init(object me)
{
	object target;
	string* players;
	
	if( !BATTLEFIELD_D->in_battle(me) )
		return;

	if( query_temp("used") )
		return tell(me, this_object()->query_idname()+"的強大能力已經被使用。\n");

	players = BATTLEFIELD_D->get_sorted_players();
	
	for(int i=0;i<sizeof(players);++i)
	{
		target = find_player(players[i]);
		
		if( objectp(target) && BATTLEFIELD_D->in_battle(target) )
			break;
	}
	
	BATTLEFIELD_D->add_bonus_score(me, 40);
	tell(me, pnoun(2, me)+"因為碰觸"+this_object()->query_idname()+"而獲得 "HIY"40"NOR" 點戰績紅利。\n");

	target->start_condition(BATTLEFIELD_FEAR);
	
	call_out((: delay_destruct :), 0);
	
	set_temp("used", 1);
}

void create()
{
	set_idname(HIR"battlefield "NOR RED"fear statue"NOR, HIR"重生的"NOR RED"戰爭恐懼石碑"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　使用這個石碑的人可以大幅降低另一位戰場中玩家的能力。");
	set("unit", "個");
	set("capacity", 0);
	set("mass", -1);
	set("value", 100000000);
	set("badsell", 1);
}
