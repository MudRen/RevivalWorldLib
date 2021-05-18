/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : little_rabbit.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-17
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
	500 	:	HIW"白兔"HIY"獵"NOR YEL"人"NOR,
	5000	:	HIW"白兔"HIB"剋"NOR BLU"星"NOR,
	50000	:	HIW"白兔"HIR"殺"NOR RED"手"NOR,
	200000	:	HIW"白兔"HIM"噬"NOR MAG"者"NOR,
	500000	:	HIW"白兔"WHT"死"HIW"神"NOR,
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

	this_object()->do_command("inn "+me->query_id(1));
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

	if( time() < 1261584000 && !random(80) )
	{
		object ob;
		
		if( !random(2) )
			ob = new("/obj/festival/2009/midwinter/sesame_glue_pudding");
		else
			ob = new("/obj/festival/2009/midwinter/peanut_glue_pudding");
			
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
		object ob = new("/quest/monster_fossil/rabbit_fossil");
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
			return "$ME很蹦蹦跳跳地往$YOU一口咬來，";
			break;
		case 1:
			return "$ME的鼻子動了一動，往$YOU的方向蹤躍咬來，";
			break;
		case 2:
			return "$ME突然往上躍，往$YOU的頭部攻擊，";
			break;
		case 3:
			return "$ME從後方輕輕咬了$YOU一口，";
			break;
		case 4:
			return "$ME淚眼汪汪的咬向$YOU的手，";
			break;
	}
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

void create()
{
	set_idname("little rabbit", "小白兔");

	set("unit", "隻");
	set("age", 2);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 5);
	set("attr/phy", 5);
	set("attr/int", 5);
	set("attr/agi", 5);
	set("attr/cha", 5);

	set("abi/stamina/max", 50);
	set("abi/stamina/cur", 50);
	set("abi/health/max", 50);
	set("abi/health/cur", 50);
	set("abi/energy/max", 50);
	set("abi/energy/cur", 50);

	set("skills/unarmed/level", 5);
	set("skills/dodge/level", 5);

	if( !random(1000) )
	{
		set_idname(HIY"little"NOR YEL" rabbit"NOR, HIY"小白"NOR YEL"兔"NOR);
		
		MAPNPC_D->become_named_npc(this_object());
	}

	set("long", "　一隻楚楚可憐的小白兔，安詳地吃著小草。");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
