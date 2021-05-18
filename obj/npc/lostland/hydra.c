/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-11-25
 * Note   : 九頭蛇海德拉
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

int reborn_time = 0;
int call_out_handle;

mapping defeat_titles =
([
	10 		:	HIW"海德拉"HIY"獵"NOR YEL"人"NOR,
	100		:	HIW"海德拉"HIB"剋"NOR BLU"星"NOR,
	1000	:	HIW"海德拉"HIR"殺"NOR RED"手"NOR,
	2000	:	HIW"海德拉"HIM"噬"NOR MAG"者"NOR,
	5000	:	HIW"海德拉"WHT"死"HIW"神"NOR,
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
	if( reborn_time == 8 )
	{
		int is_named = query("named");
		int num = environment(this_object())->query_num();
		object ob;
		string* random_drop;
		
		CHANNEL_D->channel_broadcast("combat", this_object()->query_idname()+"靜靜地躺在"+AREA_D->query_area_idname("lostland", num)+"的大地上，遠古惡靈化為"HIR"七"HIY"彩"HIG"光"HIC"芒"NOR"，逐漸消散...");
		
		foreach(ob in users())
		{
			if( ob->is_user_ob() && interactive(ob) && !wizardp(ob) )
				ob->start_condition(LOSTLAND);
		}
		
		if( !random(is_named ? 8 : 40) )
		{
			random_drop = ({ "/obj/weapon/lord_sword" });
		}
		else if( !random(is_named ? 4 : 20) )
		{
			random_drop =
			({
				"/obj/weapon/hydra_stick",
				"/obj/armor/zen_hydra_breast_plate",
				"/obj/armor/zen_hydra_leggings",
				"/obj/armor/zen_hydra_helm",
				"/obj/armor/zen_hydra_gloves",
				"/obj/armor/zen_hydra_cloak",			
				"/obj/armor/zen_hydra_bracers",
				"/obj/armor/zen_hydra_boots",
				"/obj/armor/zen_hydra_belt",
			});
		}
		else
		{
			random_drop =
			({
				"/obj/weapon/hydra_stick",
				"/obj/armor/hydra_breast_plate",
				"/obj/armor/hydra_leggings",
				"/obj/armor/hydra_helm",
				"/obj/armor/hydra_gloves",
				"/obj/armor/hydra_cloak",			
				"/obj/armor/hydra_bracers",
				"/obj/armor/hydra_boots",
				"/obj/armor/hydra_belt",
			});
		}
	
		ob = new(random_drop[random(sizeof(random_drop))]);
		ob->move(this_object());
	
		TREASURE_D->get_treasure(this_object());
	}
	else
	{
		call_out( (: destruct(this_object()) :), 1);
	}
}

// 被擊敗時的事件
void reply_defeat(object* attackers)
{
	if( reborn_time == 8 )
	{
		::reply_defeat(attackers);
		::start_cooldown(attackers, LOSTSEAL);
		
		remove_call_out(call_out_handle);
	}
	else
	{
		object ob = new(base_name(this_object()), reborn_time + 1, attackers);
			
		ob->move_to_environment(this_object());
			
		msg("$ME對著所有人怒吼著。\n", ob, 0, 1);
	}
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
			return "$ME所有的頭同時往$YOU猛力攻擊，";
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
	int times;
	int damage;

	if( !random(100) )
	{
		if( !random(2) )
			shout(HIB"遠方"NOR"-大地因"+attacker->query_idname()+"的怒吼震動了一下。\n");

		msg("$ME張開大嘴連續向$YOU噴出大量「"HIW"積屍"NOR WHT"毒泥"NOR"」，造成$YOU嚴重的傷害！！\n"NOR, attacker, defender, 1);
		
		times = random(8)+1;

		while(times--)
		{
			damage += range_random(40, 80);
			msg("$ME張開大嘴連續向$YOU噴出大量「"HIW"積屍"NOR WHT"毒泥"NOR"」，造成$YOU嚴重的傷害！！\n"NOR, attacker, defender, 1);
		}
		
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
	
	if( !random(100) )
	{
		int x = 49;
		int y = 49;
		array loc;

		while(x == 49)
			x = range_random(44, 54);
		while(y == 49)
			y = range_random(44, 54);

		foreach(object ob in COMBAT_D->query_fighting_targets(attacker))
		{
			if( random(10) )
				continue;

			if( ob == attacker || ob == defender || !ob->is_living() || ob->is_faint() || ob->is_dead() || ob->in_condition(INVINCIBILITY) || !same_environment(ob, attacker) ) continue;
		
			loc = arrange_area_location(x, y, "lostland", 2);
					
			msg("$ME在體內續積能量，猛然往四周爆出「"HIY"撼天"NOR YEL"震波"NOR"」，將$YOU震飛至"+loc_short(loc)+"！！\n"NOR, attacker, ob, 1);
			
			ob->move(loc);
			
			msg("$ME被$YOU震飛到"+loc_short(loc)+"。\n", ob, attacker, 1);
		}
	}
}

int is_boss()
{
	return 3;
}

void create(int i, object* attackers)
{
	reborn_time = i;

	set_idname(HIREV WHT"hy"HIW"dra"NOR, HIREV WHT+CHINESE_D->chinese_number(9-i)+"頭蛇"HIW"海德拉"NOR);

	set("unit", "隻");
	set("age", 3000);
	set("gender", ANIMAL_GENDER);
	
	set("attr/str", 300);
	set("attr/phy", 250);
	set("attr/int", 300);
	set("attr/agi", 250);
	set("attr/cha", 300);

	set("abi/stamina/max", (i+1)*5000);
	set("abi/stamina/cur", (i+1)*5000);
	set("abi/health/max", (i+1)*5000);
	set("abi/health/cur", (i+1)*5000);
	set("abi/energy/max", (i+1)*5000);
	set("abi/energy/cur", (i+1)*5000);
	set("job/name", HIW+"★★★"+NOR);

	set("skills/unarmed/level", 160);
	set("skills/dodge/level", 160);
	set("skills/fatalblow/level", 6);

	set("long", "　希臘神話中最強悍的怪物之父百首巨怪提風(Typhon)和女首蛇身怪厄喀德那(Echinda)所生，砍掉每一顆頭都會再生，據說中間那顆頭是永生不死的。");

	set("autokill", 1);
	
	if( reborn_time == 8 )
	{
		set("prestige", 1);// 死亡後給予玩家的聲望
		
		if( !random(15) )
			MAPNPC_D->become_named_npc(this_object());
		
		call_out_handle = "/system/daemons/mapnpc/area_lostland_2"->delay_start_cooldown(attackers);
	}

	startup_living();
}

int remove()
{
	destruct(all_inventory(this_object()));
}
