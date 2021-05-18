/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_glory.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <daemon.h>
#include <delay.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "battlefield-glory";
string name 	= HIW"���ͪ�"NOR WHT"�Ԫ��aģ"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 120;
int heartbeat 	= 0;

// �Ұʪ��A�ɪ��ĪG
void start_effect(object ob)
{
	set(query_key()+"/"+BUFF_STR, 100, ob);
	set(query_key()+"/"+BUFF_PHY, 100, ob);
	set(query_key()+"/"+BUFF_AGI, 100, ob);
	set(query_key()+"/"+BUFF_FOOD_MAX, 1000, ob);
	set(query_key()+"/"+BUFF_DRINK_MAX, 1000, ob);
	set(query_key()+"/"+BUFF_STAMINA_MAX, 3000, ob);
	set(query_key()+"/"+BUFF_HEALTH_MAX, 3000, ob);
	set(query_key()+"/"+BUFF_ENERGY_MAX, 3000, ob);

	::start_effect(ob);

	CHANNEL_D->channel_broadcast("combat", HIR"��"NOR RED"��"NOR"�G"+ob->query_idname()+"��o�u"+query_condition_name()+"�v�[���A���� "+(query_default_condition_time()/60)+" �����A�ؼЮy�Ь�"+loc_short(query_temp("location", ob))+"�A���Ѧ��H�i��o�j�q���Z");
	ob->start_delay(BATTLE_GLORY_DELAY_KEY, 6, pnoun(2, ob)+"�]���u"+query_condition_name()+"�v��"+query_condition_type()+"�A�ثe�L�k������ʧ@");
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