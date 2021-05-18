/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mountain_taoist.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
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
			//(: do_command("sweat") :)   : 50,
	    ]));
}


void delete_behavior()
{
	delete("behavior");
}

void reply_say(object me, string msg)
{
	if( !objectp(me) ) return;

	//msg("$ME默默地在一塊大石頭上靜坐，不發一語，任隨微風吹亂他幾乎淨白的鬍子...。\n", this_object(), 0, 1);
}

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;
}

void reply_get_object(object me, object ob, mixed amount)
{
	if( !objectp(ob) ) return;

	if( me->query_quest_step(QUEST_LOST_SHARD) == 2 && base_name(ob) == "/quest/lost_shard/letter" )
	{
		process_function(({
			1,
			(: msg("$ME突然放聲大笑：「哈、哈、哈」...。\n", this_object(), 0, 1) :),
			3,
			(: do_command("say 穆青依這女孩真是既聰明卻又胡鬧，居然將這難題丟給我這個早已不問世事的老頭兒...。") :),
			3,
			(: do_command("say 但我想這件事，當今世上也只有我這個不中用的老頭還知道一些其中的秘密...。") :),
			3,
			(: do_command("say "+pnoun(2, $(me))+"所收集的這些碎片，其實是一把通往遺落的大地的鑰匙...。") :),
			3,
			(: do_command("say 但這些碎片相當脆弱，"+pnoun(2, $(me))+"沒有辦法直接利用這些碎片去打開通往那塊大陸的通道") :),
			3,
			(: do_command("say 我可以幫"+pnoun(2, $(me))+"將這些碎片進行強化，請給我一塊玻璃，我便可以將這些碎片嵌在玻璃中") :),
			(: $(me)->set_quest_note(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "給崎山道士一塊玻璃，讓他強化那些碎片。") :),
			(: $(me)->set_quest_step(QUEST_LOST_SHARD, 3) :),
		}));
		
		destruct(ob);
		return;
	}
	else if( me->query_quest_step(QUEST_LOST_SHARD) == 3 && ob->query_module() == "glass" )
	{
		process_function(({
			1,
			(: do_command("say 很好，我這就來幫"+pnoun(2, $(me))+"強化一下這些碎片吧...。") :),
			1,
			(: msg("$ME緊握著玻璃與所有的遠古武器碎片，只見手中發出一陣陣的"HIC"七"HIG"彩"HIR"強"HIY"光"NOR"...\n", this_object(), 0, 1) :),
			8,
			(: msg("$ME將手輕輕放開...\n", this_object(), 0, 1) :),
			2,
			(: do_command("say 好了，這鑰匙是"+pnoun(2, $(me))+"的了，至於那塊遺落的大地的入口在哪，便要靠"+pnoun(2, $(me))+"自己去尋找了...。") :),
			(: msg("$ME將鑰匙交給了$YOU。\n", this_object(), $(me), 1) :),
			(: new("/quest/lost_shard/lostland_key")->move($(me)) :),
			(: $(me)->set_quest_finish(QUEST_LOST_SHARD, QUEST_LOST_SHARD_NAME, "從崎山道士身上取得了一把通往遺失的大陸鑰匙。") :),
			(: $(me)->set_quest_step(QUEST_LOST_SHARD, 4) :),
		}));
		
		if( !wizardp(me) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"解開了「"+QUEST_LOST_SHARD_NAME+"」。", me);

		destruct(ob);
		return;
	}

	process_function(({
		1,
		(: do_command("say 謝謝"+pnoun(2, $(me))+"的好意，我不並需要這個東西...") :),
		1,
		(: do_command("drop all "+$(ob)->query_id(1)) :),
	}));
}


void create()
{
	set_idname("mountain taoist", HIY"崎山"NOR YEL"道士"NOR);
	set_temp("status", ([HIY"靜"NOR YEL"坐"NOR:-1]));

	set("long", "  一位非常老的老道士，看起來修道已久，一幅十分慈祥的樣子。");
	set("unit", "位");
	set("age", 112);
	set("gender", MALE_GENDER);
	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 500);
	set("attr/phy", 500);
	set("attr/int", 500);
	set("attr/agi", 500);
	set("attr/cha", 500);

	set("abi/stamina/max", 50000);
	set("abi/stamina/cur", 50000);
	set("abi/health/max", 50000);
	set("abi/health/cur", 50000);
	set("abi/energy/max", 50000);
	set("abi/energy/cur", 50000);

	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();

	startup_living();
}
