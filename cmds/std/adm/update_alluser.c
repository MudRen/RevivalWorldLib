/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : update_alluser.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-11
 * Note   : update_alluser 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <object.h>
#include <condition.h>
#include <futures.h>

inherit COMMAND;

string help = @HELP
        更新所有玩家資料。
HELP;

int show_msg;

int update_user(object user)
{
	int update_done = 1;

//---------------------------------HEAD


//---------------------------------TAIL
	
	if( update_done ) user->save();
	
	if( !userp(user) ) destruct(user);
	return update_done;
}

private void update_all_user()
{
	object user_ob;
	string failed_id = "失敗 ID：";
	int total_user_amount, update_user_amount = 0;

    foreach( string id in all_player_ids() )
    {
    	reset_eval_cost();
   
    	user_ob = load_user(id);
		
		total_user_amount++;
			
		if( objectp(user_ob) && update_user(user_ob) )
			update_user_amount++;
		else
			failed_id += " "+id;
    }

	if( show_msg )
	{
    	CHANNEL_D->channel_broadcast("news", "系統更新所有玩家角色檔案(共 "+total_user_amount+" 筆資料)完畢。");
    	CHANNEL_D->channel_broadcast("sys", "更新人物檔案完畢，更新成功："+update_user_amount+" 人，更新失敗："+(total_user_amount-update_user_amount)+" 人，"+failed_id+"。");
    }
    
    call_out((: set_heartbeat_interval(1) :), 10 );
}
	
private void command(object me, string arg)
{
    string update_data;

    if( !is_command() ) return;
    
    show_msg = !stringp(arg);

    update_data = read_file(base_name(this_object())+".c");
    update_data = update_data[strsrch(update_data,"//---------------------------------HEAD")..strsrch(update_data,"//---------------------------------TAIL")+40];
    
    if( show_msg )
    {
    	CHANNEL_D->channel_broadcast("sys", me->query_idname()+NOR WHT"執行所有玩家角色檔案更新：");
    	CHANNEL_D->channel_broadcast("sys", "更新內容為：\n"+update_data);
    	CHANNEL_D->channel_broadcast("news", "五秒鐘後系統更新所有玩家角色檔案，更新過程可能耗費數秒鐘至數分鐘，請稍候。");
	}
	
	call_out((: set_heartbeat_interval(0) :), 2 );
	call_out((: update_all_user :), 5);
}
