/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : follow.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : follow 指令
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
這個指令可以讓你自動跟著另一個人移動。

follow [某人]		- 開始跟隨某人
follow -d		- 停止跟隨任何人

HELP;

private void do_command(object me, string arg)
{
	string target_id;
	object target_ob;
	object *followers;
	object following_ob;
	string *ignore;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆希望我跟隨誰？\n");
		else
			return tell(me, pnoun(2, me)+"想要跟隨誰？\n");
	}
	
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		if( me->is_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( arg == "-d" )
	{
		target_ob = query_temp("following", me);
		
		if( !objectp(target_ob) )
		{
			delete_temp("following", me);

			if( me->is_npc() )
				return me->do_command("say 我原本便沒有在跟隨任何人。\n");
			else
				return tell(me, pnoun(2, me)+"原本便沒有在跟隨任何人。\n");
		}

		followers = query_temp("follower", target_ob);		

		if( arrayp(followers) && sizeof(followers) )
		{
			followers -= ({ me });
			
			if( !sizeof(followers) )
				delete_temp("follower", target_ob);
			else
				set_temp("follower", followers, target_ob);
		}
		
		delete_temp("following", me);

		msg("$ME停止繼續跟隨$YOU的行為。\n", me, target_ob, 1);
		
		return;
	}

	target_id = arg;
	target_ob = present(target_id);

	if( !objectp(target_ob) )
	{
		if( me->is_npc() )
		{
			if( target_id == "me" )
				target_ob = this_player(1);
			else
				return me->do_command("say 這裡沒有 "+arg+" 這個東西。\n");
		}
		else
			return tell(me, "這裡沒有 "+arg+" 這個東西。\n");
	}

	if( !target_ob->is_living() )
	{
		if( me->is_npc() )
			return me->do_command("say "+target_ob->query_idname()+"不是生物，無法跟隨。\n");
		else
			return tell(me, target_ob->query_idname()+"不是生物，無法跟隨。\n");
	}
		
	if( target_ob == me )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法跟隨我自己。\n");
		else
			return tell(me, pnoun(2, me)+"無法跟隨"+pnoun(2, me)+"自己。\n");
	}

	if( BATTLEFIELD_D->inside_battlefield(me) )
		return tell(me, "戰場中無法使用跟隨指令。\n");

	following_ob = query_temp("following", me);

	if( objectp(following_ob) )
	{
		followers = query_temp("follower", following_ob) || allocate(0);
		followers -= ({ me });
		
		if( !sizeof(followers) )
			delete_temp("follower", following_ob);
		else
			set_temp("follower", followers, following_ob);
			
		msg("$ME停止跟隨$YOU。\n", me, following_ob, 1);	
	}

	if( !userp(target_ob) )
		return tell(me, "目前跟隨功能只能夠跟隨玩家。\n");

	ignore = query("ignore", target_ob);
	
	if( arrayp(ignore) )
	{
		string id;
		
		if( me->is_npc() )
			id = query("boss", me);
		else
			id = me->query_id(1);
		
		if( stringp(id) && member_array(id, ignore) != -1 )
		{
			if( me->is_npc() )
				return me->do_command("say "+target_ob->query_idname()+"拒絕讓我跟隨。\n");
			else
				return tell(me, target_ob->query_idname()+"拒絕讓"+pnoun(2, me)+"跟隨。\n");
		}
	}

	followers = query_temp("follower", target_ob) || allocate(0);
	
	followers |= ({ me });
	
	set_temp("follower", followers, target_ob);
	set_temp("following", target_ob, me);

	msg("$ME開始形影不離地跟在$YOU的身後。\n", me, target_ob, 1);	
}
