/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : medicated_wine.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-26
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "medicated_wine";
string name 	= HIR"舒筋"NOR RED"活血"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 300;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_PHY, 2, ob);
	set(query_key()+"/"+BUFF_HEALTH_REGEN, 10, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 100, ob);

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