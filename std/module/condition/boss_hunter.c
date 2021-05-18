/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cheer_coffee.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "boss hunter";
string name 	= HIW"王者"NOR WHT"獵人"NOR;
string type 	= CONDITION_TYPE_COUNTDOWN;
int time 		= 1200;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	::start_effect(ob);
	msg("$ME在狀態時間內無法再與一星與二星王進行戰鬥。\n", ob, 0, 1);
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
