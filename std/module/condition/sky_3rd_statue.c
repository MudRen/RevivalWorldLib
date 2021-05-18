/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_3rd_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-09-02
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "sky_3rd_statue";
string name 	= HIW"天魁"NOR WHT"神威"NOR;
string type 	= CONDITION_TYPE_COUNTDOWN;
int time 		= 21600;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	::start_effect(ob);
	msg("$ME在狀態持續過程中無法再攻擊天魁。\n", ob, 0, 1);
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