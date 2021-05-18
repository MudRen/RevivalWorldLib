/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_rhombopteryx.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : 尼斯湖水怪
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
	10 		:	HIB"尼斯"NOR MAG"怪"HIY"獵"NOR YEL"人"NOR,
	100		:	HIB"尼斯"NOR MAG"怪"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIB"尼斯"NOR MAG"怪"HIR"殺"NOR RED"手"NOR,
	2000	:	HIB"尼斯"NOR MAG"怪"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIB"尼斯"NOR MAG"怪"WHT"死"HIW"神"NOR,
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
		random_drop = ({ "/obj/armor/chi_lord_breast_plate" });
	}
	else if( !random(is_named ? 4 : 20) )
	{
		random_drop =
		({
			"/obj/weapon/nessiteras_fist",
			"/obj/armor/zen_nessiteras_breast_plate",
			"/obj/armor/zen_nessiteras_leggings",
			"/obj/armor/zen_nessiteras_helm",
			"/obj/armor/zen_nessiteras_gloves",
			"/obj/armor/zen_nessiteras_cloak",			
			"/obj/armor/zen_nessiteras_bracers",
			"/obj/armor/zen_nessiteras_boots",
			"/obj/armor/zen_nessiteras_belt",
		});
	}
	else
	{
		random_drop =
		({
			"/obj/weapon/nessiteras_fist",
			"/obj/armor/nessiteras_breast_plate",
			"/obj/armor/nessiteras_leggings",
			"/obj/armor/nessiteras_helm",
			"/obj/armor/nessiteras_gloves",
			"/obj/armor/nessiteras_cloak",			
			"/obj/armor/nessiteras_bracers",
			"/obj/armor/nessiteras_boots",
			"/obj/armor/nessiteras_belt",
		});
	}

	ob = new(random_drop[random(sizeof(random_drop))]);
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
	
	if( !random(2) )
	{
		broadcast(environment(this_object()), this_object()->query_idname()+"的肚子裡隱隱有個金屬物在發光...\n");
		ob = new("/quest/lost_shard/lostland_key_3");
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
	switch(random(5))
	{
		case 0:
			return "$ME揮舞著可怕的巨爪往$YOU猛力攻擊，";
			break;
		case 1:
			return "$ME怒吼往下衝撞$YOU，";
			break;
		case 2:
			return "$ME抬起巨腳往$YOU所在位置猛力踩下，";
			break;
		case 3:
			return "$ME以萬斤力量將尾巴掃往$YOU，";
			break;
		case 4:
			return "$ME猛力地往$YOU衝撞攻擊，";
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
	int time;
	int damage;

	if( !random(100) )
	{
		if( !random(2) )
			shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");

		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( !random(2) )
				continue;

			if( ob == attacker || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;

			time = range_random(5, 10);

			damage = range_random(50, 200);

			msg("$ME張開大嘴往$YOU大吼，使出「"HIR"震古"NOR RED"鑠今"NOR"」，使$YOU造成短暫暈眩("HIY+time+NOR" 秒)，並造成嚴重的傷害！！！\n"NOR, attacker, ob, 1);
			
			ob->start_delay("nessiteras_rhombopteryx", time, pnoun(2, ob)+"因為「"HIR"震古"NOR RED"鑠今"NOR"」的影響而暈眩中。\n", pnoun(2, ob)+"停止了暈眩。\n");
				
			COMBAT_D->cause_damage(attacker, ob, damage);
		}
		
		attacker->start_delay("nessiteras_rhombopteryx", 5, pnoun(2, attacker)+"因為「"HIR"震古"NOR RED"鑠今"NOR"」的影響而暈眩中。\n", pnoun(2, attacker)+"停止了暈眩。\n");
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(HIB"nessiteras"NOR MAG" rhombopteryx"NOR, HIB"尼斯湖"NOR MAG"水怪"NOR);

	set("unit", "隻");
	set("age", 600);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 250);
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

	set("long", "　潛藏在水底深處的水怪，長長的脖子和扁小的頭部，看上去完全不像任何一種的水生動物，而很像早七千多萬年前滅絕的巨大爬行動物--蛇頸龍。");

	set("autokill", 1);

	startup_living();
	
	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
}

int remove()
{
	destruct(all_inventory(this_object()));
}
