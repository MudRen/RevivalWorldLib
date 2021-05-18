/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : group.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-11
 * Note   : group ���O
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
���O����:

�����O�i�H�ΨӲզ�����

���O�榡 : 
	group        			�C�X�ثe����W��
	group create '�W��'		�إ߶���è��W
	group accept			�����[�J����
	group reject			�ڵ��[�J����
	group leave			���}����
	group invite '���a�έ��u'	�ܽЪ��a�έ��u�[�J����
	group remove '���a�έ��u'	�N���a�έ��u��X����		
	group leader '���a'		�N���������浹�t�@�쪱�a(�������O)
	group rename '�W��'		���s���W����W��(�������O)
	group dismiss			�Ѵ�����(�������O)
	group call '���a'		�ֳt�l���Y�쪱�a�����ܦ��a(�ݪ�O $RW 1,000,000)

�������O:
	gt (�����W�D)
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
			return tell(me, (target==me?pnoun(2, me):target->query_idname())+"�ثe���b���󶤥�C\n");
		
		msg = (target==me?pnoun(2, me):target->query_idname())+"�ثe�b�u"+group_leader->query_group_name()+NOR"�v������A�����W��p�U�G\n";
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		group = group_leader->query_group_data();
		
		group_data = sort_array(unique_array(keys(group), (: $(group)[$1] :)), (: $(group)[$1[0]] - $(group)[$2[0]] :));

		foreach(members in group_data)
		{
			switch(group[members[0]])
			{
				case GROUP_LEADER:
					msg += HIC"��"NOR CYN"��\n"NOR;
					break;
				case GROUP_PLAYER:
					msg += HIY"���a"NOR YEL"����\n"NOR;
					break;
				case GROUP_LABOR:
					msg += HIG"���u"NOR GRN"����\n"NOR;
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
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		return tell(me, msg);
	}

	if( sscanf(arg, "create %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");
			
		if( objectp(group_leader) )
		{
			if( me == group_leader )
				return tell(me, pnoun(2, me)+"�w�g�O�u"+me->query_group_name()+"�v��������F�C\n");
			else
				return tell(me, pnoun(2, me)+"�ثe�b�u"+me->query_group_name()+"�v������A"+pnoun(2, me)+"������������A�إ߷s����C\n");
		}
		
		if( noansi_strlen(arg) > 20 )
			return tell(me, "����W�٤��i�W�L 20 �Ӧr���C\n");

		me->create_new_group(arg);
		
		msg(HIY+"$ME"HIY"�إߤF�@�ӥ��s������W�s�u"+arg+NOR HIY"�v�C\n"NOR, me, 0, 1);
		
		return;
	}
	else if( sscanf(arg, "invite %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		target =  present(arg, environment(me)) || find_player(arg);
	
		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"�������إߤ@�ӷs����C\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"�����O��������~�i�H�ܽШ�L���a�έ��u�[�J����C\n");

		if( !objectp(target) )
			return tell(me, "�o����S�� "+arg+" �o�쪱�a�έ��u�C\n");
			
		if( me == target )
			return tell(me, pnoun(2, me)+"���ݭn�ܽЦۤv�[�J����C\n");

		if( !target->is_living() || (!userp(target) && !target->is_module_npc()) )
			return tell(me, pnoun(2, me)+"�L�k�ܽ�"+target->query_idname()+"�[�J����C\n");
		
		if( me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"�w�g�b"+pnoun(2, me)+"������F�C\n");	
		
		if( objectp(target->query_group_leader()) )
			return tell(me, target->query_idname()+"�w�g�b��L������A�L�k�A�ܽСC\n");

		if( target->is_module_npc() )
		{
			string boss = query("boss", target);
			object boss_ob = find_player(boss);
			
			if( !objectp(boss_ob) || !me->is_in_my_group(boss_ob) )
				return tell(me, pnoun(2, me)+"�������ܽ�"+target->query_idname()+"�����D�[�J����C\n");	
	
			members = me->query_group_members();
			
			foreach(member in members)
				tell(member, HIY+target->query_idname()+HIY"�[�J����C\n"NOR);
	
			me->add_group_member(target);
		}
		else
		{
			msg(HIY"$ME"HIY"�ܽ�$YOU"HIY"�[�J�u"+me->query_group_name()+HIY"�v������C\n"NOR, me, target, 1);
			
			set_temp("group/invite", me, target);
	
			tell(target, HIC"�Y���[�J����п�J�ugroup accept�v�B�ڵ��[�J�п�J�ugroup reject�v�Τ����z�|�C\n"NOR);
	
			members = me->query_group_members() - ({ me });
			
			foreach(member in members)
				tell(member, HIY+me->query_idname()+HIY"�ܽ�"+target->query_idname()+HIY"�[�J����C\n"NOR);
		}

		return;		
	}
	else if( sscanf(arg, "remove %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		target = present(arg) || find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"���b���󶤥���C\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"�����O��������~�i�H�N��L���a�έ��u��X����C\n");

		if( !objectp(target) )
			return tell(me, "�o����S�� "+arg+" �o�쪱�a�έ��u�C\n");
			
		if( me == target )
			return tell(me, pnoun(2, me)+"�L�k�N�ۤv��X����A�Y�n�����Х����涤�������Ϊ����Ѵ�����C\n");

		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"�쥻�N���b"+pnoun(2, me)+"������C\n");	
			
		msg(HIY"$ME"HIY"�N$YOU"HIY"��X����C\n"NOR, me, target, 1);

		if( userp(target) )
		foreach(member in copy(me->query_group_members()))
		{
			if( member->is_module_npc() && query("boss", member) == target->query_id(1) )
				member->do_command("group leave");
		}

		me->remove_group_member(target);

		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�N"+target->query_idname()+HIY"��X����C\n"NOR);

		return;		
	}
	else if( sscanf(arg, "leader %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		target = find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"���b���󶤥���C\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"�����O��������~�i�H���涤�������C\n");

		if( !objectp(target) )
			return tell(me, "�S�� "+arg+" �o�쪱�a�C\n");
			
		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"���b"+pnoun(2, me)+"������C\n");	
			
		tell(me, HIY+pnoun(2, me)+"�N�����������浹"+target->query_idname()+HIY"�C\n"NOR);
		
		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�N�����������浹"+target->query_idname()+HIY"�C\n"NOR);
		
		target->become_group_leader(me);

		return;		
	}
	else if( sscanf(arg, "call %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		target = find_player(arg);

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"���b���󶤥���C\n");

		if( me != group_leader )
			return tell(me, pnoun(2, me)+"�����O��������~�i�H�l�������C\n");

		if( !objectp(target) )
			return tell(me, "�S�� "+arg+" �o�쪱�a�C\n");

		if( !me->is_in_my_group(target) )
			return tell(me, target->query_idname()+"���b"+pnoun(2, me)+"������C\n");	

		// �԰���
		if( COMBAT_D->in_fight(target) )
			return tell(me, target->query_idname()+"�ثe���b�԰����C\n");
		
		// �O�ǤH
		if( !target->valid_move() )
			return tell(me, target->query_idname()+"�ثe�L�k���ʡC\n");
		
		if( same_environment(me, target) )
			return tell(me, target->query_idname()+"�w�g�b�o�̤F�C\n");

		if( AREA_D->query_area_info(environment(me)->query_area(), "need_flag") )
			return tell(me, pnoun(2, me)+"�Ҧb���a�ϬO�S��ϰ�A�L�k�ϥζ���l���C\n");
	
		if( !me->spend_money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST) )
			return tell(me, pnoun(2, me)+"���W�������� "HIY+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+NOR"�C\n");
			
		msg(me->query_idname()+"��O "+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+" ���O����X�A�N$ME�l����"+pnoun(3, me)+"����C\n", target, 0, 1);
		
		target->move_to_environment(me);

		msg("$ME��O "+money(MONEY_D->query_default_money_unit(), GROUP_CALL_COST)+" ���O����X�A�N$YOU�l����$ME����C\n", me, target, 1);

		return;		
	}
	else if( sscanf(arg, "rename %s", arg) == 1 )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"���b���󶤥���C\n");
		
		if( me != group_leader )
			return tell(me, pnoun(2, me)+"�����O��������~�i�H��ﶤ��W�١C\n");
			
		if( noansi_strlen(arg) > 20 )
			return tell(me, "����W�٤��i�W�L 20 �Ӧr���C\n");
		
		tell(me, HIY+pnoun(2, me)+"�N����W�٧�אּ�u"+arg+HIY"�v�C\n"NOR);

		members = me->query_group_members() - ({ me });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�N����W�٧�אּ�u"+arg+HIY"�v�C\n"NOR);
		
		me->set_group_name(arg);

		return;
	}
	else if( arg == "accept" )
	{
		object inviter = query_temp("group/invite", me);

		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");
		
		if( !objectp(inviter) || !inviter->is_group_leader() )
			return tell(me, "�å����H�ܽ�"+pnoun(2, me)+"�[�J����A�άO�Ӫ��a�w�g�Ѵ�����C\n");
		
		msg(HIY"$ME"HIY"�[�J$YOU"HIY"���u"+inviter->query_group_name()+HIY"�v����C\n"NOR, me, inviter, 1);
		
		members = inviter->query_group_members() - ({ inviter });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�[�J����C\n"NOR);

		inviter->add_group_member(me);

		delete_temp("group/invite", me);	
		
		return;
	}
	else if( arg == "reject" )
	{
		object inviter = query_temp("group/invite", me);

		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		if( !objectp(inviter) || !inviter->is_group_leader() )
			return tell(me, "�å����H�ܽ�"+pnoun(2, me)+"�[�J����A�άO�Ӫ��a�w�g�Ѵ�����C\n");
		
		msg(HIY"$ME"HIY"�ڵ��[�J$YOU"HIY"���u"+inviter->query_group_name()+HIY"�v����C\n"NOR, me, inviter, 1);
		
		members = inviter->query_group_members() - ({ inviter });
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�ڵ��[�J����C\n"NOR);

		delete_temp("group/invite", me);
		
		return;
	}
	else if( arg == "leave" )
	{
		if( !objectp(group_leader) )
		{
			if( me->is_npc() )
				return me->do_command("say �ڤ��b���󶤥���C\n");
			else
				return tell(me, pnoun(2, me)+"���b���󶤥���C\n");
		}

		if( me == group_leader )
			return tell(me, pnoun(2, me)+"�������N�����������浹�t�@�W���a�~�i�H���}����A�άO�����Ѵ�����C\n");

		if( me->is_npc() )
			msg(HIY"$ME"HIY"���}�F"+group_leader->query_idname()+HIY"���u"+group_leader->query_group_name()+HIY"�v����C\n"NOR, me, 0, 1);
		else
			tell(me, HIY+pnoun(2, me)+"���}�F"+group_leader->query_idname()+HIY"���u"+group_leader->query_group_name()+HIY"�v����C\n"NOR);

		foreach(member in copy(group_leader->query_group_members()))
		{
			if( !objectp(member) ) continue;

			if( member->is_module_npc() && query("boss", member) == me->query_id(1) && userp(me) )
				member->do_command("group leave");
		}

		group_leader->remove_group_member(me);

		members = group_leader->query_group_members();
		
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"���}����C\n"NOR);
			
		return;	
	}
	else if( arg == "dismiss" )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���榹�R�O�C\n");

		if( !objectp(group_leader) )
			return tell(me, pnoun(2, me)+"���b���󶤥���C\n");
		
		if( me != group_leader )
			return tell(me, pnoun(2, me)+"���O�����A�L�k�Ѵ�����C\n");
			
		tell(me, HIY+pnoun(2, me)+"�Ѵ��F��ӡu"+me->query_group_name()+HIY"�v����C\n"NOR);

		members = group_leader->query_group_members() - ({ me });
		foreach(member in members)
			tell(member, HIY+me->query_idname()+HIY"�Ѵ��F��Ӷ���C\n"NOR);

		me->dismiss_group();
		
		return;
	}


	return tell(me, help);

}
