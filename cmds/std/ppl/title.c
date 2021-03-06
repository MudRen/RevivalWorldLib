/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : title.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-02
 * Note   : title 指令
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
    稱號設定，在個人擁有特殊成就後將會獲得各種稀有的稱號
各種特殊事件或任務結果都有機會得到特殊的稱號

	title		顯示目前所用的稱號
	title -l	列出目前可以使用的稱號
	title -d	取消顯示目前稱號
	title '數字'	設定想要顯示的稱號

HELP;

private void do_command(object me, string arg)
{
	string title = query("title", me);
	string *all_titles = query("all_titles", me);
    	
    	if( !arg )
    	{
    		if( !title )
    			return tell(me, pnoun(2, me)+"目前沒有設定顯示任何稱號。\n");
    		
    		return tell(me, pnoun(2, me)+"目前所顯示的稱號為「"+title+"」。\n");
    	}
    	else if( arg == "-l" )
    	{
    		int i;
    		string msg;
    		
    		if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, pnoun(2, me)+"目前沒有任何稱號可以使用。\n");

    		msg = pnoun(2, me)+"目前可以使用的稱號如下：\n";
		
		foreach( title in all_titles )
			msg += (++i)+". "+title+"\n";
			
		return tell(me, msg+"\n");
	}
	else if( arg == "-d" )
	{
		if( !title )
			return tell(me, pnoun(2, me)+"目前沒有設定顯示任何稱號。\n");
			
		delete("title", me);
		return tell(me, pnoun(2, me)+"取消顯示「"+title+"」稱號。\n");
	}
	else if( to_int(arg) )
	{
		int i, j = to_int(arg);
		
		if( j < 1 )
			return tell(me, "請輸入正確的數字。\n");
			
    		if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, pnoun(2, me)+"目前沒有任何稱號可以使用。\n");
    
		foreach( title in all_titles )
		{
			if( ++i == j )
			{
				set("title", title, me);
				return tell(me, pnoun(2, me)+"設定要顯示的稱號為「"+title+"」。\n");
			}
		}
		return tell(me, "並沒有第 "+j+" 個編號的稱號。\n");
	}
	else if( wizardp(me) && find_player(arg) )
	{
		int i;
		string msg;
		object player = find_player(arg);
		
		all_titles = query("all_titles", player);
		
	    	if( !arrayp(all_titles) || !sizeof(all_titles) )
    			return tell(me, player->query_idname()+"目前沒有任何稱號可以使用。\n");
    		
    		msg = player->query_idname()+"目前可以使用的稱號如下：\n";
		
		foreach( title in all_titles )
			msg += (++i)+". "+title+"\n";
			
		return tell(me, msg+"\n");
	}
	return tell(me, help+"\n");
}