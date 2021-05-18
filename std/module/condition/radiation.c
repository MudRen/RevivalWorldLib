/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : radiation.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-08-27
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>
#include <daemon.h>
#include <combat.h>

inherit CONDITION_MOD;

string id 	= "radiation";
string name 	= HIR"��g"NOR RED"�ìV"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 	= 60;
int heartbeat 	= 5;

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
	object caster = query(query_key()+"/caster", ob);
	
	if( !objectp(ob) || ob->is_faint() || ob->is_dead() || !objectp(caster) ) return;
	
	msg("$ME����u"+query_condition_name()+"�v���Iŧ�A�y���۷��j���ˮ`�C\n", ob, 0, 1);
	
	COMBAT_D->cause_damage(caster, ob, ob->query_health_max()/10, NO_FATALBLOW);
}

int stackable()
{
	return 3;
}