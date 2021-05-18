/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ignore.c
 * Author : Msr@RevivalWorld
 * Date   : 2001-07-02
 * Note   : ignore 指令
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
指令說明:

此指令可以過濾某些人的交談及表情動作並且拒絕其跟隨

指令格式 : ignore        列出所有的拒絕理會的黑名單
           ignore -a id  增加新的 id 進入黑名單內
           ignore -d id  將某個人的 id 從黑名單中拿掉

相關指令:
	tell, channels
HELP;

private void do_command(object me, string str)
{
	int i = 0;
	string id, *ignore, msg;

	if( !is_command() ) return;

	if( !arrayp( ignore = query("ignore", me) ) )
		ignore = allocate(0);

	if( !stringp(str) )
	{
		if( !sizeof(ignore) ) 
			return tell( me, pnoun(2, me)+"目前沒有任何黑名單。\n");

		msg = pnoun(2, me)+"的黑名單如下 :\n";

		for( i=0 ; i < sizeof(ignore); i++)
			msg += sprintf( "[ %d ]\t%s\n", i+1, ignore[i] );

		return tell( me, msg);
	}

	if( sscanf( str,"-d %s",id) )
	{
		id = remove_ansi(lower_case(id));

		if( member_array( id, ignore ) == -1 )
			return tell(me, id+" 原本就不在黑名單中。\n");

		ignore -= ({ id });

		if( !sizeof(ignore) )
			delete("ignore", me);
		else
			set("ignore", ignore, me);

		me->save();
		return tell(me, pnoun(2, me)+"將 "+capitalize(id)+" 從"+pnoun(2, me)+"的黑名單中消除了。\n");
	}

	if( sscanf(str, "-a %s",id) )
	{
		object *followers = query_temp("follower", me);

		id = remove_ansi(lower_case(id));

		if( !user_exists(id) )
			return tell(me, "本遊戲中並沒有 "+capitalize(id)+" 這位玩家角色。\n");

		if( member_array( id, ignore ) !=-1 ) 
			return tell(me, capitalize(id)+" 已經在"+pnoun(2, me)+"的黑名單中。\n");

		if( id == me->query_id(1) )
			return tell(me, pnoun(2, me)+"不能將自己加入黑名單中。\n");

		ignore |= ({ id });

		set("ignore", ignore, me );
		me->save();
		
		if( arrayp(followers) )
		{
			object target_ob = find_player(id);
			
			if( objectp(target_ob) && member_array(target_ob, followers) != -1 )
			{
				followers -= ({ target_ob });
				
				set_temp("follower", followers, me);
				delete_temp("following", target_ob);
				
				msg("$ME停止跟隨$YOU。\n", target_ob, me, 1);
			}
		}

		return tell(me, pnoun(2, me)+"將 "+capitalize(id)+" 加入黑名單中。\n");
	}

	return tell(me, help);

}
