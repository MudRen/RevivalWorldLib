/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : john_barton.c
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
		(: do_command("say 唉，我記得這附近應該有的啊，怎麼會找不到呢...") :)   : 50,
		(: do_command("sigh 找不到的話她一定會很失望的...") :) : 50,
	    ]));
}
void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) || !msg ) return;

	if( strsrch(msg, "找") != -1 || strsrch(msg, "忙") != -1)
		do_command("say 我想找花送給我可愛的女朋友。");
		
	if( strsrch(msg, "花") != -1 )
	{
		if( me->query_quest_finish(QUEST_BOHR) )
		{
			do_command("say 哦？"+pnoun(2, me)+"不是上次那位送我花的人嗎？真的很謝謝"+pnoun(2, me)+"啊。");
			return;
		}

		do_command("say 找怎樣的花啊，我也不知道，我想大概是比較漂亮一點的花吧。");
		
		if( me->query_quest_step(QUEST_BOHR) == 0 )
		{
			me->set_quest_note(QUEST_BOHR, QUEST_BOHR_NAME, "遇見了強尼•巴頓，他似乎在找一種花。");
			me->set_quest_step(QUEST_BOHR, 1);
		}
	}
}

void reply_emote(object me, string verb, string args)
{
	if( objectp(me) ) {
		switch(verb) {
			case "hi":
				do_command(verb + " " +me->query_id(1)+ " " + "你好，請問有什麼事嗎？");
				break;
			case "?":
				do_command("say 你幹嘛用那種眼神看我，我只是來找東西的，可不是賊啊。");
				break;
			default:
				do_command("sorry " +me->query_id(1)+ " 抱歉我正在忙，請別打擾我");
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
	string obname;

	if( !objectp(ob) ) return;

	obname = ob->query_name();

	if( me->query_quest_step(QUEST_BOHR) == 1 && query("behavior") && (base_name(ob) == "/obj/farming/product/lily" || base_name(ob) == "/obj/farming/product/rose"))
	{
		destruct(ob);
	
		if( query_temp("quest/"QUEST_BOHR"/running", me) )			
			return;

		set_temp("quest/"QUEST_BOHR"/running", 1, me);

		// 停止主動行為
		delete_behavior();

		process_function(({
			2,
			(: target_gone($(me)) :),
			(: do_command("tsk 你真的要送我"+$(obname)+"啊？這樣不太好意思吧") :),
			4,
			(: target_gone($(me)) :),
			(: do_command("say 但這個送給我女朋友她一定會很高興的，好吧，謝謝"+pnoun(2, $(me))+"了。") :),
			(: do_command("thank "+$(me)->query_id(1) ) :),
			5,
			(: target_gone($(me)) :),
			(: do_command("ah") :),
			2,
			(: target_gone($(me)) :),
			(: do_command("say 這個是我剛在路上撿到的，我不懂釣魚拿了也沒用，就送給你吧∼") :),
			5,
			(: target_gone($(me)) :),
			(: new(__DIR__"broken_rod")->move($(me)) :),
			(: msg("$ME得到了一支舊舊的竹製釣竿。\n", $(me), 0, 1) :),
			(: do_command("say 那麼我走了，真是謝謝你") :),
			(: delete_temp("quest/"QUEST_BOHR"/running", $(me)) :),
			(: $(me)->set_quest_note(QUEST_BOHR, QUEST_BOHR_NAME, pnoun(2, $(me))+"幫強尼•巴頓找到了花，他給你了一支看起來舊舊的竹製釣竿。") :),
			(: $(me)->set_quest_step(QUEST_BOHR, 2) :),
			// 恢復主動行為
			(: set_behavior() :),
		    }));
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
	set_idname("john barton",HIW"強尼"NOR WHT"•巴頓"NOR);

	set("unit", "位");
	set("age", 19);
	set("gender", MALE_GENDER);
	set("long", "一位來自國外的俊俏青年，看他的樣子似乎正在苦惱著。");
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
