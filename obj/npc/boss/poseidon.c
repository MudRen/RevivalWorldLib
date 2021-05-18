/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : poseidon.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : 波塞冬(由 SHOPPING_D 驅動產生，當波塞冬出現時將導致全世界經濟蕭條)
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
	10 		:	HIC"波"HIR"塞"HIG"冬"HIY"獵"NOR YEL"人"NOR,
	100		:	HIC"波"HIR"塞"HIG"冬"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIC"波"HIR"塞"HIG"冬"HIR"殺"NOR RED"手"NOR,
	2000	:	HIC"波"HIR"塞"HIG"冬"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIC"波"HIR"塞"HIG"冬"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	this_object()->do_command("say "+me->query_name()+"膽敢對我不敬？");
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

	ob = new("/quest/epic_fishfarm/heart_of_ocean");
	ob->move(this_object());

	if( !random(is_named ? 10 : 50) )
	{
		if( !random(is_named ? 10 : 50) )
			ob = new("/obj/armor/zen_poseidon_cloak");
		else
			ob = new("/obj/armor/poseidon_cloak");

		ob->move(this_object());
	}

	if( !random(is_named ? 2 : 10) )
	{
		ob = new("/obj/weapon/faith_halberd");
		ob->move(this_object());
	}

	TREASURE_D->get_treasure(this_object());

	CHANNEL_D->channel_broadcast("news", this_object()->query_idname()+"大吼：「無知的人類！我還會再回來的！」...隨著祂的消失，海平面逐漸恢復平靜...");
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
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
			return "$ME舉起三叉戟往$YOU猛力刺擊，";
			break;
		case 1:
			return "$ME怒吼往下衝撞$YOU，";
			break;
		case 2:
			return "$ME抬起巨腳往$YOU所在位置猛力踩下，";
			break;
		case 3:
			return "$ME以萬斤力量將身體掃往$YOU，";
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
	int damage;

	if( !random(10) )
	{
		if( !random(2) )
			shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");

		damage = range_random(200, 500);

		msg("$ME舉起巨大的三叉戟猛力地掃過海平面，「"HIC"海嘯"NOR CYN"音波"NOR"」直接灌入$YOU的身體，造成嚴重傷害！！\n"NOR, attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}

int is_boss()
{
	return 3;
}

void create()
{
	set_idname(HIC"po"HIR"sei"HIG"don"NOR, HIC"波"HIR"塞"HIG"冬"NOR);

	set("unit", "尊");
	set("age", 4000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 250);
	set("attr/phy", 250);
	set("attr/int", 250);
	set("attr/agi", 200);
	set("attr/cha", 200);

	set("abi/stamina/max", 50000);
	set("abi/stamina/cur", 50000);
	set("abi/health/max", 50000);
	set("abi/health/cur", 50000);
	set("abi/energy/max", 50000);
	set("abi/energy/cur", 50000);
	set("job/name", HIW+"★★★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 100);

	set("prestige", 1);// 死亡後給予玩家的聲望

	set("long", "　波塞冬，身體巨大，為希臘神話當中的海神，宙斯的哥哥。波塞冬野心勃勃，而且好\n戰。不滿足於他所擁有的權力，當他憤怒時海底就會出現怪物，他揮動三叉戟就能引起\n海嘯和地震。");

	set("autokill", 1);

	if( !random(15) )
		MAPNPC_D->become_named_npc(this_object());
		
	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
