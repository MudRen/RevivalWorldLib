/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : group.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-11
 * Note   : group 指令
 * Update :
 *  o 2000-00-00  
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <group.h>

inherit COMMAND;

string help = @HELP
指令說明:

此指令可以用來組成隊伍

指令格式 : 
	group        			列出目前隊伍名單
	group create '名稱'		建立隊伍並取名
	group accept			答應加入隊伍
	group reject			拒絕加入隊伍
	group leave			離開隊伍
	group invite '玩家或員工'	邀請玩家或員工加入隊伍
	group remove '玩家或員工'	將玩家或員工踢出隊伍		
	group leader '玩家'		將隊長身份交給另一位玩家(隊長指令)
	group rename '名稱'		重新取名隊伍名稱(隊長指令)
	group dismiss			解散隊伍(隊長指令)
	group call '玩家'		快速召集某位玩家隊員至此地(需花費 $RW 1,000,000)

相關指令:
	gt (隊伍頻道)
HELP;

private void do_command(object me, string arg)
{
	string msg;
	mapping group;
	object group_leader = me->query_group_leader();
	object *members;
	object member;
	object target;

	if( !is_command() ) return;

	if( wizardp(me) && arg && find_player(arg) )
	{
		target = find_player(arg);
		arg = 0;
	}
	else
		target = me;
		
	if( !arg  )
	{
		array group_data;

		group_leader = target->query_group_leader();

		if( !objectp(group_leader) )
			return tell(me, (target==me?pnoun(2, me):target->query_idname())+"目前不在任何隊伍中。\n");
		
		msg = (target==me?pnoun(2, me):target->query_idname())+"目前在「"+group_leader->query_group_name()+NOR"」的隊伍中，成員名單如下：\n";
		msg += "────────────────────────────\n";
		group = group_leader->query_group_data();
		
		group_data = sort_array(unique_array(keys(group), (: $(group)[$1] :)), (: $(group)[$1[0]] - $(group)[$2[0]] :));

		foreach(members in group_data)
		{
			switch(group[members[0]])
			{
				case GROUP_LEADER:
					msg += HIC"隊"NOR CYN"長\n"NOR;
					break;
				case GROUP_PLAYER:
					msg += HIY"玩家"NOR YEL"成員\n"NOR;
					break;
				case GROUP_LABOR:
					msg += HIG"員工"NOR GRN"成員\n"NOR;
					break;
				default:
					break;
			}
			
			foreach(member in members)
			{
				if( objectp(member) )
					msg += "    "+member->query_idname()+"\n";
			}
		}
		msg += "────────────────────────────\n";
		return tell(me, msg);
	}

	if( sscanf(arg, "create %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");
			
		if( objectp(group_leader) )
		{
			if( me == group_leader )
				return tell(me, pnoun(2, me)+"已經是「"+me->query_group_name()+"」隊伍的隊長了。\n");
			else
				return tell(me, pnoun(2, me)+"目前在「"+me->query_group_name()+"」的隊伍中，"+pnoun(2, me)+"必須先離隊後再建立新隊伍。\n");
		}
		
		if( noansi_strlen(arg) > 20 )
			return tell(me, "隊伍名稱不可超過 20 個字元。\n");

		me->create_new_group(arg);
		
		msg(HIY+"$ME"HIY"建立了一個全新的隊伍名叫「"+arg+NOR HIY"」。\n"NOR, me, 0, 1);
		
		return;
	}
	else if( sscanf(arg, "invite %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		target =  present(arg, environment(me)) || find_player(arg);
	
		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"必須先建立一個新隊伍。\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"必須是隊伍的隊長才可以邀請其他玩家或員工加入隊伍。\n");

		if( !objectp(target) )
			return tell(me, "這附近沒有 "+arg+" 這位玩家或員工。\n");
			
		if( me == target )
			return tell(me, pnoun(2, me)+"不需要邀請自己加入隊伍。\n");

		if( !target->is_living() || (!userp(target) && !target->is_module_npc()) )
			return tell(me, pnoun(2, me)+"無法邀請"+target->query_idname()+"加入隊伍。\n");
		
		if( me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"已經在"+pnoun(2, me)+"的隊伍中了。\n");	
		
		if( objectp(target->query_group_leader()) )
			return tell(me, target->query_idname()+"已經在其他的隊伍中，無法再邀請。\n");

		if( target->is_module_npc() )
		{
			string boss = query("boss", target);
			object boss_ob = find_player(boss);
			
			if( !objectp(boss_ob) || !me->is_in_my_group(boss_ob) )
				return tell(me, pnoun(2, me)+"必須先邀請"+target->query_idname()+"的雇主加入隊伍中。\n");	
	
			members = me->query_group_members();
			
			foreach(member in members)
				tell(member, HIY+target->query_idname()+HIY"加入隊伍。\n"NOR);
	
			me->add_group_member(target);
		}
		else
		{
			msg(HIY"$ME"HIY"邀請$YOU"HIY"加入「"+me->query_group_name()+HIY"」的隊伍。\n"NOR, me, target, 1);
			
			set_temp("group/invite", me, target);
	
			tell(target, HIC"若欲加入隊伍請輸入「group accept」、拒絕加入請輸入「group reject」或不予理會。\n"NOR);
	
			members = me->query_group_members() - ({ me });
			
			foreach(member in members)
				tell(member, HIY+me->query_idname()+HIY"邀請"+target->query_idname()+HIY"加入隊伍。\n"NOR);
		}

		return;		
	}
	else if( sscanf(arg, "remove %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		target = present(arg) || find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"必須是隊伍的隊長才可以將其他玩家或員工踢出隊伍。\n");

		if( !objectp(target) )
			return tell(me, "這附近沒有 "+arg+" 這位玩家或員工。\n");
			
		if( me == target )
			return tell(me, pnoun(2, me)+"無法將自己踢出隊伍，若要離隊請先移交隊長身份或直接解散隊伍。\n");

		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"原本就不在"+pnoun(2, me)+"的隊伍中。\n");	
			
		msg(HIY"$ME"HIY"將$YOU"HIY"踢出隊伍。\n"NOR, me, target, 1);

		if( userp(target) )
		foreach(member in copy(me->query_group_members()))
		{
			if( member->is_module_npc() && query("boss", member) == target->query_id(1) )
				member->do_command("group leave");
		}

		me->remove_group_member(target);

		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"將"+target->query_idname()+HIY"踢出隊伍。\n"NOR);

		return;		
	}
	else if( sscanf(arg, "leader %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		target = find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"必須是隊伍的隊長才可以移交隊長身份。\n");

		if( !objectp(target) )
			return tell(me, "沒有 "+arg+" 這位玩家。\n");
			
		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"不在"+pnoun(2, me)+"的隊伍中。\n");	
			
		tell(me, HIY+pnoun(2, me)+"將隊長身份移交給"+target->query_idname()+HIY"。\n"NOR);
		
		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"將隊長身份移交給"+target->query_idname()+HIY"。\n"NOR);
		
		target->become_group_leader(me);

		return;		
	}
	else if( sscanf(arg, "call %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		target = find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"必須是隊伍的隊長才可以召集隊員。\n");

		if( !objectp(target) )
			return tell(me, "沒有 "+arg+" 這位玩家。\n");

		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"不在"+pnoun(2, me)+"的隊伍中。\n");	

		// 戰鬥中
		if( COMBAT_D->in_fight(target) )
			return tell(me, target->query_idname()+"目前正在戰鬥中。\n");
		
		// 是犯人
		if( !target->valid_move() )
			return tell(me, target->query_idname()+"目前無法移動。\n");
		
		if( same_environment(me, target) )
			return tell(me, target->query_idname()+"已經在這裡了。\n");

		if( AREA_D->query_area_info(environment(me)->query_area(), "need_flag") )
			return tell(me, pnoun(2, me)+"所在的地區是特殊區域，無法使用隊伍召集。\n");
	
		if( !me->spend_money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST) )
			return tell(me, pnoun(2, me)+"身上的錢不足 "HIY+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+NOR"。\n");
			
		msg(me->query_idname()+"花費 "+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+" 號令隊伍集合，將$ME召集至"+pnoun(3, me)+"身邊。\n", target, 0, 1);
		
		target->move_to_environment(me);

		msg("$ME花費 "+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+" 號令隊伍集合，將$YOU召集至$ME身邊。\n", me, target, 1);

		return;		
	}
	else if( sscanf(arg, "rename %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");
		
		if( me != group_leader )
			return tell(me, pnoun(2, me)+"必須是隊伍的隊長才可以更改隊伍名稱。\n");
			
		if( noansi_strlen(arg) > 20 )
			return tell(me, "隊伍名稱不可超過 20 個字元。\n");
		
		tell(me, HIY+pnoun(2, me)+"將隊伍名稱更改為「"+arg+HIY"」。\n"NOR);

		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"將隊伍名稱更改為「"+arg+HIY"」。\n"NOR);
		
		me->set_group_name(arg);

		return;
	}
	else if( arg == "accept" )
	{
		object inviter = query_temp("group/invite", me);

		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");
		
		if( !objectp(inviter) || !inviter->is_group_leader() )
			return tell(me, "並未有人邀請"+pnoun(2, me)+"加入隊伍，或是該玩家已經解散隊伍。\n");
		
		msg(HIY"$ME"HIY"加入$YOU"HIY"的「"+inviter->query_group_name()+HIY"」隊伍。\n"NOR, me, inviter, 1);
		
		members = inviter->query_group_members() - ({ inviter });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"加入隊伍。\n"NOR);

		inviter->add_group_member(me);

		delete_temp("group/invite", me);	
		
		return;
	}
	else if( arg == "reject" )
	{
		object inviter = query_temp("group/invite", me);

		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		if( !objectp(inviter) || !inviter->is_group_leader() )
			return tell(me, "並未有人邀請"+pnoun(2, me)+"加入隊伍，或是該玩家已經解散隊伍。\n");
		
		msg(HIY"$ME"HIY"拒絕加入$YOU"HIY"的「"+inviter->query_group_name()+HIY"」隊伍。\n"NOR, me, inviter, 1);
		
		members = inviter->query_group_members() - ({ inviter });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"拒絕加入隊伍。\n"NOR);

		delete_temp("group/invite", me);
		
		return;
	}
	else if( arg == "leave" )
	{
		if( !objectp(group_leader) )
		{
			if( me->is_npc() )
				return me->do_command("say 我不在任何隊伍當中。\n");
			else
				return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");
		}

		if( me == group_leader )
			return tell(me, pnoun(2, me)+"必須先將隊長身份移交給另一名玩家才可以離開隊伍，或是直接解散隊伍。\n");

		if( me->is_npc() )
			msg(HIY"$ME"HIY"離開了"+group_leader->query_idname()+HIY"的「"+group_leader->query_group_name()+HIY"」隊伍。\n"NOR, me, 0, 1);
		else
			tell(me, HIY+pnoun(2, me)+"離開了"+group_leader->query_idname()+HIY"的「"+group_leader->query_group_name()+HIY"」隊伍。\n"NOR);

		foreach(member in copy(group_leader->query_group_members()))
		{
			if( !objectp(member) ) continue;

			if( member->is_module_npc() && query("boss", member) == me->query_id(1) && userp(me) )
				member->do_command("group leave");
		}

		group_leader->remove_group_member(me);

		members = group_leader->query_group_members();
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"離開隊伍。\n"NOR);
			
		return;	
	}
	else if( arg == "dismiss" )
	{
		if( me->is_npc() )
			return me->do_command("say 我無法執行此命令。\n");

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"不在任何隊伍當中。\n");
		
		if( me != group_leader )
			return tell(me, pnoun(2, me)+"不是隊長，無法解散隊伍。\n");
			
		tell(me, HIY+pnoun(2, me)+"解散了整個「"+me->query_group_name()+HIY"」隊伍。\n"NOR);

		members = group_leader->query_group_members() - ({ me });
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"解散了整個隊伍。\n"NOR);

		me->dismiss_group();
		
		return;
	}


	return tell(me, help);

}
