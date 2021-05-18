/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : forgotten_ancient_knight.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
 * Note   : 被遺忘的遠古武士
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
	10 		:	WHT"武"HIW"士"HIY"獵"NOR YEL"人"NOR,
	100		:	WHT"武"HIW"士"HIB"剋"NOR BLU"星"NOR,
	1000	:	WHT"武"HIW"士"HIR"殺"NOR RED"手"NOR,
	2000	:	WHT"武"HIW"士"HIM"噬"NOR MAG"者"NOR,
	5000	:	WHT"武"HIW"士"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("hmm "+me->query_id(1));
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
	int num = environment(this_object())->query_num();
	object ob;
	string* random_drop;
	
	CHANNEL_D->channel_broadcast("combat", this_object()->query_idname()+"靜靜地躺在"+AREA_D->query_area_idname("lostland", num)+"的大地上，被詛咒的靈魂化為"HIR"七"HIY"彩"HIG"光"HIC"芒"NOR"，逐漸消散...");
	
	if( !random(is_named ? 8 : 40) )
	{
		random_drop = ({ "/obj/armor/chi_lord_leggings" });
	}
	else if( !random(is_named ? 4 : 20) )
	{
		random_drop =
		({
			"/obj/weapon/ancient_blade",
			"/obj/armor/zen_ancient_breast_plate",
			"/obj/armor/zen_ancient_leggings",
			"/obj/armor/zen_ancient_helm",
			"/obj/armor/zen_ancient_gloves",
			"/obj/armor/zen_ancient_cloak",			
			"/obj/armor/zen_ancient_bracers",
			"/obj/armor/zen_ancient_boots",
			"/obj/armor/zen_ancient_belt",
		});
	}
	else
	{
		random_drop =
		({
			"/obj/weapon/ancient_blade",
			"/obj/armor/ancient_breast_plate",
			"/obj/armor/ancient_leggings",
			"/obj/armor/ancient_helm",
			"/obj/armor/ancient_gloves",
			"/obj/armor/ancient_cloak",			
			"/obj/armor/ancient_bracers",
			"/obj/armor/ancient_boots",
			"/obj/armor/ancient_belt",
		});
	}

	ob = new(random_drop[random(sizeof(random_drop))]);
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
	
	if( !random(2) )
	{
		broadcast(environment(this_object()), NOR WHT"天邊傳來細細耳語："NOR"「勇者們要小心吶，遠古的惡魔就在各位眼前了，願諸位能替我完成心願...」\n");
		ob = new("/quest/lost_shard/lostland_key_2");
		ob->move(this_object());
	}
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, LOSTSEAL);	
	::reply_defeat(attackers);
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, LOSTSEAL);
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
	/*
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
	*/
}

// 防禦時的訊息
varargs string combat_defend_message(object attacker, object weapon)
{
	
	
}

// 特殊攻擊
void special_attack(object attacker, object defender)
{
	int time;

	if( !random(200) )
	{
		shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");
	}

	if( !random(100) )
	{
		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) ) continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
			
			time = range_random(1, 5);

			msg("$ME一拳擊向地面，使出「"HIY"萬均"NOR YEL"裂地"NOR"」，使$YOU造成短暫暈眩("+time+" 秒)，並造成輕微的傷害！！！\n"NOR, attacker, ob, 1);
			
			ob->start_delay("forgotten_ancient_knight", time, pnoun(2, ob)+"因為「"HIY"萬均"NOR YEL"裂地"NOR"」的影響而暈眩中。\n", pnoun(2, ob)+"停止了暈眩。\n");
			
			COMBAT_D->cause_damage(attacker, ob, 50);
		}
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(NOR WHT"forgotten"HIW" ancient knight"NOR, NOR WHT"被遺忘的"HIW"遠古武士"NOR);

	set("unit", "位");
	set("age", 50);
	set("gender", MALE_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 250);
	set("attr/cha", 300);

	set("abi/stamina/max", 30000);
	set("abi/stamina/cur", 30000);
	set("abi/health/max", 30000);
	set("abi/health/cur", 30000);
	set("abi/energy/max", 30000);
	set("abi/energy/cur", 30000);
	set("job/name", HIW+"★★★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 6);

	set("prestige", 1);// 死亡後給予玩家的聲望

	set("long", "　一位被遺忘的遠古武士，靈魂受到了詛咒，意識中只存在著血腥、屠殺、憎恨。");

	set("autokill", 1);

	startup_living();
	
	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
}

int remove()
{
	destruct(all_inventory(this_object()));
}
