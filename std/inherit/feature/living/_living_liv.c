/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _living.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-17
 * Note   : 生物繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <buff.h>
#include <login.h>
#include <condition.h>
#include <skill.h>
#include <autokill.h>

#define HEART_BEAT_TICK		1

nomask void startup_heartbeat()
{
	set_heart_beat(HEART_BEAT_TICK);
}

nomask void stop_heartbeat()
{
	set_heart_beat(0);
}

// 啟動生命特性
nomask void startup_living()
{
	if( !this_object()->is_user_ob() )
		CHAR_D->create_npc(this_object());

	startup_heartbeat();
	
	if( this_object()->is_dead() )
		this_object()->revive();
}

nomask int is_living()
{
	return 1;	
}

int query_inventory_slot()
{
	return 10+this_object()->query_str()/4+ this_object()->query_condition_buff(BUFF_SLOT_MAX) + this_object()->query_equipment_buff(BUFF_SLOT_MAX);
}


nomask int query_loading()
{
	int loading, mass;
	
	foreach(object ob in all_inventory(this_object()))
	{
		mass = query("mass", ob);
		
		if( mass > 0 && !this_object()->is_equipping_object(ob) )
			loading += mass * ob->query_amount();
	}
	
	return loading;	
}

nomask int query_max_loading()
{
	return 1000*(this_object()->query_str() + this_object()->query_condition_buff(BUFF_LOADING_MAX) + this_object()->query_equipment_buff(BUFF_LOADING_MAX));
}


// 撿查是否允許撿拾物品
varargs nomask int available_get(object ob, int amount)
{
	int mass = query("mass", ob);
	
	if( undefinedp(amount) || !amount )
		amount = ob->query_amount();

	// 若超過負重則 return 0
	if( mass == -1 || amount * mass > query_max_loading()-query_loading() )
		return 0;

	if( ob->no_amount() )
	{
		if( sizeof(all_inventory()) + amount > query_inventory_slot() )
			return 0;
	}
	else if( sizeof(all_inventory()) + 1 > query_inventory_slot() )
	{
		string basename = base_name(ob);

		if( !sizeof(filter_array(all_inventory(), (: base_name($1) == $(basename) :))) )
			return 0;
	}
	return 1;
}

// 暈倒
nomask void faint()
{
	if( query_temp("no_faint") ) return;

	// 不管如何一暈倒就將所有能力值降為 0
	this_object()->set_all_ability_zero();

	msg("\e[1;31m$ME\e[1;31m突然眼前一堆小星星，昏倒了。\n\e[m", this_object(), 0, 1);
	
	set("faint", 1);
	
	if( !BATTLEFIELD_D->inside_battlefield(this_object()) )
		set_temp("disable/msg", 1);

	set_temp("disable/cmd", 1);
	
	startup_heartbeat();
}

// 甦醒	
nomask void wakeup()
{
	delete("faint");
	delete_temp("disable/msg");
	delete_temp("disable/cmd");
	
	msg("$ME逐漸地恢復了意識。\n", this_object(), 0, 1);
	this_object()->show_msg_buffer();
	this_object()->show_prompt();
}

// 是否暈倒
nomask int is_faint()
{
	return query("faint");
}

// 復活
nomask void revive()
{
	if( !query("die") ) return;

	msg("$ME的屍體腐爛消失了...。\n", this_object(), 0, 1);

	delete("die");
	delete_temp("disable/msg");
	delete_temp("disable/cmd");
	
	msg("$ME復活了。\n", this_object(), 0, 1);
	startup_heartbeat();

	if( this_object()->is_user_ob() )
	{
		if( BATTLEFIELD_D->inside_battlefield(this_object()) )
		{
			this_object()->set_all_ability_full();
			BATTLEFIELD_D->move_to_random_loc(this_object());
		}
		else
			this_object()->start_condition(DIE);

		this_object()->show_msg_buffer();
		this_object()->show_prompt();
	}
}

nomask void delay_destruct()
{
	destruct(this_object());
}

// 屍體消失
nomask void corpse_disappear()
{
	if( this_object()->is_user_ob() )
		return;

	msg("$ME的屍體漸漸腐爛消失了。\n", this_object(), 0, 1);
	
	destruct(this_object());
}

