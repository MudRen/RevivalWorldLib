/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv5.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "sightseebless-adv5";
string name 	= HIY"��"NOR YEL"��"HIY"�W"NOR YEL"�L"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 7200;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 15, ob);
	set(query_key()+"/"+BUFF_PHY, 15, ob);
	set(query_key()+"/"+BUFF_INT, 15, ob);
	set(query_key()+"/"+BUFF_CHA, 15, ob);
	set(query_key()+"/"+BUFF_AGI, 15, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 300, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 300, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 30, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 30, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 300, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 300, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 300, ob);

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