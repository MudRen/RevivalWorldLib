/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mooncake_2007.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-09-28
 * Note   : 
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */
 
#include <ansi.h>
#include <buff.h>
#include <feature.h>

inherit CONDITION_MOD;

string id 		= "mooncake-2007";
string name 	= HIB"���a"NOR BLU"�Ѫ�"NOR;
string type 	= CONDITION_TYPE_POSITIVE;
int time 		= 299;
int heartbeat 	= 30;

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
	ob->earn_health(50);
	msg("$ME����u"+query_condition_name()+"�v��"+query_condition_type()+"�A��_ "HIR"50"NOR" �ͩR�ȡC\n", ob, 0 ,1);
}