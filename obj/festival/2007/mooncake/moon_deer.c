/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moon_rabbit.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-22
 * Note   : 月兔
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
#include <npc.h>
#include <condition.h>
#include <feature.h>

inherit STANDARD_PERSON;

string *players = allocate(0);
mapping emote_players = allocate_mapping(0);
string *keywords = ({ "鹿", "滑", "茸" });

string *awards =
({
	"/obj/festival/2007/mooncake/moon_cake",
	"/obj/festival/2007/mooncake/armor/moonrabbit_belt",
	"/obj/festival/2007/mooncake/armor/moonrabbit_boots",
	"/obj/festival/2007/mooncake/armor/moonrabbit_bracers",
	"/obj/festival/2007/mooncake/armor/moonrabbit_cloak",
	"/obj/festival/2007/mooncake/armor/moonrabbit_cloth",
	"/obj/festival/2007/mooncake/armor/moonrabbit_gloves",
	"/obj/festival/2007/mooncake/armor/moonrabbit_helm",
});

void do_command(string cmd);

int is_keyword(string msg)
{
	for(int i=0; i<sizeof(keywords); i++)
		if( msg && strsrch(msg, keywords[i]) != -1 ) return 1;
	return 0;
}
void reply_action(object me)
{
	object ob = this_object();
	string id = me->query_id(1);
	string city, city1;

	city = query("city", me);
	city1 = query_temp("first", ob);

	if( undefinedp(emote_players[id]) )
		emote_players[id] = 1;
	else
		emote_players[id]++;
	switch(emote_players[id])
	{
	case 1:
		this_object()->do_command("point "+me->query_id(1)+" ㄋ是不是要神幫ㄋ rm");
		break;
	case 2:
		this_object()->do_command("say 你是ㄅ是想進ㄅ來");
		break;
	case 3:
		this_object()->do_command("draw "+id+" 你真ㄉ會進不來ㄋ");
		break;
	case 4:
		this_object()->do_command("grin "+id);
		break;
	default:
		this_object()->do_command("gan "+id);
		//this_object()->do_command("kill "+id);
		//if( time() > 1190736000 ) { this_object()->do_command("say 都結束了還來"); break; }
		if( ! city1 ) {
			set_temp("first", city);
			msg(HIR"$ME"HIR"對著$YOU"HIR"大喊：「去死吧！！」。\n"NOR, this_object(), me, 1);
			COMBAT_D->start_fight(this_object(), me, 1);
		}
		else if( city1 == city) {
			msg(HIR"$ME"HIR"對著$YOU"HIR"大喊：「去死吧！！」。\n"NOR, this_object(), me, 1);
			COMBAT_D->start_fight(this_object(), me, 1);
		}
		else {
			tell( me,HIR"已經有人先打了。\n"NOR);
		}
		break;
	}
	return;
}

void reply_say(object me, string msg)
{
	if( is_keyword(msg) )
	{
		reply_action(me);
		return;
	}
	/*
	if( msg == "" )
	{
		string id = me->query_id(1);
		if( member_array(id, players) < 0 )
		{
			object ob = new("/obj/festival/2007/mooncake/moon_cake.c");
			this_object()->do_command("good");
			msg("$YOU從$ME手中接下了"+ob->short()+"。\n", this_object(), me, 1 );
			ob->move(me);
			players |= ({ id });
		}
		else
		{
			this_object()->do_command("slap "+id);
			this_object()->do_command("point "+id+" ㄋ都拿過ㄌ還想幹什ㄇ");
		}
		return;
	}
	*/
}

void reply_emote(object me, string verb, string args)
{
	if( is_keyword(args) )
	{
		reply_action(me);
		return;
	}
	this_object()->do_command("hmm "+me->query_id(1)+" "+args);
}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

