/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : die.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "invincibility";
string name 	= HIC"完全"NOR CYN"無敵"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 10;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
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