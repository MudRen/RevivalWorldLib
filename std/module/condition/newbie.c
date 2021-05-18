/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : newbie.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-09
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 	= "newbie";
string name 	= HIG"新手"NOR GRN"培育術"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 86400;
int heartbeat 	= 0;

// 啟動狀態時的效果
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 50, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 50, ob);

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
