/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : QuestNPC.c
 * Author : 
 * Date   : 2005-01-05
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
	//set("behavior/walking", 120);

	set("behavior/random", 
	    ([
		(: do_command("sigh 最近都釣不太到魚，如果我那把釣竿還在的話一定不會這樣的") :)   : 50,
		(: do_command("glare 為什麼會那麼粗心大意啊") :) : 50,
		(: do_command("why") :) : 50,
		(: do_command("inn") :) : 50,
		(: do_command("mutter 最近真背...") :) : 50,
	    ]));
}
void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "找") != -1 && strsrch(msg, "釣竿") < 0 || strsrch(msg, "幫忙") != -1)
		do_command("say 啊？我只是念念而已啦，我想應該找不回來了啦，我的釣竿不見了");
		
	if( strsrch(msg, "釣竿") != -1 )
		do_command("say 說到這釣竿啊，那把釣竿從我開始學釣魚就一直陪我到現在，也不是什麼神奇的釣竿，不過就是順手了點罷了..");

	if( strsrch(msg, "粗心大意") != -1 )
		do_command("say 說來話長啊，前陣子我到山裡去溪釣，喝了點酒後就呼呼大睡，結果醒來我的釣竿就不見了，我實在好難過啊...");
}

void reply_emote(object me, string verb, string args)
{
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command("sigh " +me->query_id(1)+ " 你好，有什麼事嗎");
				break;
			case "?":
				do_command("eh " +me->query_id(1)+ " 看什麼啊，心情不是很好別來亂我" );
				break;
			case "hmm":
				do_command("say 怎麼了？有什麼事嗎...");
				break;
			default:
				do_command("? " +me->query_id(1)+ " 我們．．不是很熟吧？有何貴幹？");
				break;
		}
	}
}

int target_gone(object me)
{
	if( same_environment(this_object(), me) )
		return 1;

	do_command("say 疑？！人跑哪去了呢，原來是個不求回報的大好人...真好真好...");

	if( objectp(me) )
		delete_temp("quest/"QUEST_BOHR"/running", me);

	set_behavior();
	return -1;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	if( me->query_quest_step(QUEST_BOHR) == 2 && query("behavior") && base_name(ob) == __DIR__"broken_rod"  )
	{
		destruct(ob);
		
		if( query_temp("quest/"QUEST_BOHR"/running", me) )
			return;

		set_temp("quest/"QUEST_BOHR"/running", 1, me);

		// 停止主動行為
		delete_behavior();

		process_function(({
			4,
			(: target_gone($(me)) :),
			(: do_command("wa") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("roll") :),
			(: do_command("scream "+$(me)->query_id(1) ) :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say 這．．這把釣竿不就是我的嗎？"+pnoun(2, $(me))+"特地拿來還我的嗎？") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("sob "+$(me)->query_id(1)+" "+pnoun(2, $(me))+"真是個好人") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say 真是太感激了，我該怎麼報答"+pnoun(2, $(me))+"呢。") :),
			(: do_command("ponder") :),
			6,
			(: target_gone($(me)) :),
			(: do_command("say 這樣好了，我把我的釣魚心得筆記送"+pnoun(2, $(me))+"好了，雖然不是什麼大禮，但這也是跟了我十幾年的東西。") :),
			2,
			(: target_gone($(me)) :),
			(: new(__DIR__"fishing_note")->move($(me)) :),
			(: msg("$ME得到了釣魚筆記。\n", $(me), 0, 1) :),
			(: $(me)->add_title(HIW"釣魚"NOR WHT"達人"NOR) :),
			(: delete_temp("quest/"QUEST_BOHR"/running", $(me)) :),
                        (: $(me)->set_quest_finish(QUEST_BOHR, QUEST_BOHR_NAME, pnoun(2, $(me))+"幫波爾找到了他的釣竿，得到了一些獎勵。") :),
			(: !wizardp($(me)) && CHANNEL_D->channel_broadcast("city", $(me)->query_idname()+"解開了「"QUEST_BOHR_NAME"」", $(me)) :),
			4,
			(: target_gone($(me)) :),
			(: msg("只見$ME拿起竹製釣竿小心翼翼的擦拭著上面的灰塵，將破釣竿越擦越亮，到最後居然整隻都泛滿著"HIY"金光"NOR"。\n"NOR, this_object(), 0, 1) :),
			5,
			(: target_gone($(me)) :),
			(: msg("$ME高興的拿著"HIY"黃金"NOR YEL"小釣竿"NOR"非常興奮的揮舞著。\n"NOR, this_object(), 0, 1) :),
			(: do_command("mutter 這下子我又可以再繼續的釣魚了") :),
			(: do_command("thank "+$(me)->query_id(1) ) :),
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
			do_command("drop all"),
		    }));
	}
}
void create()
{
	set_idname("bohr",HIG"波"NOR GRN"爾"NOR);
	
	set("long", "一位喜愛釣魚的外國男子，但看他愁眉苦臉的樣子似乎正遇到一些困難。");
	set("unit", "位");
	set("age", 48);
	set("gender", MALE_GENDER);
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
                                                  
