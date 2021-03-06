/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : newbie_adv.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-08-27
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 	= "newbie-adv";
string name 	= HIY"新人"NOR YEL"加持"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 2592000;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 30, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 30, ob);
	set(query_key()+"/"+BUFF_STAMINA_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_HEALTH_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_ENERGY_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_STR, 3, ob);
	set(query_key()+"/"+BUFF_PHY, 3, ob);
	set(query_key()+"/"+BUFF_INT, 3, ob);
	set(query_key()+"/"+BUFF_AGI, 3, ob);
	set(query_key()+"/"+BUFF_CHA, 3, ob);

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
