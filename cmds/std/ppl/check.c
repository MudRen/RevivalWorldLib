/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : check.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-20
 * Note   : check ���O
 * Update :
 *  o 2000-00-00  
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
check		- �b�a�Ϩt�θ��ˬd�a�W���~�����A
HELP;

#define LOG			"daemon/map_d"

private void do_command(object me, string arg)
{
	object env = environment(me);
	array loc = query_temp("location", me);
	object* inv;
	
	if( !env->is_maproom() || !arrayp(loc) )
		return tell(me, pnoun(2, me)+"�u��b�a�Ϩt�ΤW����o���ˬd�R�O�C\n");
	
	inv = MAP_D->coordinate_inventory(loc);
	
	tell(me, "�b�o�Ӯy�ФW�@�� "+sizeof(inv)+" �󪫫~�G\n");

	foreach(object ob in inv)
	{
		if( !query_temp("location", ob) )
		{
			tell(me, ob->query_idname()+"�ॢ�y�и�ơA�w�����óq���Ův�B�z�C\n");
			log_file(LOG, "�a�Ϩt�Τ��� "+file_name(ob)+" �ॢ�y�и�ơC\n");
			CHANNEL_D->channel_broadcast("sys", "�a�Ϩt�Τ��� "+file_name(ob)+" �ॢ�y�и�ơC");
		}
		else if( !environment(ob) )
		{
			tell(me, ob->query_idname()+"�ॢ���Ҹ�ơA�w�����óq���Ův�B�z�C\n");
			log_file(LOG, "�a�Ϩt�Τ��� "+file_name(ob)+" �ॢ���Ҹ�ơC\n");
			CHANNEL_D->channel_broadcast("sys", "�a�Ϩt�Τ��� "+file_name(ob)+" �ॢ���Ҹ�ơC");
		}
		else
			tell(me, ob->query_idname()+"���y�и�Ƥ@�����`�C\n");
	}
}
