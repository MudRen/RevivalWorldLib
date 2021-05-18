/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : air_conditioner.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-03
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "air_conditioner";
string name 	= HIW"天寒"NOR WHT"地凍"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 600;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STAMINA_REGEN, 2, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 100, ob);
	set(query_key()+"/"+BUFF_INT, 2, ob);

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
