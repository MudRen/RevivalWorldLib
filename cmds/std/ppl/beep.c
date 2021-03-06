/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : beep.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-10
 * Note   : beep 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
呼叫指令

這個指令可以用來呼叫其他使用者，若該使用者的 Client 支援則會播放提示聲

指令格式:
beep <使用者代號>

相關指令: waitback
HELP;

private void do_command(object me, string arg)
{
	object ob;
    	string *ignore;
   
    	if( !arg )
    		return tell(me, pnoun(2, me)+"想呼叫哪位線上玩家？\n");
   
    	if( !(ob = find_player(arg)) ) 
    		return tell(me, "沒有這玩家。\n");
    	
   	if( arrayp(ignore = query("ignore", ob)) && member_array(me->query_id(1), ignore) != -1 )
   		return tell(me, ob->query_idname()+"已將"+pnoun(2, me)+"加入"+pnoun(3, ob)+"的黑名單中，因此"+pnoun(2, me)+"目前禁止呼叫"+pnoun(3, ob)+"。\n");		
   
    	msg("$ME對著$YOU不停的嗶嗶叫，吵死了。\n", me, ob, 1);
    	tell(ob,  "\a", 0);
}