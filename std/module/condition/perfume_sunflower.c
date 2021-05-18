/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : perfume-sunflower.c
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

string id 		= "perfume-sunflower";
string name 	= HIY"�V�鸪"NOR YEL"�M��"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 3600;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_CHA, 4, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 50, ob);

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