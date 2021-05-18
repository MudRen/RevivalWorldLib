/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : send_gift.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : send_gift ���O�C
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
        �з� send_gift ���O�C
HELP;

private void send_gift()
{
	object user, ob;

	string list="";
	
	foreach(string id in explode(list, " "))
	{
		if( !objectp(user = load_user(lower_case(id))) )
		{
			CHANNEL_D->channel_broadcast("sys", "�L�k���J�W�� "+id);
			continue;
		}
		
		user->restore_inventory();
		
		//ob = new("/obj/festival/2009/forum_gift/cheer_cd");
		
		//ob->move(user);
		
		//ob->set_keep();
		
		user->save_inventory();
		user->save();
		
		CHANNEL_D->channel_broadcast("news", user->query_idname()+"�]�ѥ[�u���y�ϥν׾¼��y���� �v�A��o�S����y�~�u"+ob->query_idname()+"�v");

		if( !userp(user) )
			destruct(user);
	}
}

private void startup_send_gift()
{
	int costtime;
	
	costtime = time_expression { send_gift(); };

        CHANNEL_D->channel_broadcast("news", sprintf("���y�~�o�񧹲��A�@��O %.3f ��C", costtime/1000000.));
}
	
private void command(object me, string arg)
{
        if( !is_command() ) return;

        CHANNEL_D->channel_broadcast("news", "�T�Q������t�ζ}�l�Τ@�o��S����y�~�C");

	call_out((: startup_send_gift :), 30);
}
