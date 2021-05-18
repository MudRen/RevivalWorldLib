/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : die.c
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

string id 		= "die";
string name 	= NOR WHT"���`�A�G"NOR;
string type 	= CONDITION_TYPE_NEGATIVE;
int time 		= 3599;
int heartbeat 	= 60;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, -5, ob);
	set(query_key()+"/"+BUFF_PHY, -5, ob);
	set(query_key()+"/"+BUFF_INT, -5, ob);
	set(query_key()+"/"+BUFF_AGI, -5, ob);
	set(query_key()+"/"+BUFF_CHA, -5, ob);
	
	set(query_key()+"/"+BUFF_STAMINA_MAX, -250, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, -250, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, -250, ob);

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
	ob->cost_stamina(100);
	ob->cost_health(100);
	ob->cost_energy(100);
	msg("$ME����u"+query_condition_name()+"�v���I�k�ˮ`�A�l�� 100 �I����O�B�ͩR�B�믫�ȡC\n", ob, 0, 1);	
}