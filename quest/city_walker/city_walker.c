/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_walker.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-27
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
		(: do_command("sing") :)   : 50,
	    ]));
}

void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	object ob;

	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "城市") != -1 )
	{
		do_command("say 城市是個人類文明的大融爐...希望在我有生之年裡可以遊遍世界各大城市...看盡各種人的生活...。");
		return;
	}
		
	if( strsrch(msg, "遊") != -1 )
	{
		do_command("say 啊...是阿，旅遊是我最喜歡的一件事了...");
		do_command("say 還記得十二年前曾遇到一位和我一樣很喜歡到處旅行的詩人...到現在仍然很懷念他的一首詩呢。");
		
		return;
	}

	if( strsrch(msg, "詩") != -1 )
	{
		do_command("say 他的詩是這樣寫的..."+@TEXT

────────────────────────────────
 每次看到古厝翻新，街道變得美麗，地鐵流暢的把我們帶往城市的
 不同區域，心中，就會忍不住孵一個文藝復興的夢：
 清晨，推窗便能享受繽紛的清新•••
 路人步調悠閒•••經貿鬧區雕刻林立•••老街令人驚喜懷古•••
 整體市容優雅潔靜，是地球村的縮影，讓我們與世界同步脈動•••
 最重要的，無論走到哪裡，都能呼吸到濃郁的人文氣息：
 一半的市民懷有達文西的夢想、一半擁有哥倫布的豪情！
────────────────────────────────
	
TEXT
	);
		do_command("say 多麼令人深刻的一首詩阿！這首詩的名字是...咦，糟糕我竟然忘了這首詩的名字了...");
		
		return;
	}

	if( strsrch(msg, "名字") != -1 )
	{
		do_command("sigh");
		do_command("say 這麼好的一首詩我竟然忘了它的名字...");
		
		if( query("total_online_time", me) < 24*60*60*30 )
			return tell(me, pnoun(2, me)+"的上線時間不足 30 天無法進行此項任務。\n");

		if( me->query_quest_step(QUEST_CITY_WALKER) == 0 )
		{
			if( me->query_quest_finish(QUEST_CITY_WALKER) && time() < me->query_quest_time(QUEST_CITY_WALKER) + 60*60*24)
				return tell(me, pnoun(2, me)+"在最近的一天之內才跟這個人交談過。\n");
			
			me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "城市遊人似乎對於他忘記的詩名相當在意，或許這世界上有人知道。");
			me->set_quest_step(QUEST_CITY_WALKER, 1);
		}
		
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 3 && strsrch(msg, "孵一個文藝復興的夢") != -1 )
	{			
		do_command("ah");
		do_command("say 對！對！就是這個名字，啊...小楊，你現在一定正在哪個城市裡寫詩吧...");
		do_command("say 我想"+pnoun(2, me)+"也是從大城市來的吧！我這裡有個東西對"+pnoun(2, me)+"來說應該很實用，就給"+pnoun(2, me)+"吧。");
		
		ob = new("/quest/city_walker/land_essence");

		msg("$YOU將"+ob->query_idname()+"交給$ME。\n", me, this_object(), 1);
		
		ob->set_keep();
		ob->move(me);

		me->set_quest_finish(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "幫城市遊人找到了詩名，得到了一些獎勵品。");
		CHANNEL_D->channel_broadcast("city", me->query_idname()+" 解開了「"+QUEST_CITY_WALKER_NAME+"」", me);
		return;
	}
}

void reply_emote(object me, string verb, string args)
{

}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

void create()
{
	set_idname("city walker",NOR CYN"城市遊人"NOR);

	set("unit", "位");
	set("age", 32);
	set("gender", MALE_GENDER);
	set("long", "一個四處旅行的城市遊人。");
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 500);
	set("attr/phy", 500);
	set("attr/int", 500);
	set("attr/agi", 500);
	set("attr/cha", 500);

	set("abi/stamina/max", 30000);
	set("abi/stamina/cur", 30000);
	set("abi/health/max", 10000);
	set("abi/health/cur", 10000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("no_fight", 1);
	// 啟動主動行為
	set_behavior();

	startup_living();
}
