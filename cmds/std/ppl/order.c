/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : order.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-19
 * Note   : 命令指令
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <npc.h>
#include <skill.h>
#include <secure.h>

inherit COMMAND;

#define MAX_SOLDIER		5
#define MAX_SPORTER		10
#define MAX_ADVENTURER	5

string help = @HELP
   這可以讓你下命令給自己的人力，目前開放的命令如下：

特殊命令指令
  order '員工' to number '編號'		- 設定員工編號，可重覆編號，labor 列表依此編號排序
  order '員工' to name '中文'		- 幫員工取中文名字
  order '員工' to id '英文'		- 幫員工取英文名字

  order '員工' to work '工作'		- 命令員工從事工作(例如店員為 clerk)
                                          工作種類有 clerk / worker / rd / soldier / sporter / adventurer
  order '員工' to work -d		- 命令員工停止工作
  
  order '員工' to learn '技能'		- 命令員工向您學習技能(您本身必須具備口才技能與教學技能)
  order '員工' to learn '技能' 50%	- 一次耗費 50% 的精神來教導員工
 
  order '員工' to giveup '技能' 10	- 命令員工將某項技能降級為 10 級(降為 0 級則是全部放棄)

標準系統指令
  order '員工' to command		- 查詢員工可以使用的系統指令
  order '員工' to '指令'		- 命令員工執行某個系統指令(指令用法與玩家平常的用法相同)


相關指令: labor
HELP;

