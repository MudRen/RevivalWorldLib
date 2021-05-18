/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : steamed_roll_heal.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>
#include <daemon.h>

inherit CONDITION_MOD;

string id 	= "steamed_roll_heal";
string name 	= HIW"飽暖"WHT"舒暢"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 60;
int heartbeat 	= 2;

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
	int effect;
	
	if( !objectp(ob) || ob->is_faint() || ob->is_dead() ) return;
	
	effect = query_temp("steamed_roll_heal_effect", ob);
	
	msg("$ME受到「"+query_condition_name()+"」的治療，恢復生命值("HIR+effect+NOR")。\n", ob, 0, 1);
	
	ob->earn_health(effect);
}

