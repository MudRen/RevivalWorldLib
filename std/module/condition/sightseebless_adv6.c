/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv6.c
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

string id 	= "sightseebless-adv6";
string name 	= HIW"寰"NOR WHT"宇"HIW"大"NOR WHT"觀"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 7200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 15, ob);
	set(query_key()+"/"+BUFF_PHY, 15, ob);
	set(query_key()+"/"+BUFF_INT, 15, ob);
	set(query_key()+"/"+BUFF_CHA, 15, ob);
	set(query_key()+"/"+BUFF_AGI, 15, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 400, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 400, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 400, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 400, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 400, ob);

	set(query_key()+"/"+BUFF_FATAL_POWER, 1, ob);
	set(query_key()+"/"+BUFF_FATAL_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_ANTIFATAL_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_COUNTERATTACK_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_THORN_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_COMBO_CHANCE, 1, ob);
	
	set(query_key()+"/"+BUFF_STAMINA_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_HEALTH_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_ENERGY_REGEN, 5, ob);

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