/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_fear_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-16
 * Note   : �Ԫ����ߥ۸O
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
		return tell(me, this_object()->query_idname()+"���j�j��O�w�g�Q�ϥΡC\n");

	players = BATTLEFIELD_D->get_sorted_players();
	
	for(int i=0;i<sizeof(players);++i)
	{
		target = find_player(players[i]);
		
		if( objectp(target) && BATTLEFIELD_D->in_battle(target) )
			break;
	}
	
	BATTLEFIELD_D->add_bonus_score(me, 40);
	tell(me, pnoun(2, me)+"�]���IĲ"+this_object()->query_idname()+"����o "HIY"40"NOR" �I���Z���Q�C\n");

	target->start_condition(BATTLEFIELD_FEAR);
	
	call_out((: delay_destruct :), 0);
	
	set_temp("used", 1);
}

void create()
{
	set_idname(HIR"battlefield "NOR RED"fear statue"NOR, HIR"���ͪ�"NOR RED"�Ԫ����ߥ۸O"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�ϥγo�ӥ۸O���H�i�H�j�T���C�t�@��Գ������a����O�C");
	set("unit", "��");
	set("capacity", 0);
	set("mass", -1);
	set("value", 100000000);
	set("badsell", 1);
}
