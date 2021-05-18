/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : christmas_2007.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-28
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

string id 		= "christmas-2007";
string name 	= HIG"風中"NOR GRN"滑你老母"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 299;
int heartbeat 	= 30;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+SPEED, COMBAT_D->speed_value(ob)/20, ob);

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
