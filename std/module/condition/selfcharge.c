/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : selfcharge.c
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

string id 		= "selfcharge";
string name 	= HIY"�ۧ�"NOR YEL"�R��"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= (3*60*60);
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	string condition_key = query_key()+"/";

	set(condition_key+BUFF_SOCIAL_EXP_BONUS, 2, ob);
	set(condition_key+BUFF_COMBAT_EXP_BONUS, 2, ob);
	set(condition_key+BUFF_STAMINA_MAX, 50, ob);
	set(condition_key+BUFF_HEALTH_MAX, 50, ob);
	set(condition_key+BUFF_ENERGY_MAX, 50, ob);

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
