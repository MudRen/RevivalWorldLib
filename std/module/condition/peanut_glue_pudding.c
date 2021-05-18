/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : peanut_glue_pudding.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-22
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "peanut glue pudding";
string name 	= YEL"�H�V"HIY"�e�x"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 10800;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_AGI, 10, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 100, ob);

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