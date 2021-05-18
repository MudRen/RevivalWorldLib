/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : power_physique.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "power_physique";
string name 	= HIY"���"NOR YEL"�z�o"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 60;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_PHY, 20, ob);

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