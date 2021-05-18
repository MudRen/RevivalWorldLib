/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_1st_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-23
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
#include <buff.h>

inherit STANDARD_MONSTER;

mapping defeat_titles =
([
	10 		:	HIW"天"NOR WHT"禽"HIY"獵"NOR YEL"人"NOR,
	100		:	HIW"天"NOR WHT"禽"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIW"天"NOR WHT"禽"HIR"殺"NOR RED"手"NOR,
	2000	:	HIW"天"NOR WHT"禽"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIW"天"NOR WHT"禽"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	broadcast(query_temp("location", me), this_object()->query_idname()+"高聳的眼睛突然往下瞪了過來，發出轟隆巨響並伴隨大量的碎石落下！\n");
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
	object ob = new("/obj/inlay/sky_1st_secret_art");
	
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, SKY_1ST_STATUE);
	::reply_defeat(attackers);
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, SKY_1ST_STATUE);
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME舉起龐大石拳往$YOU猛力擊去，";
			break;
		case 1:
			return "$ME怒吼往下衝撞$YOU，";
			break;
		case 2:
			return "$ME抬起萬噸石腳往$YOU所在位置猛力踩下，";
			break;
		case 3:
			return "$ME揮動巨大的石臂往下掃往$YOU，";
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

void stop_angry(object attacker, object defender)
{
	if( !objectp(attacker) ) return;

	delete_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, attacker);
	delete_temp("buff/"+BUFF_COMBO_CHANCE, attacker);
	set_temp("buff/"+BUFF_FATAL_CHANCE, 10, attacker);

	attacker->reset_buff_cache();
	
	msg("\n$ME的憤怒漸漸平息下來...\n", attacker, defender, 1);
	msg("$ME的憤怒漸漸平息下來...\n", attacker, defender, 1);
	msg("$ME的憤怒漸漸平息下來...\n\n", attacker, defender, 1);
}

void start_angry(object attacker, object defender)
{
	if( !objectp(attacker) ) return;

	msg("\n$ME"HIY"的龐大身影開始模糊起來！！！！\n"NOR, attacker, defender, 1);
	msg("$ME"HIY"的龐大身影開始模糊起來！！！！\n"NOR, attacker, defender, 1);
	msg("$ME"HIY"的龐大身影開始模糊起來！！！！\n\n"NOR, attacker, defender, 1);

	set_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, 100, attacker);
	set_temp("buff/"+BUFF_COMBO_CHANCE, 40, attacker);
	set_temp("buff/"+BUFF_FATAL_CHANCE, 50, attacker);

	attacker->reset_buff_cache();
	
	call_out( (: stop_angry :), 30, attacker, defender );
}

void prepare_angry(object attacker, object defender)
{
	msg("\n$ME發出驚天劇吼：「"HIY"凡人敢爾！！！"NOR"」\n", attacker, defender, 1);
	msg("$ME發出驚天劇吼：「"HIY"凡人敢爾！！！"NOR"」\n", attacker, defender, 1);
	msg("$ME發出驚天劇吼：「"HIY"凡人敢爾！！！"NOR"」\n", attacker, defender, 1);
	
	msg(HIG+"\n這聲劇吼讓人覺得似乎大事不妙！！\n"NOR, attacker, 0, 1);
	msg(HIG+"這聲劇吼讓人覺得似乎大事不妙！！\n"NOR, attacker, 0, 1);
	msg(HIG+"這聲劇吼讓人覺得似乎大事不妙！！\n\n"NOR, attacker, 0, 1);
	
	call_out( (: start_angry :), 5, attacker, defender );
}

// 特殊攻擊
void special_attack(object attacker, object defender)
{
	int health;
	int damage;

	if( !random(100) )
		shout(HIB"遠方"NOR"-"+AREA_D->query_area_name(environment(attacker)->query_area())+"發生劇烈異變，陣陣深沉怒吼傳來憾動大地...。\n");

	if( !random(10) )
	{
		damage = range_random(200, 500);

		msg("$ME使出「"HIW"蔽"NOR WHT"日"HIW"飛"NOR WHT"石"NOR"」，大量巨石往$YOU飛擲過去！！\n"NOR, attacker, defender, 1);

		COMBAT_D->cause_damage(attacker, defender, damage);
	}

	health = query("abi/health/cur");
	
	if( health > 50000 )
	{
		delete_temp("healtimes");
		delete_temp("angry");
	}
	
	if( health < 30000 && addn_temp("healtimes", 1) <= 3 )
	{
		msg("\n一陣天搖地動，一道"HIW"白光"NOR"自其他兩座巨大石像直射過來，$ME生命瞬間恢復不少("NOR RED"+"HIR"3333"NOR")。\n", attacker, 0, 1);
		msg("一陣天搖地動，一道"HIW"白光"NOR"自其他兩座巨大石像直射過來，$ME生命瞬間恢復不少("NOR RED"+"HIR"3333"NOR")。\n", attacker, 0, 1);	
		msg("一陣天搖地動，一道"HIW"白光"NOR"自其他兩座巨大石像直射過來，$ME生命瞬間恢復不少("NOR RED"+"HIR"3333"NOR")。\n\n", attacker, 0, 1);	

		addn("abi/health/cur", 9999);
	}

	if( health < 15000 && addn_temp("angry", 1) <= 1 )
	{
		prepare_angry(attacker, defender);
	}
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"sky 1st "WHT"statue"NOR, HIW"天"NOR WHT"禽"NOR);

	set("unit", "尊");
	set("age", 650000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 350);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 300);
	set("attr/cha", 300);

	set("abi/stamina/max", 60000);
	set("abi/stamina/cur", 60000);
	set("abi/health/max", 60000);
	set("abi/health/cur", 60000);
	set("abi/energy/max", 60000);
	set("abi/energy/cur", 60000);
	set("job/name", HIW+"★★★★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);

	set_temp("buff/"+BUFF_FATAL_CHANCE, 10);
	set_temp("buff/"+BUFF_FATAL_POWER, 20);

	set("prestige", 1);// 死亡後給予玩家的聲望

	set("long", "　遠古諸神遺留在人間的巨大石像，與其他兩座石像並列在這塊大地上，遠遠望見一些\n白鳥在石像頂端盤旋，但附近卻是一點聲音也沒有，似乎隱藏著一些不為人知的秘密。");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
