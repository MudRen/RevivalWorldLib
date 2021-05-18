/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stance.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-31
 * Note   : stance 指令
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
這個指令可以用來改變自己的戰鬥姿態，同一時間只能進入一種戰鬥姿態。

stance attack		- 啟用攻擊姿態，提升攻擊力
stance defend		- 啟用防禦姿態，提升防禦力
stance speed		- 啟用速度姿態，提升攻擊速度
stance medic		- 啟用醫療姿勢，提升醫療效果
stance attack-adv	- 啟用舞火姿態，提升攻擊力
stance defend-adv	- 啟用堰土姿態，提升防禦力
stance speed-adv	- 啟用疾風姿態，提升攻擊速度
stance medic-adv	- 啟用翔水姿勢，提升醫療效果
stance -d		- 取消戰鬥姿態
HELP;

private string get_description(string stance)
{
	switch(stance)
	{
		case "attack":
		case "attack-adv":
			return "對於「"+STANCE_D->query_stance_name("speed")+"、"+STANCE_D->query_stance_name("speed-adv")+"」姿態取得優勢，對戰攻擊力增強為 "HIR"120%"NOR"。\n"+
				   "對於「"+STANCE_D->query_stance_name("defend")+"、"+STANCE_D->query_stance_name("defend-adv")+"」姿態受到牽制，對戰攻擊力減弱為 "HIG"80%"NOR"。\n";
			break;
		case "defend":
		case "defend-adv":
			return "對於「"+STANCE_D->query_stance_name("attack")+"、"+STANCE_D->query_stance_name("attack-adv")+"」姿態取得優勢，對戰攻擊力增強為 "HIR"120%"NOR"。\n"+
				   "對於「"+STANCE_D->query_stance_name("speed")+"、"+STANCE_D->query_stance_name("speed-adv")+"」姿態受到牽制，對戰攻擊力減弱為 "HIG"80%"NOR"。\n";
			break;
		case "speed":
		case "speed-adv":
			return "對於「"+STANCE_D->query_stance_name("defend")+"、"+STANCE_D->query_stance_name("defend-adv")+"」姿態取得優勢，對戰攻擊力增強為 "HIR"120%"NOR"。\n"+
				   "對於「"+STANCE_D->query_stance_name("attack")+"、"+STANCE_D->query_stance_name("attack-adv")+"」姿態受到牽制，對戰攻擊力減弱為 "HIG"80%"NOR"。\n";
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
				return tell(me, pnoun(2, me)+"目前正處於「"+STANCE_D->query_stance_name(stance)+"」姿態。\n"+get_description(stance), me, 0, 1);
				break;
			default:
				return tell(me, pnoun(2, me)+"目前沒有進入任何戰鬥姿態。\n", me, 0, 1);
				break;
		}
	}
	
	if( !wizardp(me) && query("combat/stance/time", me) > time() - 180 )
		return tell(me, pnoun(2, me)+"必須等待 "+(180 - time() + query("combat/stance/time", me))+" 秒後才能再轉換戰鬥姿態。\n");

	if( strsrch(arg, "-adv") == -1 )
		skill = arg+"-stance";
	else
		skill = replace(arg, "-adv", "-stance-adv");

	if( arg != "-d" && !me->query_skill_level(skill) )
		return tell(me, pnoun(2, me)+"必須先學習相關姿態技能。\n");
		
	
	switch(arg)
	{
		case "attack":
		case "attack-adv":
			msg("$ME開始進入「"+STANCE_D->query_stance_name(arg)+"」的戰鬥姿態。\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "defend":
		case "defend-adv":
			msg("$ME開始進入「"+STANCE_D->query_stance_name(arg)+"」的戰鬥姿態。\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "speed":
		case "speed-adv":
			msg("$ME開始進入「"+STANCE_D->query_stance_name(arg)+"」的戰鬥姿態。\n", me, 0, 1);
			tell(me, get_description(arg));
			break;
		case "medic":
		case "medic-adv":
			msg("$ME開始進入「"+STANCE_D->query_stance_name(arg)+"」的戰鬥姿態。\n", me, 0, 1);
			break;
		case "-d":
			msg("$ME取消了原有的戰鬥姿態。\n", me, 0, 1);
			delete("combat/stance/type", me);
			set("combat/stance/time", time(), me);
			me->reset_buff_cache();
			return;
		default:
			return tell(me, "沒有 "+arg+" 這種戰鬥姿態。\n");
			break;
	}
	
	set("combat/stance/type", arg, me);
	set("combat/stance/time", time(), me);
	me->reset_buff_cache();
}
