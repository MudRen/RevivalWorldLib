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
#include <autokill.h>

inherit STANDARD_MONSTER;

string ownerid;

void do_command(string cmd);

/*
void reply_say(object me, string msg)
{

}
*/

void reply_emote(object me, string verb, string args)
{
	
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

}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	::reply_defeat(attackers);
}

// 檢查是否為合法戰鬥目標
int valid_enemy(object enemy)
{
	if( !query("boss") )
		return 1;

	if( userp(enemy) || enemy->is_module_npc() )
		return 0;
	
	return 1;	
}

int is_summon_mob()
{
	return stringp(query("boss"));
}

// 攻擊時的訊息
varargs string combat_attack_message(object target, object weapon)
{
	switch(random(5))
	{
		case 0:
			return "$ME舉起龐小石拳往$YOU猛力擊去，";
			break;
		case 1:
			return "$ME怒吼往下衝撞$YOU，";
			break;
		case 2:
			return "$ME抬起一噸石腳往$YOU所在位置猛力踩下，";
			break;
		case 3:
			return "$ME揮動小小的石臂往下掃往$YOU，";
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

}

void destruct_self()
{
	msg("\n$ME突然碎成一地，化為塵土...。\n\n", this_object(), 0, 1);	
	
	destruct(this_object());
}

private void do_follow_assist(object owner)
{
	if( objectp(owner) )
	{
		do_command("follow "+owner->query_id(1));
		do_command("assist "+owner->query_id(1));

		foreach(object enemy in COMBAT_D->query_fighting_targets(owner))
			COMBAT_D->start_fight(this_object(), enemy);
	}
}

varargs void create(object owner)
{
	int has_owner = objectp(owner);

	if( has_owner )
	{
		set_idname(WHT"small statue"NOR, WHT"小石像"NOR);
		set("long", "  "+owner->query_name()+"召喚出來的小石像。");
		
		set("boss", owner->query_id(1));
		
		set_temp("protectors", ({ this_object() }) | (query_temp("protectors", owner)||allocate(0)), owner);

		set("skills/defend-stance-adv/level", 100);
		set("combat/stance/type", "defend-adv");
	
		call_out( (: do_follow_assist :), 0, owner);
	}
	else
	{
		set_idname(HIW"small "WHT"statue"NOR, HIW"小"NOR WHT"石像"NOR);
		set("long", "　天劍召喚出來的小石像。");
	}
	
	set("unit", "尊");
	set("age", 1500);
	set("gender", MALE_GENDER);
	
	set("attr/str", has_owner ? to_int(owner->query_str()*0.4) : 200);
	set("attr/phy", has_owner ? to_int(owner->query_phy()*0.4) : 200);
	set("attr/int", has_owner ? to_int(owner->query_int()*0.4) : 200);
	set("attr/agi", has_owner ? to_int(owner->query_agi()*0.4) : 200);
	set("attr/cha", has_owner ? to_int(owner->query_cha()*0.4) : 200);

	set("abi/stamina/max", has_owner ? owner->query_stamina_max() : 5000);
	set("abi/stamina/cur", has_owner ? owner->query_stamina_max() : 5000);
	set("abi/health/max", has_owner ? owner->query_health_max() : 5000);
	set("abi/health/cur", has_owner ? owner->query_health_max() : 5000);
	set("abi/energy/max", has_owner ? owner->query_energy_max() : 5000);
	set("abi/energy/cur", has_owner ? owner->query_energy_max() : 5000);
	set("job/name", HIW+"★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	
	if( clonep() )
		call_out( (: destruct_self :), has_owner ? 30 : 60 );

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
