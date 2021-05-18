/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : poison_giant_spider.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>
#include <daemon.h>
#include <combat.h>

inherit CONDITION_MOD;

string id 	= "poison_giant_spider";
string name 	= NOR HIW"蜘蛛"WHT"毒害"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 	= 60;
int heartbeat 	= 10;

// 啟動狀態時的效果
void start_effect(object ob)
{
	::start_effect(ob);
}

// 結束狀態時的效果
void stop_effect(object ob)
{
	::stop_effect(ob);
}

// 狀態進行中的效果
void heart_beat_effect(object ob)
{
	object caster = query(query_key()+"/caster", ob);
	
	if( !objectp(ob) || ob->is_faint() || ob->is_dead() || !objectp(caster) ) return;

	msg("$ME受到「"+query_condition_name()+"」的侵蝕，造成不小的傷害。\n", ob, 0, 1);
	
	COMBAT_D->cause_damage(caster, ob, 50, NO_FATALBLOW | NO_ATTACKDEFEND);
}