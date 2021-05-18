/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : caretta_gigas.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 赤蠵龜
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

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	10 		:	HIR"赤"NOR RED"蠵龜"HIY"獵"NOR YEL"人"NOR,
	100		:	HIR"赤"NOR RED"蠵龜"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIR"赤"NOR RED"蠵龜"HIR"殺"NOR RED"手"NOR,
	2000	:	HIR"赤"NOR RED"蠵龜"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIR"赤"NOR RED"蠵龜"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("arc "+me->query_id(1));
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
	int is_named = query("named");
	object ob;

	if( !random(4) )
	{
		ob = new("/quest/lost_shard/lost_shard_1");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_helm" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_gigas_breast_plate",
				"/obj/armor/zen_gigas_leggings",
				"/obj/armor/zen_gigas_helm",
				"/obj/armor/zen_gigas_gloves",
				"/obj/armor/zen_gigas_cloak",			
				"/obj/armor/zen_gigas_bracers",
				"/obj/armor/zen_gigas_boots",
				"/obj/armor/zen_gigas_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/gigas_fist",
				"/obj/armor/gigas_breast_plate",
				"/obj/armor/gigas_leggings",
				"/obj/armor/gigas_helm",
				"/obj/armor/gigas_gloves",
				"/obj/armor/gigas_cloak",			
				"/obj/armor/gigas_bracers",
				"/obj/armor/gigas_boots",
				"/obj/armor/gigas_belt",
			});
		}
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	}

	TREASURE_D->get_treasure(this_object());
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, BOSS_HUNTER);	
	::reply_defeat(attackers);
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, BOSS_HUNTER);
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME揮舞著可怕的爪子往$YOU猛力攻擊，";
			break;
		case 1:
			return "$ME將四肢縮入殼中，接著旋轉龜殼攻擊$YOU，";
			break;
		case 2:
			return "$ME一口咬住$YOU的腿部，";
			break;
		case 3:
			return "$ME用尾巴狠狠地掃往$YOU的小腿，";
			break;
		case 4:
			return "$ME猛力地往$YOU衝刺攻擊，";
			break;
	}
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// 特殊攻擊
void special_attack(object attacker, object defender)
{
	if( !random(150) && !this_object()->in_condition(INVINCIBILITY))
	{
		msg("$ME吼叫一聲，使出「"HIR"完全"NOR RED"防禦"NOR"」！！\n"NOR, attacker, defender, 1);
		
		shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");
		
		this_object()->start_condition(INVINCIBILITY);
	}
}

int is_boss()
{
	return 2;
}

void auto_disappear()
{
	destruct(this_object());
}

void create()
{
	set_idname(HIR"caretta"NOR RED" gigas"NOR, HIR"赤"NOR RED"蠵龜"NOR);

	set("unit", "隻");
	set("age", 150);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 150);
	set("attr/phy", 350);
	set("attr/int", 100);
	set("attr/agi", 150);
	set("attr/cha", 20);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 15000);
	set("abi/health/cur", 15000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);
	set("job/name", HIW+"★★"+NOR);

	set("skills/unarmed/level", 130);
	set("skills/dodge/level", 130);
	set("skills/fatalblow/level", 3);
	
	set("long", "　一隻兇惡的赤蠵龜。");

	set("autokill", 1);

	if( !random(15) && clonep() )
	{
		MAPNPC_D->become_named_npc(this_object());
		call_out((: auto_disappear :), 3600 + random(1800));
	}

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
