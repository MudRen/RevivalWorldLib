/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fishingnote.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "fishingnote";
string name 	= HIW"釣魚"NOR WHT"筆記"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 1200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_INT, 1, ob);
	set(query_key()+"/"+BUFF_STR, -1, ob);

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
