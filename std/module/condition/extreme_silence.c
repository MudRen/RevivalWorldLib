/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : extreme_silence.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-08
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "extreme_silence";
string name 	= HIW"���P"NOR WHT"�����R"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 172800;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
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