/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _message_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : NPC訊息繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <message.h>
#include <daemon.h>

/* 由 tell_object(), write() 等呼叫 */
void catch_tell(string msg, string classes)
{
	object me = this_player();
	string *command, verb, args;

	if( query_temp("disable/msg") || !objectp(me) || !userp(me) || me == this_object() ) return;
	
	command = query_temp("command", me);
	
	if( sizeof(command) != 2 ) return;
	
	verb = command[0];
	args = command[1];
	
	// 表情指令
	if( classes == EMTMSG && EMOTE_D->is_emote(verb) )
	{
		string id, arg;

		if( args )
		{
			if( sscanf(args, "%s %s", id, arg) == 2 && this_object()->id(id) )
				call_out((: call_other :), 1, this_object(), "reply_emote", me, verb, arg);
			else if( this_object()->id(args) )
				call_out((: call_other :), 1, this_object(), "reply_emote", me, verb);
		}
		else
			call_out((: call_other :), 1, this_object(), "reply_emote", me, verb);
	}
	// Say 指令
	else if( classes == SAYMSG )
		call_out((: call_other :), 1, this_object(), "reply_say", me, args);
		
	// 任意 catch_tell 所收到的訊息
	//else
	//	call_out((: call_other :), 1, this_object(), "reply_catch_tell", me, verb, args);
}

