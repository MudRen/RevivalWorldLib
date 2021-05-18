/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chelonia_mydas.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 綠蠵龜
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
	10 		:	HIG"綠"NOR GRN"蠵龜"HIY"獵"NOR YEL"人"NOR,
	100		:	HIG"綠"NOR GRN"蠵龜"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIG"綠"NOR GRN"蠵龜"HIR"殺"NOR RED"手"NOR,
	2000	:	HIG"綠"NOR GRN"蠵龜"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIG"綠"NOR GRN"蠵龜"WHT"死"HIW"神"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_2");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_gloves" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_mydas_breast_plate",
				"/obj/armor/zen_mydas_leggings",
				"/obj/armor/zen_mydas_helm",
				"/obj/armor/zen_mydas_gloves",
				"/obj/armor/zen_mydas_cloak",			
				"/obj/armor/zen_mydas_bracers",
				"/obj/armor/zen_mydas_boots",
				"/obj/armor/zen_mydas_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/mydas_fist",
				"/obj/armor/mydas_breast_plate",
				"/obj/armor/mydas_leggings",
				"/obj/armor/mydas_helm",
				"/obj/armor/mydas_gloves",
				"/obj/armor/mydas_cloak",			
				"/obj/armor/mydas_bracers",
				"/obj/armor/mydas_boots",
				"/obj/armor/mydas_belt",
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
	if( !random(20) )
	{
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker)  ) continue;
			
			msg("$ME將五體縮入殼中，使出「"HIG"龜殼"NOR CYN"共鳴"NOR"」對$YOU造成傷害！！！\n"NOR, attacker, ob, 1);

			COMBAT_D->cause_damage(attacker, ob, 50);
		}
	}
}

int is_boss()
{
	return 1;
}

void auto_disappear()
{
	destruct(this_object());
}

void create()
{
	set_idname(HIG"chelonia "NOR GRN"mydas"NOR, HIG"綠"NOR GRN"蠵龜"NOR);

	set("unit", "隻");
	set("age", 120);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 150);
	set("attr/phy", 350);
	set("attr/int", 100);
	set("attr/agi", 100);
	set("attr/cha", 20);

	set("abi/stamina/max", 3000);
	set("abi/stamina/cur", 3000);
	set("abi/health/max", 7000);
	set("abi/health/cur", 7000);
	set("abi/energy/max", 1000);
	set("abi/energy/cur", 1000);
	set("job/name", HIW+"★"+NOR);

	set("skills/unarmed/level", 80);
	set("skills/dodge/level", 80);
	set("skills/fatalblow/level", 3);

	set("long", "　一隻兇惡的綠蠵龜。");

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
