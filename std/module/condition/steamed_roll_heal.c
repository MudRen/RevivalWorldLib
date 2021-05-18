/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : steamed_roll_heal.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>
#include <daemon.h>

inherit CONDITION_MOD;

string id 	= "steamed_roll_heal";
string name 	= HIW"���x"WHT"�κZ"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 	= 60;
int heartbeat 	= 2;

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
	int effect;
	
	if( !objectp(ob) || ob->is_faint() || ob->is_dead() ) return;
	
	effect = query_temp("steamed_roll_heal_effect", ob);
	
	msg("$ME����u"+query_condition_name()+"�v���v���A��_�ͩR��("HIR+effect+NOR")�C\n", ob, 0, 1);
	
	ob->earn_health(effect);
}

