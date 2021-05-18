/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv6.c
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

string id 	= "sightseebless-adv6";
string name 	= HIW"��"NOR WHT"�t"HIW"�j"NOR WHT"�["NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 7200;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 15, ob);
	set(query_key()+"/"+BUFF_PHY, 15, ob);
	set(query_key()+"/"+BUFF_INT, 15, ob);
	set(query_key()+"/"+BUFF_CHA, 15, ob);
	set(query_key()+"/"+BUFF_AGI, 15, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 400, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 400, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 40, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 400, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 400, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 400, ob);

	set(query_key()+"/"+BUFF_FATAL_POWER, 1, ob);
	set(query_key()+"/"+BUFF_FATAL_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_ANTIFATAL_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_COUNTERATTACK_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_THORN_CHANCE, 1, ob);
	set(query_key()+"/"+BUFF_COMBO_CHANCE, 1, ob);
	
	set(query_key()+"/"+BUFF_STAMINA_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_HEALTH_REGEN, 5, ob);
	set(query_key()+"/"+BUFF_ENERGY_REGEN, 5, ob);

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