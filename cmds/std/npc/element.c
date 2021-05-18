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
				return tell(me, pnoun(2, me)+"目前正處於「"+STANCE_D->query_stance_name(stance)+"」姿態。\n", me, 0, 1);
				break;
			default:
				return tell(me, pnoun(2, me)+"目前沒有進入任何戰鬥姿態。\n", me, 0, 1);
				break;
		}
	}
	
	if( query("combat/stance/time", me) > time() - 180 )
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
		case "defend":
		case "speed":
		case "medic":
		case "attack-adv":
		case "defend-adv":
		case "speed-adv":
		case "medic-adv":
			msg("$ME開始進入「"+STANCE_D->query_stance_name(arg)+"」的戰鬥姿態。\n", me, 0, 1);
			break;
		case "-d":
			msg("$ME取消了原有的戰鬥姿態。\n", me, 0, 1);
			delete("combat/stance/type", me);
			set("combat/stance/time", time(), me);
			return;
		default:
			return tell(me, "沒有 "+arg+" 這種戰鬥姿態。\n");
			break;
	}
	
	set("combat/stance/type", arg, me);
	set("combat/stance/time", time(), me);
}
