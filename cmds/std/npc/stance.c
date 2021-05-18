/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stance.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-31
 * Note   : stance ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
�o�ӫ��O�i�H�Ψӧ��ܦۤv���԰����A�A�P�@�ɶ��u��i�J�@�ؾ԰����A�C

stance attack		- �ҥΧ������A�A���ɧ����O
stance defend		- �ҥΨ��m���A�A���ɨ��m�O
stance speed		- �ҥγt�׫��A�A���ɧ����t��
stance medic		- �ҥ��������աA���������ĪG
stance attack-adv	- �ҥλR�����A�A���ɧ����O
stance defend-adv	- �ҥγ��g���A�A���ɨ��m�O
stance speed-adv	- �ҥίe�����A�A���ɧ����t��
stance medic-adv	- �ҥε������աA���������ĪG
stance -d		- �����԰����A
HELP;

private string get_description(string stance)
{
	switch(stance)
	{
		case "attack":
		case "attack-adv":
			return "���u"+STANCE_D->query_stance_name("speed")+"�B"+STANCE_D->query_stance_name("speed-adv")+"�v���A���o�u�աA��ԧ����O�W�j�� "HIR"120%"NOR"�C\n"+
				   "���u"+STANCE_D->query_stance_name("defend")+"�B"+STANCE_D->query_stance_name("defend-adv")+"�v���A����o��A��ԧ����O��z�� "HIG"80%"NOR"�C\n";
			break;
		case "defend":
		case "defend-adv":
			return "���u"+STANCE_D->query_stance_name("attack")+"�B"+STANCE_D->query_stance_name("attack-adv")+"�v���A���o�u�աA��ԧ����O�W�j�� "HIR"120%"NOR"�C\n"+
				   "���u"+STANCE_D->query_stance_name("speed")+"�B"+STANCE_D->query_stance_name("speed-adv")+"�v���A����o��A��ԧ����O��z�� "HIG"80%"NOR"�C\n";
			break;
		case "speed":
		case "speed-adv":
			return "���u"+STANCE_D->query_stance_name("defend")+"�B"+STANCE_D->query_stance_name("defend-adv")+"�v���A���o�u�աA��ԧ����O�W�j�� "HIR"120%"NOR"�C\n"+
				   "���u"+STANCE_D->query_stance_name("attack")+"�B"+STANCE_D->query_stance_name("attack-adv")+"�v���A����o��A��ԧ����O��z�� "HIG"80%"NOR"�C\n";
			break;
		case "medic":
		case "medic-adv":
			return "";
			break;
		default:
			return "";
			break;
	}
}

private void do_command(object me, string arg)
{
	string skill;
	string stance = query("combat/stance/type", me);

	if( !arg || !arg[0] )
	{
		switch(stance)
		{
			case "attack":
			case "defend":
			case "speed":
			case "medic":
			case "attack-adv":
			case "defend-adv":
			case "speed-adv":
			case "medic-adv":
				return tell(me, pnoun(2, me)+"�ثe���B��u"+STANCE_D->query_stance_name(stance)+"�v���A�C\n"+get_description(stance), me, 0, 1);
				break;
			default:
				return tell(me, pnoun(2, me)+"�ثe�S���i�J����԰����A�C\n", me, 0, 1);
				break;
		}
	}
	
	if( !wizardp(me) && query("combat/stance/time", me) > time() - 180 )
		return tell(me, pnoun(2, me)+"�������� "+(180 - time() + query("combat/stance/time", me))+" ���~��A�ഫ�԰����A�C\n");

	if( strsrch(arg, "-adv") == -1 )
		skill = arg+"-stance";
	else
		skill = replace(arg, "-adv", "-stance-adv");

	if( arg != "-d" && !me->query_skill_level(skill) )
		return tell(me, pnoun(2, me)+"�������ǲ߬������A�ޯ�C\n");
		
	
	switch(arg)
	{
		case "attack":
		case "attack-adv":
			msg("$ME�}�l�i�J�u"+STANCE_D->query_stance_name(arg)+"�v���԰����A�C\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "defend":
		case "defend-adv":
			msg("$ME�}�l�i�J�u"+STANCE_D->query_stance_name(arg)+"�v���԰����A�C\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "speed":
		case "speed-adv":
			msg("$ME�}�l�i�J�u"+STANCE_D->query_stance_name(arg)+"�v���԰����A�C\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "medic":
		case "medic-adv":
			msg("$ME�}�l�i�J�u"+STANCE_D->query_stance_name(arg)+"�v���԰����A�C\n", me, 0, 1);
			break;
		case "-d":
			msg("$ME�����F�즳���԰����A�C\n", me, 0, 1);
			delete("combat/stance/type", me);
			set("combat/stance/time", time(), me);
			me->reset_buff_cache();
			return;
		default:
			return tell(me, "�S�� "+arg+" �o�ؾ԰����A�C\n");
			break;
	}
	
	set("combat/stance/type", arg, me);
	set("combat/stance/time", time(), me);
	me->reset_buff_cache();
}
