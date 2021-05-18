/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wild_tiger.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-19
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

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	500 	:	"野虎"HIY"獵"NOR YEL"人"NOR,
	5000	:	"野虎"HIB"剋"NOR BLU"星"NOR,
	50000	:	"野虎"HIR"殺"NOR RED"手"NOR,
	200000	:	"野虎"HIM"噬"NOR MAG"者"NOR,
	500000	:	"野虎"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("cat");
}

int target_gone(object me)
{

}

/*
void reply_get_object(object me, object ob, mixed amount)
{

}
*/

// 死亡時的事件
void reply_die()
{
	TREASURE_D->get_treasure(this_object());
	
	if( !random(100000/COMBAT_D->total_value(this_object())) )
	{
		object ob;
		string* random_drop =
		({
			"/obj/weapon/coarse_fist",
			"/obj/weapon/coarse_blade",
			"/obj/weapon/coarse_stick",
			"/obj/armor/coarse_breast_plate",
			"/obj/armor/coarse_leggings",
			"/obj/armor/coarse_helm",
			"/obj/armor/coarse_gloves",
			"/obj/armor/coarse_cloak",			
			"/obj/armor/coarse_bracers",
			"/obj/armor/coarse_boots",
			"/obj/armor/coarse_belt",
		});
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	}
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	int probability = 3200;
	
	foreach(object attacker in attackers)
	{
		if( !objectp(attacker) ) continue;
			
		switch(query("combat/defeat/"+replace_string(base_name(this_object()), "/", "#"), attacker))
		{
			case 500: probability = 3000; break;
			case 5000:  probability = 2800; break;
			case 50000: probability = 2600; break;
			case 200000: probability = 2400; break;
			case 500000: probability = 2200; break;
			default: probability = 3200; break;
		}
		
		break;
	}
	
	if( !random(probability) )
	{
		object ob = new("/quest/monster_fossil/tiger_fossil");
		ob->move(this_object());
	}
	
	::reply_defeat(attackers);
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
			return "$ME兇惡的往$YOU一口咬來，";
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
			return "$ME死命咬住$YOU的手，";
			break;
	}
}

// 防禦時的訊息
string combat_defend_message(object attacker, object weapon)
{
	
	
}

void create()
{
	set_idname("wild tiger", "野虎");

	set("unit", "頭");
	set("age", 4);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 40);
	set("attr/phy", 40);
	set("attr/int", 40);
	set("attr/agi", 40);
	set("attr/cha", 40);

	set("abi/stamina/max", 400);
	set("abi/stamina/cur", 400);
	set("abi/health/max", 400);
	set("abi/health/cur", 400);
	set("abi/energy/max", 400);
	set("abi/energy/cur", 400);

	set("skills/unarmed/level", 40);
	set("skills/dodge/level", 40);
	
	set("long", "　一隻渾身髒亂又兇惡的野虎。");

	if( !random(1000) )
	{
		set_idname(HIY"wild"NOR YEL" tiger"NOR, HIY"野"NOR YEL"虎"NOR);
		
		MAPNPC_D->become_named_npc(this_object());
	}
	
	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