private void do_command(object me, string arg)
{
	string id, key, value;
	object npc, env;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要命令誰？\n");

	if( sscanf(arg, "%s to %s", id, key) != 2 )
		return tell(me, pnoun(2, me)+"想要命令誰做什麼事？(例 order boy to name 男孩)\n");
		
	env = environment(me);

	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( BATTLEFIELD_D->inside_battlefield(me) )
		return tell(me, "戰場中無法使用命令指令。\n");

	if( !objectp(npc = present(id, env)) )
		return tell(me, "這裡沒有 "+id+" 這個人。\n");
	
	if( !npc->is_living() )
		return tell(me, pnoun(2, me)+"無法命令一個無生命的東西。\n");
	
	if( !npc->is_npc() || (!wizardp(me) && query("boss", npc) != me->query_id(1)) )
		return tell(me, npc->query_idname()+"並不是"+pnoun(2, me)+"雇用的。\n");

	sscanf(key, "%s %s", key, value);

	if( npc->is_faint() )
		return tell(me, pnoun(2, me)+"無法命令昏倒中的員工做任何事情。\n");
	else if( npc->is_dead() )
		return tell(me, pnoun(2, me)+"無法命令死亡的員工做任何事情。\n");
		
	npc->startup_living();

	switch(lower_case(key))
	{
		case "name":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要幫"+npc->query_idname()+"取什麼中文名字？\n");

			value = kill_repeat_ansi(value+NOR);

			if( !is_chinese(value) )
				return tell(me, "請"+pnoun(2, me)+"用「中文」取名字。\n");
	
			if( noansi_strlen(value) > 12 || noansi_strlen(value) < 2 )
				return tell(me, "中文名字必須在 1 到 6 個中文字之間。\n");

			msg("$ME吩咐$YOU道："+pnoun(2, npc)+"以後中文名子就改名叫做「"+value+"」吧！\n", me, npc, 1);
			npc->set_idname(0, value);
			npc->do_command("say 謝謝老闆幫我取新的中文名字，我一定會不負您的期望...");

			npc->save();				
			break;
		}
		case "id":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要幫"+npc->query_idname()+"取什麼英文名字？\n");
			
			value = replace_string(kill_repeat_ansi(lower_case(value)+NOR)," ", "");
			
			if( !is_english(value) )
				return tell(me, "請"+pnoun(2, me)+"用「英文」取名字。\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "英文名字必須在 3 到 16 個英文字數之間。\n");
				
			if( user_exists(remove_ansi(value)) )
				return tell(me, "不得取名為現有玩家的名字。\n");
				
			msg("$ME吩咐$YOU道："+pnoun(2, npc)+"以後英文名子就改名叫做「"+value+"」吧！\n", me, npc, 1);
			npc->set_idname(value, 0);
			npc->do_command("say 謝謝老闆幫我取新的英文名字，我一定會不負您的期望...");
			
			npc->save();
			break;
		}
		case "work":
		{
			int job;
			string job_name;

			if( COMBAT_D->in_fight(npc) )
				return tell(me, npc->query_idname()+"正在戰鬥中，無法進行工作指派。\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要叫"+npc->query_idname()+"做什麼樣的工作(clerk, worker, rd, soldier)？\n");

			if( time() < query("last_die_time", npc) + 300 )
				return tell(me, "員工死亡後必須等待 5 分鐘才能重新指派工作。\n");

			if( query("jog/type", npc) == SPORTER && BASEBALL_D->is_playing(npc) )
				return tell(me, npc->query_idname()+"正在進行球賽，目前無法轉換工作。\n");

			value = lower_case(value);

			switch( value )
			{
				case "-d":
					if( !query("job", npc) )
						return tell(me, npc->query_idname()+"原本就沒有在進行工作。\n");
					
					msg("$ME叫$YOU停止手邊所有工作。\n", me, npc, 1);
					
					delete("job", npc);

					if( objectp(query_temp("assisting", npc)) )
						npc->do_command("assist -d");

					npc->do_command("say 是的老闆。");
					return;
				case "rd":
					job = RD;		
					job_name = HIR"研"NOR RED"發"NOR;
					break;
				case "worker":
					job = WORKER;
					job_name = HIY"工"NOR YEL"人"NOR;
					break;
				case "clerk":
				{
					int limit = LABOR_D->get_labor_limit(me);
					
					if( sizeof(LABOR_D->get_labors(me, CLERK)) >= (limit/2)+(limit%2) )
						return tell(me, pnoun(2, me)+"的店員數量已經達員工人數上限的 50%，無法再增派人手。\n");
						
					job = CLERK;
					job_name = HIC"店"NOR CYN"員"NOR;
					break;
				}
				case "adventurer":
				{
					if( sizeof(LABOR_D->get_labors(me, ADVENTURER)) >= MAX_ADVENTURER )
						return tell(me, pnoun(2, me)+"的探險隊數量已經到達 "+MAX_ADVENTURER+" 位，無法再增派人手。\n");
					
					job = ADVENTURER;
					job_name = HIM"探"NOR MAG"險隊"NOR;
					break;
				}
				case "soldier":
				{
					if( sizeof(LABOR_D->get_labors(me, SOLDIER)) >= MAX_SOLDIER )				
						return tell(me, pnoun(2, me)+"的軍人數量已經到達 "+MAX_SOLDIER+" 位，無法再增派人手。\n");
						
					job = SOLDIER;
					job_name = HIG"軍"NOR GRN"人"NOR;
					break;
				}
				case "sporter":
				{			
					if( sizeof(LABOR_D->get_labors(me, SPORTER)) >= MAX_SPORTER	)
						return tell(me, pnoun(2, me)+"的球員數量已經到達 "+MAX_SPORTER+" 位，無法再增派人手。\n");

					job = SPORTER;
					job_name = HIW"球"NOR WHT"員"NOR;
					break;
				}
				default:
					return tell(me, "要做何種工作(clerk, worker, rd, soldier)？\n"NOR);
			}
					
			msg("$ME吩咐$YOU道：從今天起"+pnoun(2, npc)+"就做「"+job_name+"」的工作吧！\n", me, npc, 1);
			npc->do_command("say 好的！我一定會盡心盡力的工作的！");
	
			set("job/name", job_name, npc);
			set("job/type", job, npc);

			if( job == SOLDIER )
			{
				npc->do_command("follow "+me->query_id(1));
				npc->do_command("assist "+me->query_id(1));
			}
			else
			{
				npc->do_command("follow -d");
				if( objectp(query_temp("assisting", npc)) )
					npc->do_command("assist -d");
			}

			npc->save();
			break;
		}
		case "number":
		{
			int number = to_int(value);
			int limit = LABOR_D->get_labor_limit(me);
			
			if( number < 1 || number > limit )
				return tell(me, "目前"+pnoun(2, me)+"的員工編號設定只能介於 1 至 "+limit+" 之間。\n");
			
			set("number", number, npc);
			
			msg("$ME吩咐$YOU道："+pnoun(2, npc)+"的身份編號現在是 "+number+" 號了！\n", me, npc, 1);
			npc->do_command("say 是的，老闆...");
			
			npc->save();
			
			break;	
		}
		case "giveup":
		{
			int npc_level;
			int to_level = 0;

			if( !value )
				return tell(me, pnoun(2, me)+"想要"+npc->query_idname()+"放棄什麼技能？\n");
			
			if( sscanf(value, "%s %d", value, to_level) != 2 )
				return tell(me, pnoun(2, me)+"想要"+npc->query_idname()+"放棄什麼技能至幾級？\n");
							
			if( !SKILL_D->skill_exists(value) )
				return tell(me, "並沒有 "+value+" 這種技能。\n");
							
			npc_level = npc->query_skill_level(value);
			
			if( npc_level < 1 )
				return tell(me, npc->query_idname()+"原本便沒有此項技能。\n");
			
			if( to_level < 0 || to_level >= npc_level )
				return tell(me, npc->query_idname()+"無法將此技能降為 "+to_level+" 級。\n");
				
			if( to_level == 0 )
			{
				msg("$ME吩咐$YOU道：放棄使用"+(SKILL(value))->query_idname()+"的技能吧！\n", me, npc, 1);
				npc->do_command("say 是的，老闆...");
				
				npc->add_skill_exp(value, -1000000000000);
				
				delete("skills/"+value, npc);
			}
			else if( to_level > 0 )
			{
				int from_exp = to_int(query("skills/"+value+"/exp", npc));
				int to_exp = SKILL_D->query_level_exp(to_level);
				
				msg("$ME吩咐$YOU道：將"+(SKILL(value))->query_idname()+"的技能降為 "+to_level+" 級吧！\n", me, npc, 1);
				npc->do_command("say 是的，老闆...");
				
				npc->add_skill_exp(value, (to_exp - from_exp)/(SKILL(value))->exp_multiple());
			}

			npc->save();
			break;
		}
		case "learn":
		{
			int percent;
			int energy;
			int max_level;
			int npc_level;
			int teach_level = me->query_skill_level("teach");
			
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"想要"+npc->query_idname()+"學習什麼技能？\n");

			if( sscanf(value, "%s %d%%", value, percent) == 2 )
			{
				if( percent <= 0 || percent > 100 )
					return tell(me, "請輸入正確的精神耗費百分比。\n");
					
				energy = percent * me->query_energy_max() / 100;
			}
			else
				energy = 50;

			value = lower_case(value);

			if( !SKILL_D->skill_exists(value) )
				return tell(me, "並沒有 "+value+" 這種技能。\n");
	
			npc_level = npc->query_skill_level(value);

			if( teach_level < 100 && npc_level >= teach_level )
				return tell(me, pnoun(2, me)+"的"+(SKILL("teach"))->query_idname()+"等級不足，"+npc->query_idname()+"的"+(SKILL(value))->query_idname()+"等級不能再往上提升。\n");

			max_level = (SKILL(value))->max_level(npc);
			
			if( npc_level >= max_level )
				return tell(me, npc->query_idname()+"的"+(SKILL(value))->query_idname()+"技能等級已經到達上限。\n");

			if( !(SKILL(value))->allowable_learn(npc) )
				return tell(me, npc->query_idname()+"無法學習這項技能。\n");
				
			if( !me->cost_energy(energy) )
				return tell(me, pnoun(2, me)+"的精神不足 "+energy+"，無法再傳授技能了。\n");
							
			msg("$ME耗費 "+energy+" 的精神仔細地傳授$YOU有關"+(SKILL(value))->query_idname()+"的經驗。\n"+npc->query_idname()+"對"+(SKILL(value))->query_idname()+"有了更進一步的瞭解。\n", me, npc, 1);
			
			npc->add_skill_exp(value, pow((me->query_skill_level("eloquence")+1) * energy, 1.001));
			
			npc_level = npc->query_skill_level(value);
			
			if( npc_level >= max_level )
			{
				set("skills/"+value+"/level", max_level, npc);
				set("skills/"+value+"/exp", (SKILL(value))->level_exp(max_level), npc);
				//tell(me, npc->query_idname()+"的"+(SKILL(value))->query_idname()+"技能等級已經不能再往上提升。\n");
			}
			
			npc->delay_save(300);
			break;
		}
		case "command":
		{
			npc->do_command("say 我目前可以遵行的系統指令有："+implode(COMMAND_D->query_available_command(STANDARD), ", "));
			break;
		}

		default:
		{
			msg("$ME命令$YOU：「"HIY+(value?key+" "+value:key)+NOR"」。\n", me, npc, 1);

			if( !wizardp(me) && (COMMAND_D->find_chn_command(key) || COMMAND_D->find_emt_command(key)) )
				return npc->do_command("say 我不能在頻道上發言也不能作任何表情指令");

			npc->do_command(value?key+" "+value:key);
			break;
		}
	}
}