/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : yin_ling_yu.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-13
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	系統預設之 NPC 反應函式
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <quest.h>

inherit STANDARD_PERSON;

void do_command(string cmd);

void set_behavior()
{
	set("behavior/random", 
	    ([
		(: do_command("inn 為什麼會這樣...") :)   : 50,
		(: do_command("cold") :) : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{	
	if( !msg || !msg[0] )
		do_command("say "+pnoun(2, me)+"說什麼？");
	
	else if( me->query_quest_step(QUEST_YIN_LING_YU_2) == 2 && strsrch(msg, "人參") != -1 )
	{
		do_command("say "+pnoun(2, me)+"是說我給"+pnoun(2, me)+"的那把人參嗎？那是我在微光森林西方的一處隱密山洞中發現的");
		do_command("shiver");
		do_command("say 如果...如果不是為了凡環，我根本沒有勇氣進入那可怕的山洞中...嗚...凡環");
		
		me->set_quest_step(QUEST_YIN_LING_YU_2, 3);
		me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "茵玲羽向"+pnoun(2, me)+"說起人參的來歷是在微光森林西方的一個隱密山洞中。");
	}
	else if( strsrch(msg, "柯文") != -1 )
		do_command("sob "+me->query_id(1)+" 對！對！易柯文！就是他，快去救他阿！他在古厝那...嗚...快阿！");
	else if( strsrch(msg, "凡環") != -1 )
		do_command("sob "+me->query_id(1)+" 凡環他已經死了...原來...原來易柯文他...嗚...都是我的錯...");
	else if( strsrch(msg, "人參") != -1 )
		do_command("say 人參？"+pnoun(2, me)+"有那把野山人參嗎？可以給我瞧瞧嗎？");
	else
		do_command("say "+pnoun(2, me)+"說什麼？");
}

void reply_emote(object me, string verb, string args)
{
	do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;
	
	do_command("say 人呢？這世界上都沒人要理我了嗎...");
	
	if( objectp(me) )
	{
		tell(me, this_object()->query_idname()+"在遠方呼喊著：人呢？這世界上都沒人要理我了嗎...\n");
		delete_temp("quest/"QUEST_YIN_LING_YU"/running", me);
	}
	
	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;
	
	if( me->query_quest_step(QUEST_YIN_LING_YU)==3 && query("behavior") && base_name(ob) == "/quest/yin_ling_yu/wild_ginseng" )
	{
		destruct(ob);
		
		if( query_temp("quest/"QUEST_YIN_LING_YU"/running", me) )
			return;
		
		tell(me, HIY"注意：以下故事進行過程中若離開此地或離線將導致任務失敗。\n"NOR);

		set_temp("quest/"QUEST_YIN_LING_YU"/running", 1, me);
		
		// 停止主動行為
		delete_behavior();
		
		process_function(({
			2,
			(: target_gone($(me)) :),
			(: msg("$ME呆呆地看著野山人參，$YOU告訴她易柯文已經死亡的訊息。\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$ME開始失魂落魄地說起一段故事。\n\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"曾經有個平凡的農夫，名叫"HIW"良凡環"NOR"\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"大著茵玲羽三歲的他是茵玲羽的鄰居，從小兩人就是"HIM"青梅"NOR MAG"竹馬"NOR"\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"但在茵玲羽十六歲的時候，茵玲羽便和家人搬到山上過著"HIG"茶"NOR GRN"農"NOR"的生活\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽知道良凡環身體不好，卻做著如此粗重的工作，便時常到山腳下照顧他\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"就這樣過了好幾年\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽二十二歲了，"HIY"亭亭"NOR YEL"玉立"NOR"的她依然常常下山照顧良凡環\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"但事情就發生在前幾天\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"良凡環突然對著茵玲羽"HIW"大吼"NOR"：妳實在很煩，不要再來了好不好\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"說著說著良凡環轉過身咳嗽著，他的手上佈著一絲"HIR"鮮"NOR RED"血"NOR"，但茵玲羽沒有看見\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽非常傷心地回到山中\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"難道良凡環不再喜歡她了嗎\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"這時在山路上\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"遇到了正在山區獨自郊遊的"HIW"易柯文"NOR"\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文是個成長在繁榮城市中的"HIG"俊俏"NOR GRN"青年"NOR"\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"雖然是初次見面\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"但易柯文見到了神情難過的茵玲羽\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"依然大方地安慰著茵玲羽\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽也知道易柯文的心思\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"馬上便跟易柯文表明了她已經有男朋友的事\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文笑著點了點頭\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"天色"WHT"已晚"NOR"\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文在下山前問了茵玲羽有沒有什麼需要幫忙的地方\n", this_object(), $(me), 1) :),
			7,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽想了想\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"便拜託了易柯文順道幫她送一些"HIY"野山"NOR YEL"人參"NOR"到山腳下的良凡環家裡\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文親切的點了點頭，提著一把"HIY"野山"NOR YEL"人參"NOR"離開了\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽心情也好了許多，便回到家裡\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"隔日一早\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽又擔心地下山去找良凡環，希望和他和好如初\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"豈知一到良凡環家中\n", this_object(), $(me), 1) :),
			3,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"卻見良凡環"HIR"心"NOR RED"口"NOR"插著把刀，已然氣絕\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"見到這一幕的茵玲羽崩潰了，她不瞭解為什麼會發生這種事，是誰謀殺了他！？\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽想著，腦海中浮出一個人臉\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文！他竟然是個"HIY"殺"NOR YEL"人犯"NOR"！\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽一跤跌坐在良凡環屍體的身旁，是她害了他\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"她哭著，"HIM"怨"NOR MAG"恨"NOR"的心讓她拿起刀子\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"延著門口的"HIR"血"NOR RED"跡"NOR"試圖尋找易柯文的下落\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"繞了好幾個彎角\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"突然眼前出現一個男子，正躲藏在一個"HIW"古"NOR WHT"厝"NOR"後\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"正是昨天那個易柯文\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"見到了滿身鮮血的易柯文\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"就是他殺了良凡環！\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽"WHT"顫抖"NOR"著，不發一語地走到易柯文身後\n", this_object(), $(me), 1) :),
			8,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"猛然拿刀往易柯文身上刺去\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"刀子就這樣一聲不響地進了易柯文的身體\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文痛苦地大叫，"HIY"絕"NOR YEL"望"NOR"地回頭看了看\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"竟然是她，這是為什麼...\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"突然古厝的另一端出現一群"HIW"山"NOR WHT"賊"NOR"大喊著：在那！那個人在那！追阿！！\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文將茵玲羽的身子奮力一推\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"反手拔出背後的刀子，"HIR"鮮"NOR RED"血"NOR"直噴而出，濺得茵玲羽滿身鮮血\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文對著茵玲羽撕聲大喊："HIY"跑阿！快跑！"NOR"\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"說著便拿著刀子往山賊那衝去...\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽嚇壞了，她不知道是怎麼一回事，為什麼會這樣，眼前只見易柯文朦朧的身驅拿著刀子正在和山賊們打鬥著\n", this_object(), $(me), 1) :),
			10,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"易柯文遠遠傳來嘶喊聲："HIY"快跑！！趕快跑阿！！"NOR"\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"茵玲羽被嚇得轉身就跑，跑過了"HIG"田"NOR GRN"野"NOR"，跑過了"HIB"山"NOR BLU"澗"NOR"，眼淚從沒有停過\n", this_object(), $(me), 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"終於"WHT"氣盡力竭"NOR"，倒在地上\n", this_object(), $(me), 1) :),
			12,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"也不知過了多久\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg(HIY"故事"NOR YEL" - "NOR"醒來後便見到了$YOU...\n\n", this_object(), $(me), 1) :),
			4,
			(: target_gone($(me)) :),
			(: msg("$ME斷斷續續地講完這段故事，不斷流出的眼淚中充滿著悔恨\n", this_object(), $(me), 1) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$ME把手中的野山人參交給了$YOU\n", this_object(), $(me), 1) :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say 這把野山人參我已經沒用了，就給了"+pnoun(2, $(me))+"吧 ") :),
			(: new("/quest/yin_ling_yu/tear_wild_ginseng")->move($(me)) :),
			(: $(me)->set_quest_finish(QUEST_YIN_LING_YU, QUEST_YIN_LING_YU_NAME, "遇見了『茵玲羽』，幫她找到了易柯文，但易柯文已經死了。") :),
			(: delete_temp("quest/"QUEST_YIN_LING_YU"/running", $(me)) :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" 解開了「"+QUEST_YIN_LING_YU_NAME+"」", $(me)) :),
			6,
			(: target_gone($(me)) :),
			(: msg("$YOU看著手上把野山人參上浸滿著淚水，$YOU被這段悲慘的故事震撼了，久久不發一語。\n", this_object(), $(me), 1) :),
			
			6,
			(: target_gone($(me)) :),
			(: msg("回頭一看只見茵玲羽呆呆地看著遠方，$YOU覺得就讓她靜一靜比較好...。\n", this_object(), $(me), 1) :),
			// 恢復主動行為
			(: set_behavior() :),
		    }));
	}
	else
	{
		process_function(({
			1,
			(: do_command("say 這是什麼？我不要這個東西...") :),
			1,
			(: do_command("drop all "+$(ob)->query_id(1)) :),
		    }));
	}
}


void create()
{
	set_idname("yin ling yu",HIC"茵"NOR CYN"玲羽"NOR);
	set_temp("status", ([HIC"絕望"NOR CYN"悔恨"NOR:-1]));
	
	set("long", "一個傷心欲絕的女子，不知道她為何如此傷心。");
	set("unit", "位");
	set("age", 22);
	set("gender", FEMALE_GENDER);
	set("heartbeat", 1); // 永久性心跳
	
	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();
	
	startup_living();
}
