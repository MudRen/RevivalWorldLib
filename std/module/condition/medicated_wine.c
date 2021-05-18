/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : medicated_wine.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-26
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "medicated_wine";
string name 	= HIR"�ε�"NOR RED"����"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 300;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_PHY, 2, ob);
	set(query_key()+"/"+BUFF_HEALTH_REGEN, 10, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 100, ob);

	::start_effect(ob);
}

// �������A�ɪ��ĪG
void stop_effect(object ob)
{
	::stop_effect(ob);
}

// ���A�i�椤���ĪG
void heart_beat_effect(object ob)
{


}