/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_glory_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-16
 * Note   : 戰爭榮耀石碑
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

void delay_destruct()
{
	destruct(this_object());
}

void init(object ob)
{
	if( !userp(ob) )
		return;

	if( !BATTLEFIELD_D->in_battle(ob) )
		return;

	if( query_temp("used") )
		return tell(ob, this_object()->query_idname()+"的強大能力已經被吸收。\n");
	
	ob->start_condition(BATTLEFIELD_GLORY);
	BATTLEFIELD_D->add_bonus_score(ob, 40);
	tell(ob, pnoun(2, ob)+"因為碰觸"+this_object()->query_idname()+"而獲得 "HIY"40"NOR" 點戰績紅利。\n");

	call_out((: delay_destruct :), 0);
	
	set_temp("used", 1);
}

void create()
{
	set_idname(HIW"battlefield "NOR WHT"glory statue"NOR, HIW"重生的"NOR WHT"戰爭榮耀石碑"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　第一個看到這個石碑的人可以獲得極強大的能力。");
	set("unit", "個");
	set("capacity", 0);
	set("mass", -1);
	set("value", 100000000);
	set("badsell", 1);
}
