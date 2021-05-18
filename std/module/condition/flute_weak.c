/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : flute_weak.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <daemon.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "flute_weak";
string name 	= HIR"震"NOR RED"懾"HIR"幻"NOR RED"音"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 		= 60;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	addn(query_key()+"/"+DEFEND, -COMBAT_D->defend_value(ob, 1)/25, ob);

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


}

int stackable()
{
	return 5;
}
