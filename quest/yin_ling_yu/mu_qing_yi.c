/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mu_qing_yi.c
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
			(: do_command("sing") :)   : 50,
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
		return;
		
	if( strsrch(msg, "柯文") != -1 )
	{
		do_command("say 我不知道他是誰，但我可以從這個名字中感受到一股人間的大愛");
		return;
	}
	
	if( strsrch(msg, "凡環") != -1 )
	{
		do_command("say 我不知道他是誰，但我知道他已經遭遇不測，人生就是如此，珍惜您所擁有的一切吧...");
		return;
	}
	
	if( strsrch(msg, "玲羽") != -1 )
	{
		do_command("say 唉...玲羽阿，她的純樸很討人喜歡，兩年前我曾遇見過她，僅管當時我已預知今日的結果，但我仍不忍心告訴她這個必定發生的悲劇");
		return;
	}
	
	if( strsrch(msg, "人參") != -1 )
	{
		do_command("say 玲羽所挖掘出的那株野山人參的特色並不在於它的藥性，而是在於它蘊育了數千年所產生的靈性");
		return;
	}
	
	if( strsrch(msg, "靈性") != -1 )
	{
		do_command("smile "+me->query_id(1));
		do_command("say "+pnoun(2, me)+"會遇上茵玲羽，便是有緣人，上天自會給"+pnoun(2, me)+"答案");
		
		if( me->query_quest_step(QUEST_YIN_LING_YU_2) == 1 )
		{
			me->set_quest_note(QUEST_YIN_LING_YU_2, QUEST_YIN_LING_YU_2_NAME, "找到了仙女『穆青依』，知道了原來這株野山人參具有著神秘的靈性。");
			me->set_quest_step(QUEST_YIN_LING_YU_2, 2);
		}
		return;
	}
	
	if( strsrch(msg, "答案") != -1 )
	{
		do_command("say 解鈴尚需繫鈴人阿.....");
		return;
	}
		
	if( strsrch(msg, "化石") != -1 )
	{
		do_command("say "+pnoun(2, me)+"是說從自然界生物上取得那些化石嗎？聽說最近克諾索斯地區來了一個考古學家專門在收集這些化石...");	
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 1 && strsrch(msg, "城市遊人") != -1 )
	{
		do_command("say 嗯？"+pnoun(2, me)+"想知道一些關於他的事情嗎？");
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "或許該讀那段詩給穆青依聽聽。");
		me->set_quest_step(QUEST_CITY_WALKER, 2);
		return;
	}
	
	if( me->query_quest_step(QUEST_CITY_WALKER) == 2 && strsrch(msg, "一半擁有哥倫布的豪情") != -1 )
	{
		msg("$ME注意到$YOU輕靈的身形突然顫動了一下，但隨即又回復原本的樣子...。\n", me, this_object(), 1);
		do_command("say ...原來如此");
		do_command("say 這首詩叫作「孵一個文藝復興的夢」，"+pnoun(2, me)+"記著吧。");
		
		me->set_quest_note(QUEST_CITY_WALKER, QUEST_CITY_WALKER_NAME, "原來這首詩叫作「孵一個文藝復興的夢」。");
		me->set_quest_step(QUEST_CITY_WALKER, 3);
		return;
	}
	
	
	if( strsrch(msg, "遠古武器碎片") != -1 )
	{
		do_command("say 喔？"+pnoun(2, me)+"看過那些傳說中的碎片嗎？");
		return;
	}
	
	if( strsrch(msg, "看過") != -1 || strsrch(msg, "傳說中的碎片") != -1 )
	{
		int current_quest_step = me->query_quest_step(QUEST_LOST_SHARD);

		process_function(
		({
			(: do_command("ponder") :),
			3,
			(: do_command("say 數年前，在一份不知何處出土的古文中...") :),
			3,
			(: do_command("say 記載著數千年前曾有一位武士手持一把遠古神刀，橫跨數個大陸並斬殺無數害人的怪物...") :),
			3,
			(: do_command("say 但那位武士卻在一次出海之後消失了蹤跡，從此之後再也沒人見過他...") :),
			3,
			(: do_command("say 之後世界上陸續傳出有人拾獲遠古神刀的碎片，但沒人知道這些碎片有什麼用...") :),
			3,
			(: do_command("say 如果我可以拿到這些碎片，或許我可以從中得到一些當初那位武士之所以失蹤的線索...") :),
			3,
			(: do_command("say 當然，這或許只不過是一個老掉牙的傳說而已...") :),
		}));
		
		if( current_quest_step == 0 || current_quest_step == 4 )
		{
			me->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "原來世界上散落著許多類似的碎片，收集齊全的話或許有些用處。");
			me->set_quest_step(QUEST_LOST_SHARD, 1);
		}
		return;
	}
	
	if( strsrch(msg, "碎片") != -1 )
	{
		do_command("say 嗯？什麼碎片？");
		return;
	}
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
	
	do_command("bow "+me->query_id(1));
}

