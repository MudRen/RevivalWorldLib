/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quest.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-01-05
 * Note   : quest 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
任務查詢，查詢已完成或未完成的各項任務與任務註解

quest		顯示任務資訊
quest 編號 -d	刪除某項未完成的任務

HELP;

private void do_command(object me, string arg)
{
	int i, num;
	string msg;
	object ob;
	mapping quest;
	int total, totalopen;
	
	if( arg && sscanf(arg, "%d -d", num) == 1  && arg[<2..<1] == "-d")
	{
		int size;
		
		quest = query("quest", me);
		
		size = sizeof(quest);
		
		if( num <= 0 || num > size )
			return tell(me, "沒有這個任務。\n");

		i = 0;
		foreach(string questname, mapping data in quest)
		{
			i++;
			
			if( i == num )
			{
				if( me->query_quest_finish(questname) )
					return tell(me, "任務「"+questname+"」已經完成，無法刪除。\n");
					
				me->delete_quest_data(questname);
				me->save();

				return tell(me, "刪除任務「"+questname+"」完成。\n");
			}	
		}
		return;
	}
		
	if( arg && wizardp(me) && objectp(ob = find_player(arg)) )
	{
		if( !sizeof(quest = query("quest", ob)) )
			return tell(me, ob->query_idname()+"尚未完成任何任務。\n");
			
		msg = ob->query_idname()+"的任務資訊如下(共 "+sizeof(quest)+" 項任務)：\n";
	}
	else
	{
		ob = me;
		
		if( !sizeof(quest = query("quest", ob)) )
			return tell(me, pnoun(2, me)+"尚未完成任何任務。\n");

		msg = pnoun(2, me)+"的任務資訊如下(共 "+sizeof(quest)+" 項任務)：\n";
	}
	msg += WHT"────────────────────────────\n"NOR;
	foreach(string questname, mapping data in quest)
	{
		if( undefinedp(data["name"]) ) continue;

		msg += sprintf(HIW"%-4s"NOR"%s %s\n    "HIB"筆記"NOR"：%s\n"NOR, (++i)+".", data["name"]+NOR YEL" ─ "HIY+TIME_D->replace_ctime(data["time"])+NOR, (data["finish"]? GRN"("HIG"已完成，總共步驟："+(data["step"]?data["step"]+"/":"")+QUEST_D->query_quest_steps(questname)+"，完成次數："+data["finish"]+NOR GRN")"NOR:RED"("HIR"未完成，目前步驟:"+data["step"]+"/"+QUEST_D->query_quest_steps(questname)+NOR RED")"NOR), data["note"]?data["note"]:"無註解");
	}
	
	total = QUEST_D->query_total_quests_amount();
	totalopen = QUEST_D->query_total_opened_quests_amount();
	
	msg += WHT"────────────────────────────\n總任務數："+HIW+total+NOR WHT"、完成開放："+HIW+totalopen+NOR WHT"、測試中："+HIW+(total-totalopen)+"\n"NOR;
	me->more(msg);
}