/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : air_conditioner.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-03
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "air_conditioner";
string name 	= HIW"�ѴH"NOR WHT"�a��"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 600;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STAMINA_REGEN, 2, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 100, ob);
	set(query_key()+"/"+BUFF_INT, 2, ob);

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
