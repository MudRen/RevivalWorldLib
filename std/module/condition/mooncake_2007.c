/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mooncake_2007.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-28
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "mooncake-2007";
string name 	= HIB"月懸"NOR BLU"碧空"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 299;
int heartbeat 	= 30;

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
	ob->earn_health(50);
	msg("$ME受到「"+query_condition_name()+"」的"+query_condition_type()+"，恢復 "HIR"50"NOR" 生命值。\n", ob, 0 ,1);
}