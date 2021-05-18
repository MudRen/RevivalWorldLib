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

string id 	= "sightseebless-adv7";
string name 	= HIW"瀚"NOR WHT"海"HIW"皓"NOR WHT"月"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 7200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 20, ob);
	set(query_key()+"/"+BUFF_PHY, 20, ob);
	set(query_key()+"/"+BUFF_INT, 20, ob);
	set(query_key()+"/"+BUFF_CHA, 20, ob);
	set(query_key()+"/"+BUFF_AGI, 20, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 500, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 500, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 500, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 500, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 500, ob);

	set(query_key()+"/"+BUFF_FATAL_POWER, 2, ob);
	set(query_key()+"/"+BUFF_FATAL_CHANCE, 2, ob);
	set(query_key()+"/"+BUFF_ANTIFATAL_CHANCE, 2, ob);
	set(query_key()+"/"+BUFF_COUNTERATTACK_CHANCE, 2, ob);
	set(query_key()+"/"+BUFF_THORN_CHANCE, 2, ob);
	set(query_key()+"/"+BUFF_COMBO_CHANCE, 2, ob);
	
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