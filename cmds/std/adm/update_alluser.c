/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : update_alluser.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-11
 * Note   : update_alluser ���O�C
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
        ��s�Ҧ����a��ơC
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
	string failed_id = "���� ID�G";
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
    	CHANNEL_D->channel_broadcast("news", "�t�Χ�s�Ҧ����a�����ɮ�(�@ "+total_user_amount+" �����)�����C");
    	CHANNEL_D->channel_broadcast("sys", "��s�H���ɮק����A��s���\�G"+update_user_amount+" �H�A��s���ѡG"+(total_user_amount-update_user_amount)+" �H�A"+failed_id+"�C");
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
    	CHANNEL_D->channel_broadcast("sys", me->query_idname()+NOR WHT"����Ҧ����a�����ɮק�s�G");
    	CHANNEL_D->channel_broadcast("sys", "��s���e���G\n"+update_data);
    	CHANNEL_D->channel_broadcast("news", "��������t�Χ�s�Ҧ����a�����ɮסA��s�L�{�i��ӶO�Ƭ����ܼƤ����A�еy�ԡC");
	}
	
	call_out((: set_heartbeat_interval(0) :), 2 );
	call_out((: update_all_user :), 5);
}
