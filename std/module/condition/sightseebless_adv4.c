/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv4.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "sightseebless-adv4";
string name 	= HIW"叱吒"NOR WHT"風雲"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 7200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 7, ob);
	set(query_key()+"/"+BUFF_PHY, 7, ob);
	set(query_key()+"/"+BUFF_INT, 7, ob);
	set(query_key()+"/"+BUFF_CHA, 7, ob);
	set(query_key()+"/"+BUFF_AGI, 7, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 150, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 150, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 15, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 15, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 150, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 150, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 150, ob);

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