/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sightseebless-adv3.c
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

string id 		= "sightseebless-adv3";
string name 	= HIG"����"NOR GRN"�ѤU"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 7200;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 5, ob);
	set(query_key()+"/"+BUFF_PHY, 5, ob);
	set(query_key()+"/"+BUFF_INT, 5, ob);
	set(query_key()+"/"+BUFF_CHA, 5, ob);
	set(query_key()+"/"+BUFF_AGI, 5, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 100, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 100, ob);
	set(query_key()+"/"+BUFF_SOCIAL_EXP_BONUS, 10, ob);
	set(query_key()+"/"+BUFF_COMBAT_EXP_BONUS, 10, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 100, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 100, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 100, ob);

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