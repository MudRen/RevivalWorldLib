/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deadly_contract.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-08
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "deadly_contract";
string name 	= HIW"致命"NOR WHT"的契約"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 172800;
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