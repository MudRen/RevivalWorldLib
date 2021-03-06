/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : grandam.c
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
			(: do_command("sweat") :)   : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	if( !msg || !msg[0] )
		do_command("say 嗯？"+pnoun(2, me)+"說什麼我聽不清楚");
	
	else if( strsrch(msg, "柯文") != -1 )
		do_command("say 那是誰呀？我不認識耶");
		
	else if( strsrch(msg, "凡環") != -1 )
		do_command("say 可憐的孩子，凡環他就住在山腳下，只不過是個簡樸的農夫，但竟然在家中被人殺害");
		
	else if( strsrch(msg, "玲羽") != -1 )
	{
		do_command("sigh 玲羽阿...");
		do_command("say 最近村子裡鬧得沸沸揚揚，大家都說是玲羽殺了凡環後便逃逸無蹤，但我知道玲羽她不會做這種事的");
		do_command("say 我是看著她長大的，她是這麼善解人意怎麼可能會做那種事...");
	}
	else if( strsrch(msg, "人參") != -1 )
		do_command("say 哦？"+pnoun(2, me)+"是說野山人參嗎？那是玲羽在一處陰溼山洞中發現的，但那株人參似乎並沒有什麼特別的，玲羽那時候看到也就隨手取了回來");
	else if( strsrch(msg, "山洞") != -1 )
		do_command("say 我也不清楚那個山洞在哪裡，我只知道那是玲羽前一陣子到深山中找尋治療凡環的藥材時找到的");
	else if( strsrch(msg, "藥材") != -1 )
		do_command("say 說到藥材就是我的專長了，您要不要也來試試阿，我這裡可有不少種藥材，包您藥到病除阿，哇哈哈");
	else if( strsrch(msg, "試") != -1 )
	{
		do_command("say 來來來，這帖藥保證強身健體");
		msg("$ME被$YOU強灌了一帖味道辛辣的藥草，$ME只覺一陣頭暈目眩。\n", me, this_object(), 1);
		me->faint();
		do_command("say 咦？看來這帖藥不大對，我就說嘛...年輕人身體好好的就不要亂吃藥");
	}
	else if( strsrch(msg, "治療") != -1 )
	{
		do_command("say 凡環的病很奇怪，我也曾經去看過他幾次，但也無從斷定他的病因");
		do_command("say 或許...憶之地一位美麗的仙女有辦法治得好他，不過現在講這些都太遲了");
	}
	else if( strsrch(msg, "山賊") != -1 )
		do_command("say 這個年頭還有什麼山賊嗎？我已經好久沒看見了");
	else if( strsrch(msg, "錦囊") != -1 )
		do_command("say 那個錦囊是玲羽繡給凡環用的，她的那雙手真的是巧阿");
	else if( strsrch(msg, "仙女") != -1 )
	{
		do_command("say 她實在太漂亮了，醫術又是極為高明，所以我總是稱她為仙女，但那位仙女總是飄忽不定，我也不曉得她在哪裡");
		
		if( me->query_quest_finish(QUEST_YIN_LING_YU) && me->query_quest_step(QUEST_YIN_LING_YU_2) == 0)
		{
			me->set_quest_note(QUEST_YIN_LING_YU_2, HIC"茵玲羽"NOR CYN"之謎續章"NOR, "遇見了採藥老太婆，知道了憶之地仙女的存在。");
			me->set_quest_step(QUEST_YIN_LING_YU_2, 1);
		}
	}
	else
		do_command("say 嗯？"+pnoun(2, me)+"說什麼我聽不清楚");
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	process_function(({
		1,
		(: do_command("say 這是什麼？我不要這個東西...") :),
		1,
		(: do_command("drop all "+$(ob)->query_id(1)) :),
	}));
}


void create()
{
	set_idname("grandam", "老太婆");
	set_temp("status", ([ HIY"辛勤"NOR YEL"採藥"NOR : -1 ]));

	set("long", "一位在山中長住並以採藥為生的老太婆，或許她知道一些事情，試著問問看(say)她吧。");
	set("unit", "位");
	set("age", 62);
	set("gender", FEMALE_GENDER);
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 500);
	set("attr/phy", 200);
	set("attr/int", 300);
	set("attr/agi", 500);
	set("attr/cha", 10);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 8000);
	set("abi/health/cur", 8000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();

	startup_living();
}
