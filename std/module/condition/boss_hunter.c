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

string id 		= "boss hunter";
string name 	= HIW"����"NOR WHT"�y�H"NOR;
string type 	= CONDITION_TYPE_COUNTDOWN;
int time 		= 1200;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	::start_effect(ob);
	msg("$ME�b���A�ɶ����L�k�A�P�@�P�P�G�P���i��԰��C\n", ob, 0, 1);
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
