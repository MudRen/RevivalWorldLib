/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : urocissa_caerulea.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 臺灣藍鵲
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
	10 		:	HIB"藍"NOR BLU"鵲"HIY"獵"NOR YEL"人"NOR,
	100		:	HIB"藍"NOR BLU"鵲"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIB"藍"NOR BLU"鵲"HIR"殺"NOR RED"手"NOR,
	2000	:	HIB"藍"NOR BLU"鵲"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIB"藍"NOR BLU"鵲"WHT"死"HIW"神"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_5");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_boots" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_caerulea_breast_plate",
				"/obj/armor/zen_caerulea_leggings",
				"/obj/armor/zen_caerulea_helm",
				"/obj/armor/zen_caerulea_gloves",
				"/obj/armor/zen_caerulea_cloak",			
				"/obj/armor/zen_caerulea_bracers",
				"/obj/armor/zen_caerulea_boots",
				"/obj/armor/zen_caerulea_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/caerulea_fist",
				"/obj/armor/caerulea_breast_plate",
				"/obj/armor/caerulea_leggings",
				"/obj/armor/caerulea_helm",
				"/obj/armor/caerulea_gloves",
				"/obj/armor/caerulea_cloak",			
				"/obj/armor/caerulea_bracers",
				"/obj/armor/caerulea_boots",
				"/obj/armor/caerulea_belt",
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
			return "$ME俯衝向下，攻擊$YOU，";
			break;
		case 2:
			return "$ME一嘴啄往$YOU的腿部，";
			break;
		case 3:
			return "$ME用翅膀狠狠地掃往$YOU的背部，";
			break;
		case 4:
			return "$ME猛力地往$YOU啄刺攻擊，";
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
	if( !random(200) )
	{
		shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");
	}

	if( !random(20) )
	{
		int damage;
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			damage = range_random(80,120);

			msg("$ME直衝雲頂，使出「"HIW"萬羽"NOR WHT"飛箭"NOR"」對$YOU造成嚴重傷害！！！\n"NOR, attacker, ob, 1);
			
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
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
	set_idname(HIB"urocissa "NOR BLU"caerulea"NOR, HIB"臺灣"NOR BLU"藍鵲"NOR);

	set("unit", "隻");
	set("age", 50);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
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

	set("long", "　一隻羽毛亮麗的臺灣藍鵲。");

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
