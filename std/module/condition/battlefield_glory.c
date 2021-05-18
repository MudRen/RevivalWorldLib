/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_glory.c
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
#include <delay.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "battlefield-glory";
string name 	= HIW"重生的"NOR WHT"戰爭榮耀"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 120;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 100, ob);
	set(query_key()+"/"+BUFF_PHY, 100, ob);
	set(query_key()+"/"+BUFF_AGI, 100, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 1000, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 1000, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 3000, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 3000, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 3000, ob);

	::start_effect(ob);

	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"："+ob->query_idname()+"獲得「"+query_condition_name()+"」加持，持續 "+(query_default_condition_time()/60)+" 分鐘，目標座標為"+loc_short(query_temp("location", ob))+"，擊敗此人可獲得大量戰績");
	ob->start_delay(BATTLE_GLORY_DELAY_KEY, 6, pnoun(2, ob)+"因為「"+query_condition_name()+"」的"+query_condition_type()+"，目前無法做任何動作");
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