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
		(: msg("$ME拿著特殊的器具不斷地在水晶上敲敲打打。\n", this_object(), 0, 1) :)   : 500,
		(: msg("$ME拿著特殊的磨石器將水晶一顆顆地磨成元素。\n", this_object(), 0, 1) :)   : 500,
		(: do_command("sweat") :)               : 100,
		(: do_command("wahaha 這個水晶實在是太完美了！") :) : 100,
	    ]));
}

void reply_say(object me, string msg)
{
	string title;
	string name = me->query_name(1);

	if( is_english(name) )
		title = " "+capitalize(name[0..0])+" ";
	else
		title = name[0..1];

	if( query("gender", me) == MALE_GENDER )
		do_command("say 這位"+title+"先生，要不要考慮買一些漂亮的水晶回家呀？");
	else
		do_command("say 這位"+title+"小姐，要不要考慮買一些漂亮的水晶回家呀？");
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
		do_command("say 這是什麼？我不要這個東西..."),
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
	set_idname("crystal miner","水晶礦工");
	
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



