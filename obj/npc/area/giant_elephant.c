/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : giant_elephant.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-26
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
	500 	:	"巨象"HIY"獵"NOR YEL"人"NOR,
	5000	:	"巨象"HIB"剋"NOR BLU"星"NOR,
	50000	:	"巨象"HIR"殺"NOR RED"手"NOR,
	200000	:	"巨象"HIM"噬"NOR MAG"者"NOR,
	500000	:	"巨象"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	if( !objectp(me) ) return;

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
			case 500..4999: probability = 3000; break;
			case 5000..49999:  probability = 2800; break;
			case 50000..199999: probability = 2600; break;
			case 200000..499999: probability = 2400; break;
			case 500000..MAX_INT: probability = 2200; break;
			default: probability = 3200; break;
		}
		
		break;
	}

	if( !random(probability) )
	{
		object ob = new("/quest/monster_fossil/elephant_fossil");
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
			return "$ME的鼻子橫向往$YOU掃來，";
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
			return "$ME利用象牙攻擊$YOU，";
			break;
	}
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// 特殊攻擊
varargs void special_attack(object attacker, object defender)
{
	if( !random(20) )
	{
		msg("$ME全力往$YOU正面衝刺，龐大的象牙夾帶剛猛的力道「"HIY"破石"NOR YEL"穿岩"NOR"」將$YOU狠狠刺傷！！！\n", attacker, defender, 1);
		
		COMBAT_D->cause_damage(attacker, defender, range_random(200, 350));
	}
}

void create()
{
	set_idname("giant elephant", "巨象");

	set("unit", "頭");
	set("age", 40);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 100);
	set("attr/phy", 100);
	set("attr/int", 100);
	set("attr/agi", 100);
	set("attr/cha", 100);

	set("abi/stamina/max", 1000);
	set("abi/stamina/cur", 1000);
	set("abi/health/max", 1000);
	set("abi/health/cur", 1000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);

	set("skills/unarmed/level", 100);
	set("skills/dodge/level", 100);

	set("long",@TEXT
　　眼前的這頭大象明顯的較普通的還要巨大，感覺很不尋常。巨象以兇惡銳利
的眼神狠狠地環視四週，似乎正在找大開殺戒的對象。
TEXT);

	set("autokill", 1);
	if( !random(1000) )
	{
		set_idname(HIY"giant"NOR YEL" elephant"NOR, HIY"巨"NOR YEL"象"NOR);
		
		MAPNPC_D->become_named_npc(this_object());
	}
	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