#define DIE_LOST_SKILL		({ "strength", "physique", "intelligence", "agility", "charisma", "stamina", "health", "energy", "strength-adv", "physique-adv", "intelligence-adv", "agility-adv", "charisma-adv", "stamina-adv", "health-adv", "energy-adv" })
// 死亡
nomask void die()
{
	int in_battle = BATTLEFIELD_D->inside_battlefield(this_object());
	int in_life_miracle = this_object()->in_condition(LIFE_MIRACLE);
	this_object()->set_all_ability_zero();
	delete("faint");
	delete_temp("disable/msg");
	delete_temp("disable/cmd");
	
	msg("\n"HIR"$ME"HIR"死了！！！\n"NOR, this_object(), 0, 1);

	if( !in_battle && !in_life_miracle )
	{
		this_object()->reset_fatigue();
		this_object()->stop_condition(CONDITION_TYPE_NEGATIVE);
		this_object()->stop_condition(CONDITION_TYPE_POSITIVE);
	}
	else if( in_battle )
		this_object()->stop_condition(CONDITION_TYPE_NEGATIVE);

	this_object()->cost_food(500);
	this_object()->cost_drink(500);		
	this_object()->reply_die();

	if( this_object()->is_user_ob() || this_object()->is_module_npc() )
	{
		if( !in_battle && !in_life_miracle )
		{
			string skill;
			string *available_skills = allocate(0);
	
			foreach(skill in DIE_LOST_SKILL)
				if( this_object()->query_skill_level(skill) > 0 )
					available_skills |= ({ skill });
			
			if( sizeof(available_skills) )
			{
				int social_tot_exp = this_object()->query_social_exp_tot();
				int combat_tot_exp = this_object()->query_combat_exp_tot();
				int lost_exp;
				
				skill = available_skills[random(sizeof(available_skills))];	
				
				lost_exp = this_object()->query_skill_exp(skill)/50./(SKILL(skill))->exp_multiple();
				
				if( social_tot_exp >= lost_exp && combat_tot_exp >= lost_exp )
				{
					if( !random(2) )
						set("exp/social/tot", social_tot_exp - lost_exp);
					else
						set("exp/combat/tot", combat_tot_exp - lost_exp);
				}
				else if( social_tot_exp >= lost_exp )
					set("exp/social/tot", social_tot_exp - lost_exp);
				else if( combat_tot_exp >= lost_exp )
					set("exp/combat/tot", combat_tot_exp - lost_exp);
					
				this_object()->add_skill_exp(skill, -lost_exp);
			}
		}

		if( this_object()->is_user_ob() )
		{
			log_file("command/die", this_object()->query_idname()+" "+sprintf("%O\n", call_stack()), -1);
			
			if( !in_battle )
			{
				set("killlock", time());
				call_out( (: revive :), 30 );
			}
			else
			{
				call_out( (: revive :), 3 );
			}
		}
		else
			call_out( (: revive :), 300 );
	}
	else if( this_object()->is_boss() )
		call_out( (: corpse_disappear :), 600 );
	else
		call_out( (: corpse_disappear :), 180 );
		
	if( in_life_miracle )
	{
		if( !in_battle )
			this_object()->stop_condition(LIFE_MIRACLE);

		this_object()->stop_condition(CONDITION_TYPE_NEGATIVE);
	}



	set("die", 1);
	
	if( !BATTLEFIELD_D->inside_battlefield(this_object()) )
		set_temp("disable/msg", 1);
	
	set_temp("disable/cmd", 1);
	set("last_die_time", time());
}

int valid_enemy(object enemy)
{
	return 1;
}

private void autokill_start(object attacker, object defender)
{
	if( 
		!objectp(attacker) || 
		!objectp(defender) || 
		!attacker->is_living() || 
		!defender->is_living() || 
		attacker->is_faint() ||
		attacker->is_dead() || 
		defender->is_dead() || 
		base_name(attacker) == base_name(defender) || 
		!same_environment(attacker, defender) ||
		COMBAT_D->in_fight(attacker) ||
		!attacker->valid_enemy(defender) ||
		!defender->valid_enemy(attacker)
	) return;

	msg(HIR"$ME"HIR"突然衝了過來，看來是想要殺死$YOU"HIR"！！\n"NOR, attacker, defender, 1);
	COMBAT_D->start_fight(attacker, defender);
}

void autokill(object ob)
{
	int autokill;
	
	if( (autokill = query("autokill", ob)) )
	{
		object me = this_object();

		if( base_name(ob) == base_name(me) || COMBAT_D->in_fight(ob) || !me->is_living() || me->is_dead() || ob->is_dead() || ob->is_faint() || query("killlock")) return;
		
		if( autokill == AUTOKILL_DELAY )
		{
			call_out( (: msg :), 0, WHT"$ME"WHT"狠狠地瞪著$YOU"WHT"，空氣中瀰漫著肅殺之氣...\n"NOR, ob, me);
			call_out( (: autokill_start :), range_random(3, 6), ob, me);
		}
		else if( autokill == AUTOKILL_IMMEDIATELY )
		{
			call_out( (: autokill_start :), 0, ob, me);
		}
	}
	else if( (autokill = query("autokill")) )
	{
		object me = this_object();

		if( base_name(ob) == base_name(me) || COMBAT_D->in_fight(me) || !ob->is_living() || ob->is_dead() || me->is_dead() || me->is_faint() || query("killlock", ob)) return;

		if( autokill == AUTOKILL_DELAY )
		{
			call_out( (: msg :), 0, WHT"$ME"WHT"狠狠地瞪著$YOU"WHT"，空氣中瀰漫著肅殺之氣...\n"NOR, me, ob);
			call_out( (: autokill_start :), range_random(3, 6), me, ob);
		}
		else if( autokill == AUTOKILL_IMMEDIATELY )
		{
			call_out( (: autokill_start :), 0, me, ob);
		}
	}
}

// 是否死亡
nomask int is_dead()
{
	return query("die");
}

void add_follower(object ob)
{
	object* followers;
	
	if( !objectp(ob) ) return;

	followers = (query_temp("follower") || allocate(0)) | ({ ob });
	
	set_temp("follower", followers);
	set_temp("following", this_object(), ob);
}

varargs void remove_follower(object ob)
{
	object *followers = query_temp("follower") || allocate(0);
	
	if( undefinedp(ob) )
	{
		foreach(ob in followers)
		{
			if( objectp(ob) )
				delete_temp("following", ob);
		}
		
		delete_temp("follower");
	}
	else
	{
		followers -= ({ ob });
		
		if( !sizeof(followers) )
			delete_temp("follower");
		else
			set_temp("follower", followers);
			
		delete_temp("following", ob);
	}
}
