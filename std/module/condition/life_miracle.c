/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : life_miracle.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-09
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "life miracle";
string name 	= HIW"�ͩR"NOR WHT"�_��"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 21600;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	::start_effect(ob);
	msg("$ME�b���A�ɶ����Y�������`�N���|���즺�`�g�@�C\n", ob, 0, 1);
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
