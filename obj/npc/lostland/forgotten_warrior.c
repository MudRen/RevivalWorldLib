/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : forgotten_warrior.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-08-18
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
#include <condition.h>

inherit STANDARD_PERSON;

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("bite "+me->query_id(1));
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
	destruct(all_inventory(this_object()));

	TREASURE_D->get_treasure(this_object());
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{

}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	if( enemy->is_module_npc() )
	{
		object boss = find_player(query("boss", enemy) || "");
		
		if( !objectp(boss) )
			return 0;
			
		return valid_enemy(boss);
	}

	if( enemy->in_condition(LOSTSEAL) ) 
		return 0;

	return 1;	
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
/*
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
*/
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// 特殊攻擊
varargs void special_attack(object attacker, object defender)
{
	if( !random(8) )
	{
		int damage;
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			damage = range_random(50,100);

			msg("$ME瘋狂地大範圍旋轉攻擊，對$YOU造成嚴重的傷害！！！\n"NOR, attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
	}
}

void create()
{
	set_idname("forgotten warrior", "被遺忘的戰士");

	set("unit", "位");
	set("age", 15);
	set("gender", MALE_GENDER);
	
	set("attr/str", 60);
	set("attr/phy", 60);
	set("attr/int", 60);
	set("attr/agi", 60);
	set("attr/cha", 60);

	set("abi/stamina/max", 1500);
	set("abi/stamina/cur", 1500);
	set("abi/health/max", 1500);
	set("abi/health/cur", 1500);
	set("abi/energy/max", 1500);
	set("abi/energy/cur", 1500);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);
	set("skills/fatalblow/level", 3);

	set("long",@TEXT
　　被遺忘的戰士，靈魂被禁錮在這死寂的大地裡，只剩下血腥的戰鬥意志。
TEXT);

	set("autokill", 1);

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