// 死亡時的事件
void reply_die()
{
	object ob;
	mixed loc, test;
	string ob_idname;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	ob = new("/obj/festival/2007/mooncake/moon_cake");
	//ob = new(awards[random(sizeof(awards))]);
	ob->move(this_object());

	ob_idname = ob->query_idname();

	//CHANNEL_D->channel_broadcast("news", "某人殺掉了吉祥物月兔並從月兔屍體撿取了"+ob_idname+"。");

	//destruct(ob); // 不開放暫時 dest 掉

	TREASURE_D->get_treasure(this_object());

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	//ob = new("/obj/festival/2007/mooncake/moon_deer");
	//ob->move(loc);
	//shout(HIB"遠方"NOR" - 復仇的月兔出現在  "+test+" "NOR"附近。\n");
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	return 1;	
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
	case 0:
		return "$ME兇惡的往$YOU衝撞過來，";
		break;
	case 1:
		return "$ME往$YOU的背後衝撞過來，";
		break;
	case 2:
		return "$ME用後腿猛踢$YOU的腿部，";
		break;
	case 3:
		return "$ME從側面快速攻擊$YOU，";
		break;
	case 4:
		return "$ME用頭上的角頂$YOU，";
		break;
	}
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{

	return "$ME用頭上的角擋住了$YOU的攻擊冒出陣陣火花，";
}

// 特殊攻擊
int special_attack(object attacker, object defender)
{
	int time;

	if( !random(50) )
	{
		shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");
	}

	if( !random(30) )
	{
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) ) continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			time = range_random(1, 5);

			msg("$ME快速的滑動，一招「"HIY"鹿角"NOR YEL"衝撞"NOR"」，使$YOU眼花撩亂("+time+" 秒)，並造成 50 點傷害！！！\n"NOR, attacker, ob, 1);

			ob->start_delay("moondeer", time, pnoun(2, ob)+"因為「"HIY"鹿角"NOR YEL"衝撞"NOR"」的影響而暈眩中。\n", pnoun(2, ob)+"停止了暈眩。\n");

			COMBAT_D->cause_damage(attacker, ob, 50);
		}
	}
	return 0;
}

int is_boss()
{
	return 1;
}

void destruct_self()
{
	/*
	object ob;
	mixed loc, test;
	string *where_name = CITY_D->query_cities();
	string where = where_name[random(sizeof(where_name))];

	loc = arrange_city_location(range_random(0,99), range_random(0,99), where, random(CITY_D->query_city_num(where)));

	test = loc_short(loc);

	ob = new("/obj/festival/2007/mooncake/moon_deer");
	ob->move(loc);
	shout(HIB"遠方"NOR" - 沒有人理的復仇月兔移動到了  "+test+" "NOR"附近。\n");
	*/
	shout(HIB"\n遠方"NOR" - 沒有人理的月兔們跑回了月球。\n\n");
	destruct(this_object());
}

void create()
{
	set_idname("moon rabbit", HIC"復仇"NOR+CYN"的"HIW"月兔"NOR);

	set("unit", "隻");
	set("age", 999);
	set("gender", MALE_GENDER);

	set("attr/str", 10);
	set("attr/phy", 10);
	set("attr/int", 10);
	set("attr/agi", 10);
	set("attr/cha", 10);

	set("abi/stamina/max", 15000);
	set("abi/stamina/cur", 15000);
	set("abi/health/max", 15000);
	set("abi/health/cur", 15000);
	set("abi/energy/max", 15000);
	set("abi/energy/cur", 15000);
	set("job/name", HIW+"★★★★★"+NOR); 
	set("job/type", SOLDIER);
	set("skills/unarmed/level", 10);
	set("skills/dodge/level", 10);

	set("long", "　中秋節才會出現的特殊動物，它其奇怪的身影讓人不寒而慄，\n有個古老的傳說，只要對它說出禁語，會遭受可怕的懲罰。");
	set("no_fight", 1);

	startup_living();

	if( clonep() )
		call_out( (: destruct_self :), 420 );
}

int remove()
{
	destruct(all_inventory(this_object()));
}

