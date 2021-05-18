/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cheer_coffee.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "cheer coffee";
string name 	= HIW"�E��"NOR WHT"���@�ة�"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 600;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_INT, 2, ob);
	set(query_key()+"/"+BUFF_CHA, 2, ob);
	set(query_key()+"/"+BUFF_ENERGY_REGEN, 4, ob);

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
