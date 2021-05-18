/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : assist.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : assist 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <npc.h>

inherit COMMAND;

string help = @HELP
輔助他人進行攻擊。

assist [某人]		- 開始輔助某人攻擊
assist -d		- 停止輔助攻擊

若由員工輔助攻擊，怪物的攻擊順序：員工 -> 下達攻擊指令玩家
若由玩家輔助攻擊，敵人的攻擊順序：下達攻擊指令玩家 -> 輔助攻擊玩家
HELP;

private void do_command(object me, string arg)
{
	string target_id;
	object target_ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆希望我輔助誰攻擊？\n");
		else
			return tell(me, pnoun(2, me)+"想要輔助誰攻擊？\n");
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
		target_ob = query_temp("assisting", me);
		
		if( !objectp(target_ob) )
		{
			delete_temp("assisting", me);

			if( me->is_npc() )
				return me->do_command("say 我原本便沒有輔助任何人攻擊。\n");
			else
				return tell(me, pnoun(2, me)+"原本便沒有輔助任何人攻擊。\n");
		}

		delete_temp("assisting", me);

		msg("$ME停止輔助$YOU攻擊。\n", me, target_ob, 1);
		
		return;
	}

	if( me->is_module_npc() && query("job/type", me) != SOLDIER )
	{
		me->do_command("say 我不是軍人，無法戰鬥");
		return;
	}

	if( BATTLEFIELD_D->inside_battlefield(me) )
		return tell(me, "戰場中無法使用輔助攻擊指令。\n");
		
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
			return me->do_command("say "+target_ob->query_idname()+"不是生物，無法輔助攻擊。\n");
		else
			return tell(me, target_ob->query_idname()+"不是生物，無法輔助攻擊。\n");
	}
		
	if( target_ob == me )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法輔助我自己進行攻擊。\n");
		else
			return tell(me, pnoun(2, me)+"無法輔助"+pnoun(2, me)+"自己進行攻擊。\n");
	}

	if( (me->is_module_npc() || userp(me)) && !me->is_in_my_group(target_ob) )
	{
		if( me->is_npc() )
			return me->do_command("say 我不在"+target_ob->query_idname()+"的隊伍中，無法協助"+pnoun(3, target_ob)+"進行攻擊。\n");
		else
			return tell(me, pnoun(2, me)+"不在"+target_ob->query_idname()+"的隊伍中，無法協助"+pnoun(3, target_ob)+"進行攻擊。\n");
	}

	if( query_temp("assisting", target_ob) )
	{
		if( me->is_npc() )
			return me->do_command("say "+target_ob->query_idname()+"正在協助其他人攻擊，我無法協助"+pnoun(3, target_ob)+"。\n");
		else
			return tell(me, target_ob->query_idname()+"正在協助其他人攻擊，"+pnoun(2, me)+"無法協助"+pnoun(3, target_ob)+"。\n");
	}
	
	set_temp("assisting", target_ob, me);

	msg("$ME開始輔助$YOU進行攻擊。\n", me, target_ob, 1);	
}
