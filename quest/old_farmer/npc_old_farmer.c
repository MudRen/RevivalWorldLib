/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc_old_farmer.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
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
void make_money();

void set_behavior()
{
	//set("behavior/walking", 120);
	
	set("behavior/random", 
	    ([
		(: make_money :) : 50,
		(: do_command("inn 肚子好餓阿...") :)   : 50,
		(: do_command("cough") :)               : 50,
		(: do_command("sigh 最近景氣差，工作都沒了...") :) : 50,
		(: this_object()->drop_money("RW", 1) && msg("$ME的口袋裡似乎掉出了一些東西，但他並沒有注意到...\n", this_object(), 0, 1) :) : 50,
	    ]));
}

void make_money()
{
	int i, m = random(5) + 1;
	object ob, *obs;
	
	obs = obs = present_objects(this_object());
	obs = filter(obs, (: $1->is_user_ob() :));
	if( !(i = sizeof(obs)) ) return;
	ob = obs[random(i)];
	process_function(({
		(: msg("$ME搖尾乞憐的跪下向$YOU討錢。\n", this_object(), $(ob), 1) :),
		4,
		(: (objectp($(ob)) && environment($(ob)) == environment(this_object())) ? 1 : -1 :),
		(:
		    $(ob)->spend_money("RW", $(m)) &&
		    msg("$YOU一時心軟從口袋掏出了 "HIY"$RW " + $(m) + NOR" 給$ME。\n", this_object(), $(ob), 1) ||
		    this_object()->earn_money("RW", $(m)) &&
		    do_command("kneel " + $(ob)->query_id(1))
		:),
	    }));
}

void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( same_environment(this_object(), me) && msg && strsrch(msg, "飯") != -1 )
		do_command("nod "+me->query_id(1)+" 對！對！我好想吃飯阿...");
}

void reply_emote(object me, string verb, string args)
{
	if( same_environment(this_object(), me) )
		do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;
	
	do_command("say 疑？！人跑哪去了呢，原來是個不求回報的大好人...真好真好...");
	
	if( objectp(me) )
		delete_temp("quest/"QUEST_OLD_FARMER"/running", me);
	
	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;
	
	if( base_name(ob) == "/obj/farming/product/rice" )
	{
		if( time() - me->query_quest_time(QUEST_OLD_FARMER) < 60*60*24 )
		{
			destruct(ob);
			return do_command("say 啊！"+pnoun(2, me)+"不是上次那個大好人嗎？真的很謝謝"+pnoun(2, me)+"又給我飯吃，真是謝謝了");
		}

		if( query("total_online_time", me) < 24*60*60 )
		{
			destruct(ob);
			return tell(me, pnoun(2, me)+"的上線時間不足 1 天無法進行此項任務。\n");
		}
		
		if( query_temp("quest/"QUEST_OLD_FARMER"/running", me) )
		{
			destruct(ob);
			return;
		}
		
		set_temp("quest/"QUEST_OLD_FARMER"/running", 1, me);
		
		delete_behavior();
		
		process_function(({
			2,
			(: target_gone($(me)) :),
			(: do_command("wa") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("thank "+$(me)->query_id(1)+" "+pnoun(2, $(me))+"真是個大好人吶") :),
			
			4,
			(: target_gone($(me)) :),
			(: do_command("say "+pnoun(2, $(me))+"先等等，讓我好好想想要怎麼報答"+pnoun(2, $(me))) :),
			(: do_command("ponder") :),
			5,
			(: target_gone($(me)) :),
			(: do_command("ah") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say 雖然這不是什麼大不了的東西，但畢竟是我五十多年來的專長...") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say 看"+pnoun(2, $(me))+"資質不差，讓我將畢生的農業經驗教授於你吧！") :),
			2,
			(: target_gone($(me)) :),
			(: msg("$ME一口氣講述了五十多年來的農作經驗，$YOU獲得了大量社會經驗值。\n", this_object(), $(me), 1 ) :),
			(: $(me)->add_social_exp(300000+random(300000)) :),
			(: delete_temp("quest/"QUEST_OLD_FARMER"/running", $(me)) :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+" 解開了「"+QUEST_OLD_FARMER_NAME+"」", $(me)) :),
			(: $(me)->set_quest_finish(QUEST_OLD_FARMER, QUEST_OLD_FARMER_NAME, "遇見了『饑餓的老農夫』，給了他飯吃，得到了『社會經驗』。") :),
			// 恢復主動行為
			(: set_behavior() :),
		    }));
		
		destruct(ob);
	}
	
	else
	{
		process_function(({
			1,
			do_command("say 這是什麼？我不要這個東西..."),
			1,
			do_command("drop all "+ob->query_id(1)),
		    }));
	}
}

void reply_get_money(object me, string unit, mixed money)
{
	return do_command("say 謝謝"+pnoun(2, me)+"！"+pnoun(2, me)+"真是個大好人，不知道"+pnoun(2, me)+"可不可以給我一口飯吃呢？");
}

void create()
{
	set_idname("old farmer","饑餓的老農夫");
	
	set("unit", "位");
	
	set("age", 78);
	set("gender", MALE_GENDER);
	set("title", HIG"農神"NOR);
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
