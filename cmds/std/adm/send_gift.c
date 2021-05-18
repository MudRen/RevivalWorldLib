/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : send_gift.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-11
 * Note   : send_gift 指令。
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
        標準 send_gift 指令。
HELP;

private void send_gift()
{
	object user, ob;

	string list="";
	
	foreach(string id in explode(list, " "))
	{
		if( !objectp(user = load_user(lower_case(id))) )
		{
			CHANNEL_D->channel_broadcast("sys", "無法載入名單 "+id);
			continue;
		}
		
		user->restore_inventory();
		
		//ob = new("/obj/festival/2009/forum_gift/cheer_cd");
		
		//ob->move(user);
		
		//ob->set_keep();
		
		user->save_inventory();
		user->save();
		
		CHANNEL_D->channel_broadcast("news", user->query_idname()+"因參加「鼓勵使用論壇獎勵活動 」，獲得特殊獎勵品「"+ob->query_idname()+"」");

		if( !userp(user) )
			destruct(user);
	}
}

private void startup_send_gift()
{
	int costtime;
	
	costtime = time_expression { send_gift(); };

        CHANNEL_D->channel_broadcast("news", sprintf("獎勵品發放完畢，共花費 %.3f 秒。", costtime/1000000.));
}
	
private void command(object me, string arg)
{
        if( !is_command() ) return;

        CHANNEL_D->channel_broadcast("news", "三十秒鐘後系統開始統一發放特殊獎勵品。");

	call_out((: startup_send_gift :), 30);
}
