/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : seehumans.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-17
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <daemon.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "seehumans";
string name 	= HIW"睥"NOR WHT"睨"HIW"人"NOR WHT"間"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 21600;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{	
	set(query_key()+"/"+BUFF_COMBO_CHANCE, 3, ob);
	set(query_key()+"/"+BUFF_COUNTERATTACK_CHANCE, 3, ob);
	set(query_key()+"/"+BUFF_COLLECT_ABILITY, 5, ob);
	set(query_key()+"/"+BUFF_GROW_ABILITY, 5, ob);
	
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