void got_all_lost_shard(object me, object ob)
{
	
	
}

void reply_get_object(object me, object ob, mixed amount)
{
	string *lost_shards = query("quest_temp/"+QUEST_LOST_SHARD+"/shard", me) || allocate(0);
	string basename;

	if( !objectp(ob) ) return;

	basename = base_name(ob);

	if( me->query_quest_step(QUEST_LOST_SHARD) == 1 )
	{
		switch(basename)
		{
			case "/quest/lost_shard/lost_shard_1":
			case "/quest/lost_shard/lost_shard_2":
			case "/quest/lost_shard/lost_shard_3":
			case "/quest/lost_shard/lost_shard_4":
			case "/quest/lost_shard/lost_shard_5":
			case "/quest/lost_shard/lost_shard_6":
			
				lost_shards |= ({ basename });
				
				if( sizeof(lost_shards) == 6 )
				{
					process_function(({
						1,
						(: do_command("say 非常好...") :),
						3,
						(: do_command("say 在"+pnoun(2, $(me))+"交給我這最後一片碎片時的同時，我也參透這些碎片中所隱藏的最大秘密。") :),
						3,
						(: do_command("say "+pnoun(2, $(me))+"請稍等一會兒，我準備一封信給"+pnoun(2, $(me))+"...") :),
						1,
						(: msg("$ME拿起紙筆開始撰寫一封書信...。\n", this_object(), 0, 1) :),
						8,
						(: msg("$ME放下手中的筆，將所有的碎片與信一齊放入信封中..。\n", this_object(), 0, 1) :),
						2,
						(: do_command("say 把這封信交給一位遠在另一個大陸上的老道士，他會告訴"+pnoun(2, $(me))+"接下來該怎麼做...") :),
						1,
						(: msg("$ME將書信交給了$YOU。\n", this_object(), $(me), 1) :),
						(: new("/quest/lost_shard/letter")->move($(me)) :),
						(: delete("quest_temp/"+QUEST_LOST_SHARD+"/shard", $(me)) :),
						(: $(me)->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "將穆青依撰寫的信交給一位遠在另一個大陸上的老道士。") :),
						(: $(me)->set_quest_step(QUEST_LOST_SHARD, 2) :),
					}));
				}
				else
				{
					me->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, pnoun(2, me)+"已經收集到了 "+sizeof(lost_shards)+" 片碎片，收集齊全的話或許有些用處。");
	
					process_function(({
						1,
						(: do_command("nod 想不到"+pnoun(2, $(me))+"可以找到這些碎片...") :),
						2,
						(: do_command("say 這片碎片非常脆弱，隨時都可能損壞，我會幫"+pnoun(2, $(me))+"妥善保存好，等"+pnoun(2, $(me))+"收集完所有碎片時，我想我便可以參詳出裡面的秘密。") :),
					}));
				
					set("quest_temp/"+QUEST_LOST_SHARD+"/shard", lost_shards, me);
					
				}
				me->save();
				destruct(ob);
				break;
			default:
			{
				process_function(({
					1,
					(: do_command("say 解鈴尚需繫鈴人...") :),
					1,
					(: do_command("drop all "+$(ob)->query_id(1)) :),
				}));
				break;
			}
		}
		
		return;
	}
	
	process_function(({
		1,
		(: do_command("say 解鈴尚需繫鈴人...") :),
		1,
		(: do_command("drop all "+$(ob)->query_id(1)) :),
	}));
}


void create()
{
	set_idname("mu qing yi", HIG"穆"NOR GRN"青依"NOR);
	set_temp("status", ([HIG"世外"NOR GRN"仙女"NOR:-1]));

	set("long", "好清秀的年輕女子，她的氣質風采使人不敢直視著她，站在她的身旁不由得自慚形穢。");
	set("unit", "位");
	set("age", 24);
	set("gender", FEMALE_GENDER);
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 1500);
	set("attr/phy", 1200);
	set("attr/int", 100);
	set("attr/agi", 700);
	set("attr/cha", 200);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 11000);
	set("abi/health/cur", 11000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();

	startup_living();
}
