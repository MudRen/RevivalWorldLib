/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_fear.c
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

string id 		= "battlefield-fear";
string name 	= HIR"重生的"NOR RED"戰爭恐懼"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 		= 120;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, -ob->query_str(1)/2, ob);
	set(query_key()+"/"+BUFF_PHY, -ob->query_phy(1)/2, ob);
	set(query_key()+"/"+BUFF_AGI, -ob->query_agi(1)/2, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, -ob->query_stamina_max(1)/2, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, -ob->query_health_max(1)/2, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, -ob->query_energy_max(1)/2, ob);

	::start_effect(ob);

	CHANNEL_D->channel_broadcast("combat", HIR"戰"NOR RED"爭"NOR"："+ob->query_idname()+"遭到「"+query_condition_name()+"」"+query_condition_type()+"，持續 "+(query_default_condition_time()/60)+" 分鐘，目標座標為"+loc_short(query_temp("location", ob))+"。");
	ob->start_delay(BATTLE_FEAR_DELAY_KEY, 12, pnoun(2, ob)+"因為「"+query_condition_name()+"」的"+query_condition_type()+"，目前無法做任何動作");
	
	if( ob->query_stamina_cur() > ob->query_stamina_max() )
		ob->set_stamina_full();
	if( ob->query_health_cur() > ob->query_health_max() )
		ob->set_health_full();
	if( ob->query_energy_cur() > ob->query_energy_max() )
		ob->set_energy_full();
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