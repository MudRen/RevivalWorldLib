/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : guitar_weak.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-06-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <daemon.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "guitar_weak";
string name 	= HIY"�_"NOR YEL"��"HIY"�M"NOR YEL"��"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 		= 60;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	addn(query_key()+"/"+ATTACK, -COMBAT_D->attack_value(ob, 1)/25, ob);

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

int stackable()
{
	return 5;
}
