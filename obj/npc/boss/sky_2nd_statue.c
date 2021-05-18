/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky_2nd_statue.c
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
	10 		:	HIW"天"NOR WHT"劍"HIY"獵"NOR YEL"人"NOR,
	100		:	HIW"天"NOR WHT"劍"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIW"天"NOR WHT"劍"HIR"殺"NOR RED"手"NOR,
	2000	:	HIW"天"NOR WHT"劍"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIW"天"NOR WHT"劍"WHT"死"HIW"神"NOR,
]);

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	broadcast(query_temp("location", me), this_object()->query_idname()+"高聳的眼睛突然往下瞪了過來，伴隨大量的碎石落下！\n");
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
	object ob = new("/obj/inlay/sky_2nd_secret_art");
	
	ob->move(this_object());

	TREASURE_D->get_treasure(this_object());
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::start_cooldown(attackers, SKY_2ND_STATUE);	
	::reply_defeat(attackers);
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	return ::check_cooldown(enemy, SKY_2ND_STATUE);
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

// 特殊攻擊
int special_attack(object attacker, object defender)
{
	if( !random(100) )
		shout(HIB"遠方"NOR"-"+AREA_D->query_area_name(environment(attacker)->query_area())+"發生劇烈異變，陣陣深沉怒吼傳來憾動大地...。\n");
		
	if( !random(25) )
	{
		object small_statue = new("/obj/npc/boss/small_statue");
		
		small_statue->move_to_environment(attacker);
		
		msg("\n$ME低聲悶雷呼喊：「"HIY"去吧！"NOR"」，一塊巨石從$ME身上落下，幻化成$YOU並加入戰鬥。\n\n", attacker, small_statue, 1);
		
		COMBAT_D->start_fight(small_statue, defender);
	}
}

int is_boss()
{
	return 4;
}

void create()
{
	set_idname(HIW"sky 2nd "WHT"statue"NOR, HIW"天"NOR WHT"劍"NOR);

	set("unit", "尊");
	set("age", 650000);
	set("gender", MALE_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 300);
	set("attr/int", 300);
	set("attr/agi", 350);
	set("attr/cha", 300);

	set("abi/stamina/max", 100000);
	set("abi/stamina/cur", 100000);
	set("abi/health/max", 100000);
	set("abi/health/cur", 100000);
	set("abi/energy/max", 100000);
	set("abi/energy/cur", 100000);
	set("job/name", HIW+"★★★★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);

	set_temp("buff/"+BUFF_FATAL_CHANCE, 10);
	set_temp("buff/"+BUFF_COUNTERATTACK_CHANCE, 10);

	set("prestige", 1);// 死亡後給予玩家的聲望

	set("long", "　遠古諸神遺留在人間的巨大石像，與其他兩座石像並列在這塊大地上，遠遠望見一些\n白鳥在石像頂端盤旋，但附近卻是一點聲音也沒有，似乎隱藏著一些不為人知的秘密。");

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
