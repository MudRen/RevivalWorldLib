/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc_seller.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-31
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

inherit STANDARD_PERSON;

void do_command(string cmd);



void set_behavior()
{
	set("behavior/random", 
	    ([
		(: msg("$ME張著大眼往這裡仔細端詳著...。\n", this_object(), 0, 1) :)   : 500,
		(: do_command("sweat") :)               : 100,
		(: do_command("smile") :) : 100,
	    ]));
}

void reply_say(object me, string msg)
{
	if( query("gender", me) == MALE_GENDER )
		do_command("say 這位先生，看"+pnoun(2, me)+"像是從大城市裡來的，相逢便是有緣，我這裡有一些配方書，需要的話可以跟我說喔。");
	else
		do_command("say 這位小姐，看"+pnoun(2, me)+"像是從大城市裡來的，相逢便是有緣，我這裡有一些配方書，需要的話可以跟我說喔。");
}

void reply_emote(object me, string verb, string args)
{
	if( same_environment(this_object(), me) )
		do_command(verb + " " + me->query_id(1) + " " + (args||""));
}

void reply_get_object(object me, object ob, mixed amount)
{
	process_function(({
		1,
		do_command("say 這是什麼？我不需要這個東西..."),
		1,
		do_command("drop all "+ob->query_id(1)),
	    }));
}

void reply_get_money(object me, string unit, mixed money)
{

}

int is_seller_npc()
{
	return 1;
}

void create()
{
	set_idname("pack traveller","布包旅者");
	
	set("unit", "位");
	
	set("age", 35);
	set("gender", MALE_GENDER);

	set("heartbeat", 1); // 永久性心跳

	set("attr/str", 1000);
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



