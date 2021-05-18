/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gods_transformer.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-17
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

string id 		= "gods_transformer";
string name 	= HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 21600;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{	
	set(query_key()+"/"+BUFF_THORN_CHANCE, 2, ob);
	set(query_key()+"/"+BUFF_FATAL_POWER, 5, ob);
	
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