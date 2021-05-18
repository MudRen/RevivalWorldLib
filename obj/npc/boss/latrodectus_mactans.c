/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : latrodectus_mactans.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-15
 * Note   : 黑寡婦
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
	10 		:	WHT"黑"NOR BLU"寡婦"HIY"獵"NOR YEL"人"NOR,
	100		:	WHT"黑"NOR BLU"寡婦"HIB"剋"NOR BLU"星"NOR,
	1000	:	WHT"黑"NOR BLU"寡婦"HIR"殺"NOR RED"手"NOR,
	2000	:	WHT"黑"NOR BLU"寡婦"HIM"噬"NOR MAG"者"NOR,
	5000	:	WHT"黑"NOR BLU"寡婦"WHT"死"HIW"神"NOR,
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
		ob = new("/quest/lost_shard/lost_shard_3");
		ob->move(this_object());
	}

	{
		string* random_drop;
		
		if( !random(is_named ? 50 : 250) )
		{
			random_drop = ({ "/obj/armor/chi_lord_cloak" });
		}
		else if( !random(is_named ? 10 : 50) )
		{
			random_drop =
			({
				"/obj/armor/zen_mactans_breast_plate",
				"/obj/armor/zen_mactans_leggings",
				"/obj/armor/zen_mactans_helm",
				"/obj/armor/zen_mactans_gloves",
				"/obj/armor/zen_mactans_cloak",			
				"/obj/armor/zen_mactans_bracers",
				"/obj/armor/zen_mactans_boots",
				"/obj/armor/zen_mactans_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/mactans_fist",
				"/obj/armor/mactans_breast_plate",
				"/obj/armor/mactans_leggings",
				"/obj/armor/mactans_helm",
				"/obj/armor/mactans_gloves",
				"/obj/armor/mactans_cloak",			
				"/obj/armor/mactans_bracers",
				"/obj/armor/mactans_boots",
				"/obj/armor/mactans_belt",
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
			return "$ME露出可怕的毒牙往$YOU一口咬來，";
			break;
		case 1:
			return "$ME快速往前竄往$YOU的腰部咬來，";
			break;
		case 2:
			return "$ME利用多隻腳將$YOU困住進行攻擊，";
			break;
		case 3:
			return "$ME從側面快速攻擊$YOU，";
			break;
		case 4:
			return "$ME從遠處直接往$YOU的頭部噴出毒液，";
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
	if( !random(30) )
	{
		msg("$ME使出「"HIC"八爪"NOR CYN"迴旋"NOR"」在$YOU的身上劃出數道傷痕！！！\n", attacker, defender, 1);
		defender->start_condition(POISON_GIANT_SPIDER, 60, 5, attacker);
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
	set_idname(WHT"latrodectus "NOR BLU"mactans"NOR, WHT"黑"NOR BLU"寡婦"NOR);

	set("unit", "隻");
	set("age", 20);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
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

	set("long", "　一隻附有劇毒的蜘蛛，隨時準備進行攻擊。");

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
