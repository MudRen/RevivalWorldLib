/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv3.c
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

string id 		= "sightseebless-adv3";
string name 	= HIG"王者"NOR GRN"天下"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 7200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 5, ob);
	set(query_key()+"/"+BUFF_PHY, 5, ob);
	set(query_key()+"/"+BUFF_INT, 5, ob);
	set(query_key()+"/"+BUFF_CHA, 5, ob);
	set(query_key()+"/"+BUFF_AGI, 5, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 100, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 100, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 10, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 10, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 100, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 100, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 100, ob);

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