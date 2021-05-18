/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : combat_d_main.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : �԰����F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <equipment.h>
#include <buff.h>
#include <location.h>
#include <condition.h>
#include <npc.h>
#include <stuck.h>
#include <combat.h>

mapping combat;

#define MODULE_NPC_POWER		0.7
#define COMBAT_TICK_BASE		160

varargs void attack(object attacker, object defender, int effect);

//
// �O�_���b�԰�
//
varargs int in_fight(object me, object enemy)
{
	if( !sizeof(combat[me]) )
		return 0;

	if( undefinedp(enemy) )
	{
		if( sizeof(combat[me]) )
			return 1;
		else
			return 0;
	}
	else
		return member_array(enemy, combat[me]) != -1;
}

//
// �^�ǥ��b�԰�����H
//
object *query_fighting_targets(object me)
{
	if( !sizeof(combat[me]) )
		return allocate(0);
		
	return filter_array(combat[me], (: objectp($1) :));
}

//
// �����O�p��
//
varargs int attack_value(object me, int without_buff)
{
	string stance_type = query("combat/stance/type", me);
	float attack;
	object weapon;
	object* weapons = me->query_equipping_object(EQ_HAND[PART_ID]) || me->query_equipping_object(EQ_TWOHAND[PART_ID]) || ({ me });
	
	attack = to_float(me->query_str(without_buff));

	foreach(weapon in weapons)
	{
		if( weapon == me )
			attack += me->query_skill_level("unarmed")/2.;
		else
			attack += me->query_skill_level(query("skill", weapon))/2;
	}
	
	if( !without_buff )
		attack += me->query_all_buff(ATTACK);

	if( stance_type == "attack" )
		attack *= 1. + me->query_skill_level("attack-stance")/500.;
	else if( stance_type == "attack-adv" )
		attack *= 1.2 + me->query_skill_level("attack-stance-adv")/500.;

	if( me->is_module_npc() )
		attack *= MODULE_NPC_POWER;

	return to_int(attack);
}

//
// ���m�O�p��
//
varargs int defend_value(object me, int without_buff)
{
	string stance_type = query("combat/stance/type", me);
	float defend;

	defend = to_float(me->query_phy(without_buff));
	
	if( !without_buff )
		defend += me->query_all_buff(DEFEND);

	defend += me->query_skill_level("dodge")/2.;

	if( stance_type == "defend" )
		defend *= 1. + me->query_skill_level("defend-stance")/500.;
	else if( stance_type == "defend-adv" )
		defend *= 1.2 + me->query_skill_level("defend-stance-adv")/500.;

	if( me->is_module_npc() )
		defend *= MODULE_NPC_POWER;

	return to_int(defend);
}

//
// ��t�p��
//
varargs int speed_value(object me, int without_buff)
{
	string stance_type = query("combat/stance/type", me);
	float speed;

	speed = to_float(me->query_agi(without_buff)+10);

	if( !without_buff )
		speed += me->query_all_buff(SPEED);

	if( stance_type == "speed" )
		speed *= 1. + me->query_skill_level("speed-stance")/500.;
	else if( stance_type == "speed-adv" )
		speed *= 1.2 + me->query_skill_level("speed-stance-adv")/500.;

	return to_int(speed);	
}

int total_value(object me)
{
	return attack_value(me) + defend_value(me) + speed_value(me);
}

int allow_escape(object me)
{
	if( undefinedp(combat[me]) )
		return 1;

	foreach(object enemy in combat[me])
	{
		if( !objectp(enemy) || !same_environment(enemy, me) ) continue;
			
		// 1/3 �����v�L�k�k��, �ӥB�|�y���ۤv���w�t
		if( !random(3) )
		{
			me->start_stuck(COMBAT_STUCK_KEY, 3, pnoun(2, me)+"�յ۰k���԰��o���ѡA�Ϧ����ۤv�Q�x��");
			return 0;
		}
	}
	
	return 1;
}

//
// �}�l�԰�
//
void start_fight(object me, object enemy)
{
	if( !objectp(me) || !objectp(enemy) ) return;

	if( undefinedp(combat[me]) )
		combat[me] = allocate(0);

	if( undefinedp(combat[enemy]) )
		combat[enemy] = allocate(0);

	// �D�ʭn�D�԰�(�N���ǩԦܲĤ@��)
	combat[me] = ({ enemy }) | combat[me];

	// �Q�ʥ[�J�԰�
	combat[enemy] |= ({ me });

	if( enemy->is_boss() )
		set_temp("status/"HIR"��"NOR RED"��"NOR, 999999, me);
	else
		set_temp("status/"HIR"��"NOR RED"��"NOR, 999999, me);
	
	if( me->is_boss() )
		set_temp("status/"HIR"��"NOR RED"��"NOR, 999999, enemy);
	else
		set_temp("status/"HIR"��"NOR RED"��"NOR, 999999, enemy);
		
	set_temp("attack/attackvalue", attack_value(me), me);
	set_temp("attack/defendvalue", defend_value(me), me);

	set_temp("attack/attackvalue", attack_value(enemy), enemy);
	set_temp("attack/defendvalue", defend_value(enemy), enemy);
	
	set_heart_beat(1);
}

//
// ����԰�
//
varargs void stop_fight(object me, object enemy)
{
	if( !objectp(me) )
	{
		map_delete(combat, me);
		return;
	}

	if( undefinedp(enemy) )
	{
		if( arrayp(combat[me]) )
		{
			combat[me] -= ({ 0 });

			if( !sizeof(combat[me]) )
			{
				map_delete(combat, me);
				delete_temp("attack", me);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, me);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, me);
			}
			else
			foreach(enemy in combat[me])
				stop_fight(me, enemy);
		}
		else map_delete(combat, me);
	}
	else
	{
		if( arrayp(combat[me]) )
		{
			combat[me] -= ({ enemy, 0 });
			
			if( !sizeof(combat[me]) )
			{
				map_delete(combat, me);
				delete_temp("attack", me);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, me);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, me);

			}
		}
		else map_delete(combat, me);
		
		if( arrayp(combat[enemy]) )
		{
			combat[enemy] -= ({ me, 0 });

			if( !sizeof(combat[enemy]) )
			{
				map_delete(combat, enemy);
				delete_temp("attack", enemy);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, enemy);
				delete_temp("status/"HIR"��"NOR RED"��"NOR, enemy);
			}
		}
		else map_delete(combat, enemy);
	}
}

varargs string damage_message(object attacker, object defender, int damage, int effect, int fatalblow_power)
{
	int cur = defender->query_health_cur() - damage;
	string* effect_msgs = allocate(0);
	string special_msg = "";
	
	if( cur < 0 ) cur = 0;

	if( effect & COUNTERATTACK )
		effect_msgs += ({ HIM"��"NOR MAG"��"NOR });
	
	if( effect & COMBOATTACK )
		effect_msgs += ({ HIG"�s"NOR GRN"��"NOR });
	
	if( effect & THORNDAMAGE )
		effect_msgs += ({ HIB"��"NOR BLU"��"NOR });
		
	if( effect & FATALBLOW )
		effect_msgs += ({ HIY"��"NOR YEL"��"HIY+fatalblow_power+NOR YEL"%"NOR });
					
	if( sizeof(effect_msgs) > 0 )
		special_msg = NOR WHT"<" + implode(effect_msgs, " ") +NOR WHT">"NOR;

	switch(cur * 10 / defender->query_health_max())
	{
		case -MAX_INT..0:
			return "$YOU������n�n���Y[$COLOR"RED"-"+damage+NOR"]..."+special_msg+"\n";
			break;
		case 1..2:
			return "$YOU���ʧ@�w�M���õL��[$COLOR"HIR"-"+damage+NOR"]..."+special_msg+"\n";
			break;
		case 3..4:
			return "$YOU�������o�Q���j�j[$COLOR"YEL"-"+damage+NOR"]..."+special_msg+"\n";
			break;
		case 5..6:
			return "$YOU�y�S�h�W����[$COLOR"HIY"-"+damage+NOR"]..."+special_msg+"\n";
			break;
		case 7..8:
			return "��$YOU�y�����p���ˮ`[$COLOR"GRN"-"+damage+NOR"]..."+special_msg+"\n";
			break;
		case 9..MAX_INT:
			return "��$YOU���F�@�I����[$COLOR"HIG"-"+damage+NOR"]..."+special_msg+"\n";
			break;
	}
}

//
// ���ѼĤH
//
int defeat(object attacker, object defender)
{
	int is_boss = defender->is_boss();
	int prestige = query("prestige", defender);
	int defender_power = total_value(defender);
	int attacker_power;
	int leader_power;
	int temp;
	float total_exp = 10. * defender_power;
	float exp;
	int attacker_prev_exp;
	int attacker_new_exp;
	object member;
	object* winners = (copy(combat[defender]) || allocate(0)) | ({ attacker });
	object leader;
	int soldiers;
	int players;
	string player_id = "";
	string city;
	array loc;
	string ob_idname;

	// �������
	stop_fight(defender);

	winners -= ({ 0 });
	defender->reply_defeat(winners);

	foreach(object winner in winners)
	{
		temp = total_value(winner);
		
		if( temp > leader_power )
		{
			leader_power = temp;
			leader = winner;
		}
	}

	if( leader->is_module_npc() )
	{
		string boss = query("boss", leader);
		
		if( stringp(boss) && objectp(find_player(boss)) )
			leader = find_player(boss);
	}

	city = query("city", leader);
	loc = query_temp("location", leader);
	
	if( defender->is_user_ob() )
	{
		addn("combat/kill/winplayer", 1, attacker);
		
		if( BATTLEFIELD_D->in_battle(attacker) )
		{
			string attacker_city = query("city", attacker);
			string defender_city = query("city", defender);
			
			if( !attacker_city || attacker_city != defender_city )
			{
				BATTLEFIELD_D->add_killplayer_score(attacker, 1);
				BATTLEFIELD_D->add_dead_score(defender, 1);
			}
			
			if( defender->in_condition(BATTLEFIELD_GLORY) && (!attacker_city || attacker_city != defender_city) )
			{
				defender->stop_condition(BATTLEFIELD_GLORY);
				BATTLEFIELD_D->add_bonus_score(attacker, 80);
			}
			
			BATTLEFIELD_D->add_bonus_buff(defender, attacker->is_user_ob());
		}
	}
	else
	{
		addn("combat/kill/win", 1, attacker);
		if( BATTLEFIELD_D->in_battle(attacker) )
			BATTLEFIELD_D->add_killnpc_score(attacker, 1);
	}
	
	addn("combat/kill/lose", 1, defender);

	foreach(member in winners)
	{
		if( member->is_module_npc() )
			soldiers++;
		else if( member->is_user_ob() )
		{
			player_id += capitalize(member->query_id())+"�B";
			players++;
		}
	}
	
	if( defender->is_user_ob() )
	{
		if( BATTLEFIELD_D->in_battle(defender) || BATTLEFIELD_D->in_battle(attacker) )
			CHANNEL_D->channel_broadcast("combat", replace_string(defender->query_idname(), "������(Corpse of ", "(")+"�Q"+replace_string(attacker->query_idname(), "������(Corpse of ", "(")+"����"+(loc_short(query_temp("location", attacker))||"")+"�C");
		else
			CHANNEL_D->channel_broadcast("combat", replace_string(defender->query_idname(), "������(Corpse of ", "(")+"�Q"+replace_string(attacker->short(), "������(Corpse of ", "(")+"�����C");
	}

	if( defender->is_boss() && leader->is_user_ob() )
	{
		leader->add_fatigue(range_random(5, 30));
		//CHANNEL_D->channel_broadcast("sys", defender->query_idname()+"�Q "+(player_id==""?"":player_id[0..<3]+" �� "+players+" �쪱�a�P ")+soldiers+" ��x�H�����F�C");		
	}
	
	// �ۨ��j�׶W�L�Ǫ��� 4 ���N�L�k���o����g����y
	if( total_value(leader) <= defender_power*4 && (leader->is_user_ob() || leader->is_module_npc()) )
	{
		// �Y���a���B���ϡA�B�z�귽����p��
		if( leader->is_user_ob() && environment(leader)->is_maproom() && arrayp(loc) && AREA_D->area_exist(loc[AREA], loc[NUM]) && CITY_D->city_exist(city) && city != "wizard" && !defender->is_boss())
		{
			int point = defender_power/10;
	
			mapping area_section_influence = AREA_D->query_section_info(loc[AREA], loc[NUM], "area_section_influence") || allocate_mapping(0);
	
			area_section_influence[city] += point;
			
			AREA_D->set_section_info(loc[AREA], loc[NUM], "area_section_influence", area_section_influence);
			
			addn("area_influence", point, leader);
			tell(leader, CITY_D->query_city_idname(city)+HIG"��o "+NUMBER_D->number_symbol(point)+" �I�귽�����I�ơC\n"NOR);
		}

		// �g��ȭp��
		total_exp = to_float(range_random(to_int(total_exp*0.9), to_int(total_exp*1.1)));
	
		if( BATTLEFIELD_D->in_battle(attacker) )
			total_exp *= 2;

		// ��ު̷U�h�A�g��ȷU��
		if( sizeof(winners) > 1 )
		{
			float rate = pow(to_float(sizeof(winners)), 0.2);
			
			if( rate < 1.0 )
				rate = 1.0;
				
			total_exp /= rate;
		}
		
		if( total_exp < 1 )
			total_exp = 1.;
	
		foreach(member in winners)
		{
			attacker_power = total_value(member);
	
			// �g����H�����H�ƥ������t
			exp = total_exp / sizeof(winners);
	
			// �j�L�ĤH�U�h�A�g��ȷU��
			if( attacker_power > defender_power )
				exp /= to_float(attacker_power) / to_float(defender_power);
			
			if( exp < 1. )
				exp = 1.;
	
			attacker_prev_exp = member->query_combat_exp_cur();
			attacker_new_exp = member->add_combat_exp(to_int(exp));
	
			if( member->is_user_ob() )
			{
				tell(member, HIY+pnoun(2, member)+"��o "+NUMBER_D->number_symbol(attacker_new_exp - attacker_prev_exp)+" �I�԰��g��ȡC\n"NOR);
	
				if( is_boss && prestige && !random(players) && !random(10) )
					member->earn_prestige(random(prestige)+1);
			}
		}
	}

	// �۰ʾ߬B����
	if( query("auto/loot", leader) && !defender->is_user_ob() && !defender->is_module_npc() )
	{
		int special;
		
		if( !sizeof(all_inventory(defender)) )
			tell(leader, defender->query_idname()+"�S�����󪫫~�i�H�߭C...�C\n");
		else
		{
			foreach(object ob in all_inventory(defender))
			{
				if( ob->is_money() )
				{
					msg("$ME�q$YOU���W�߰_�F "HIY+ob->query_idname()+NOR"�C\n", leader, defender, 1);
					ob->move(leader);
				}
				else
				{
					special = query("special", ob);
					ob_idname = ob->query_idname();
	
					if( leader->available_get(ob) )
					{
						ob->move(leader);
		
						if( special )
							CHANNEL_D->channel_broadcast("combat", leader->query_idname()+"�q"+defender->query_idname()+"���W�߰_�F"+ob_idname+"�C");
		
						log_file("command/loot", leader->query_idname()+"�q"+defender->query_idname()+"���W�߰_�F"+ob_idname+"�C", -1);
		
						msg("$ME�q$YOU���W�߰_�F"+ob_idname+"�C\n", leader, defender, 1);				
					}
					else
						tell(leader, pnoun(2, leader)+"�L�k�A�߰_"+ob_idname+"�F�C\n");
				}
			}
			
			if( defender->is_boss() )
				leader->save();
		}

		if( !sizeof(all_inventory(defender)) )
			defender->corpse_disappear();
	}
}

//
// �y���ˮ`
//
varargs void cause_damage(object attacker, object defender, int damage, int effect)
{
	string attacker_stance;
	string defender_stance;
	int stance_level;
	int fatalblow_power;
	string msg;

	if( !objectp(attacker) || !objectp(defender) || !defender->is_living() || defender->is_dead() || defender->in_condition(INVINCIBILITY) || !defender->valid_enemy(attacker) ) return;
		
	// �ˮ`�� = ���� * (�����O/�`�X)
	//damage = to_int(damage * damage / (damage + to_float(query_temp("attack/defendvalue", defender)) + 1.));
	if( !(effect & NO_ATTACKDEFEND) )
		damage = to_int(damage * pow( to_float(query_temp("attack/attackvalue", attacker)+50) / to_float(query_temp("attack/defendvalue", defender)+250), 0.4));

	// �üƶˮ`�d�� 0.5 ~ 1.5 ������
	if( !random(200) )
		damage = range_random(to_int(damage*0.5), to_int(damage * 1.5));
	else if( !random(50) )
		damage = range_random(to_int(damage*0.7), to_int(damage * 1.3));
	else
		damage = range_random(to_int(damage*0.8), to_int(damage * 1.2));
	
	attacker_stance = query("combat/stance/type", attacker);
	defender_stance = query("combat/stance/type", defender);
	
	if( stringp(attacker_stance) && stringp(defender_stance) )
	switch(attacker_stance)
	{
		case "attack":
		case "attack-adv":
			switch(defender_stance)
			{
				case "speed":
				case "speed-adv": 
					stance_level = attacker->query_skill_level("attack-stance") + attacker->query_skill_level("attack-stance-adv");
					damage *= 1 + stance_level/1000.;
					break;
				case "defend":
				case "defend-adv":	
					stance_level = defender->query_skill_level("defend-stance") + defender->query_skill_level("defend-stance-adv");
					damage *= 1 - stance_level/1000.;
					break;
			}
			break;
		case "defend":
		case "defend-adv":
			switch(defender_stance)
			{
				case "attack":
				case "attack-adv": 
					stance_level = attacker->query_skill_level("defend-stance") + attacker->query_skill_level("defend-stance-adv");
					damage *= 1 + stance_level/1000.;
					break;
				case "speed":
				case "speed-adv":
					stance_level = defender->query_skill_level("speed-stance") + defender->query_skill_level("speed-stance-adv");
					damage *= 1 - stance_level/1000.;
					break;
			}
			break;
		case "speed":
		case "speed-adv":
			switch(defender_stance)
			{
				case "defend":
				case "defend-adv": 
					stance_level = attacker->query_skill_level("speed-stance") + attacker->query_skill_level("speed-stance-adv");
					damage *= 1 + stance_level/1000.;
					break;
				case "attack":
				case "attack-adv":
					stance_level = defender->query_skill_level("attack-stance") + defender->query_skill_level("attack-stance-adv");
					damage *= 1 - stance_level/1000.;
					break;
			}
			break;
	}
	
	if( damage <= 0 ) return;

	if( !(effect & NO_FATALBLOW) )
	{
		// �P�R�@��
		if( ((effect & COUNTERATTACK > 0) * 5 + (attacker->query_skill_level("fatalblow")/2) + attacker->query_all_buff(BUFF_FATAL_CHANCE) > random(100) + defender->query_all_buff(BUFF_ANTIFATAL_CHANCE)) || attacker->in_condition(DEADLY_CONTRACT) )
			effect |= FATALBLOW;
						
		if( effect & FATALBLOW )
		{
			fatalblow_power = range_random(200, range_random(300, range_random(350, 400))) + attacker->query_all_buff(BUFF_FATAL_POWER);
			damage *= fatalblow_power / 100.;
		}
	}

	// �p�⤳���
	defender->calculate_combat_hate(attacker, damage);
	
	if( !random(400) )
		attacker->add_fatigue(1);
	if( !random(400) )
		defender->add_fatigue(1);

	if( BATTLEFIELD_D->in_battle(attacker) )
		BATTLEFIELD_D->add_damage_score(attacker, damage);

	if( attacker->is_user_ob() && !wizardp(attacker) )
	{
		if( damage > query("combat/damage/battle_max", attacker) && BATTLEFIELD_D->in_battle(attacker) )
		{
			set("combat/damage/battle_max", damage, attacker);
			TOP_D->update_top("battle_max_damage", attacker->query_id(1), damage, attacker->query_idname(), query("city", attacker));
			
			if( damage > 1000 )
				CHANNEL_D->channel_broadcast("combat", attacker->query_idname()+"�b�Գ��԰������X�ӤH���s���ˮ`�� "HIY+damage+NOR"�I�I");
		}

		if( damage > query("combat/damage/combat_max", attacker) && !BATTLEFIELD_D->in_battle(attacker) )
		{
			set("combat/damage/combat_max", damage, attacker);
			TOP_D->update_top("combat_max_damage", attacker->query_id(1), damage, attacker->query_idname(), query("city", attacker));
			
			if( damage > 1000 )
				CHANNEL_D->channel_broadcast("combat", attacker->query_idname()+"�b�@��԰������X�ӤH���s���ˮ`�� "HIY+damage+NOR"�I�I");
		}
	}

	msg = damage_message(attacker, defender, damage, effect, fatalblow_power);

	if( defender != attacker )
	{
		msg(replace_string(msg, "$COLOR", ""), attacker, defender, 1, 0, ({ defender }));
		msg(replace_string(msg, "$COLOR", HIREV), attacker, defender, 0, 0, ({ attacker }));
	}
	else
	{
		msg = replace_string(msg, "$YOU", "$ME");
		msg(replace_string(msg, "$COLOR", HIREV), attacker, 0, 1);
	}
	
	// ���ѼĤH
	if( !defender->cost_health(damage, 1) )
	{
		defeat(attacker, defender);
		return;
	}
	else if( same_environment(attacker, defender) )
	{	
		int defender_thorn_chance = defender->query_all_buff(BUFF_THORN_CHANCE);
		
		if( defender_thorn_chance > random(100) )
		{
			damage /= 2;

			msg = "$YOU�Q$ME�˳ƤW�y�U����Ƥϼu�ˮ`�A"+damage_message(defender, attacker, damage, THORNDAMAGE);
			
			if( defender != attacker )
			{
				msg(replace_string(msg, "$COLOR", ""), defender, attacker, 1, 0, ({ attacker }));
				msg(replace_string(msg, "$COLOR", HIREV), defender, attacker, 0, 0, ({ defender }));
			}
			else
			{
				msg = replace_string(msg, "$YOU", "$ME");
				msg(replace_string(msg, "$COLOR", HIREV), defender, 0, 1);
			}
	
			// ���ѼĤH
			if( !attacker->cost_health(damage, 1) )
			{
				defeat(defender, attacker);
				return;
			}
		}
		
		if( attacker->query_all_buff(BUFF_COMBO_CHANCE) > random(100) )
		{
			attack(attacker, defender, COMBOATTACK);
			
			if( !objectp(defender) || defender->is_dead() )
				return;
		}
	}

	//if( !in_fight(attacker, defender) && attacker != defender )
	//	start_fight(attacker, defender);
}

//
// �i������ʧ@
//
varargs void attack(object attacker, object defender, int effect)
{
	string msg;
	int damage = 0;			// �̫�ˮ`��
	object weapon, *weapons;
	int is_twohand_weapon;
	int attacker_agi = attacker->query_agi();

	object *protectors = query_temp("protectors", defender);

	if( arrayp(protectors) )
	{
		object new_defender;
		
		protectors -= ({ 0 });

		foreach(object protector in protectors)
		{
			if( !objectp(protector) || protector->is_dead() )
			{
				protectors -= ({ protector });
				continue;
			}
			else if( !same_environment(defender, protector) )
			{
				continue;
			}
			else if( !random(3) )
			{
				new_defender = protector;
				break;
			}
		}
		
		if( sizeof(protectors) )
			set_temp("protectors", protectors, defender);
		else
			delete_temp("protectors", defender);
			
		if( objectp(new_defender) )
			defender = new_defender;
	}

	if( attacker->is_dead() )
	{
		stop_fight(attacker);
		return;
	}

	// ����
	if( !(effect & COUNTERATTACK) && defender->query_all_buff(BUFF_COUNTERATTACK_CHANCE) > random(100) )
	{
		attack(defender, attacker, COUNTERATTACK);
		return;
	}
	
	// �ƭ�
	if( attacker->query_all_buff(BUFF_SLIP_CHANCE) > random(200) )
	{
		msg("$ME�}���@�}"HIG"��"NOR GRN"��"NOR"�A�|�}�¤�"HIG"��"NOR GRN"��"NOR"�b�a�I�I\n", attacker, 0, 1);
		return;
	}
	
	weapons = attacker->query_equipping_object(EQ_HAND[PART_ID]);

	if( !sizeof(weapons) )
	{
		weapons = attacker->query_equipping_object(EQ_TWOHAND[PART_ID]);
		
		if( !sizeof(weapons) )
		{
			if( attacker->is_person() )
			{
				weapons = ({ attacker, attacker });
				msg = "$ME���U�ϥX�@�ۮ{��氫�ޯ੹$YOU���������A";
			}
			else
			{
				weapons = ({ attacker });
				msg = attacker->combat_attack_message() || "$ME��$YOU�i������A";
			}
		}
		else
		{
			is_twohand_weapon = 1;
			msg = weapons[0]->combat_attack_message() || "$ME���ⴣ�_"+weapons[0]->query_name()+"��$YOU�i������A";
		}
	}
	else if( sizeof(weapons) == 1 )
	{
		if( attacker->is_person() )
		{
			weapons += ({ attacker });
			msg = "$ME���U�ϵ�"+weapons[0]->query_name()+"�ή{��氫�ۼƩ�$YOU�i������A";
		}
		else
		{
			msg = "$ME�D��ϵ�"+weapons[0]->query_name()+"��$YOU�i������A";
		}
	}
	else
	{
		if( base_name(weapons[0]) == base_name(weapons[1]) )
			msg = "$ME���U�ϵ�"+weapons[0]->query_name()+"��$YOU�i������A";
		else
			msg = "$ME���U�ϵ�"+weapons[0]->query_name()+"�B"+weapons[1]->query_name()+"��$YOU�i������A";
	}

	if( !defender->in_condition(INVINCIBILITY) )
	foreach(weapon in weapons)
	{
		// �����R��
		if( random(attacker_agi + 200) >= random(300) || defender->is_faint() )
		{
			damage += to_int(pow(to_float(query_temp("attack/attackvalue", attacker)), 0.95)) / 2;

			// �����̪������S�����
			attacker->special_attack(attacker, defender);
	
			if( !objectp(attacker) || !objectp(defender) || defender->is_dead() ) return;
		
			// �Z���P�ޯ઺�����O�v�T
			if( weapon == attacker )
			{			
				if( attacker->is_user_ob() || attacker->is_module_npc() )
					attacker->add_skill_exp("unarmed", range_random(50, 150));
			}
			else
			{
				// �Z�����S�����
				weapon->special_attack(attacker, defender);
				
				if( !objectp(defender) || defender->is_dead() ) return;
			
				if( attacker->is_user_ob() || attacker->is_module_npc() )
					attacker->add_skill_exp(query("skill", weapon), range_random(50, 150) * (is_twohand_weapon ? 2 : 1));
			}
		}
		// �{��
		else
		{
			if( defender->is_user_ob() || defender->is_module_npc() )
				defender->add_skill_exp("dodge", range_random(50, 150));			
		}
	}
	
	if( damage <= 0 )
	{
		if( sizeof(weapons) == 2 )
			msg += "���ҳQ�{�}�C\n";
		else
			msg += "���Q�{�}�C\n";
			
		msg(msg, attacker, defender, 1, "dodge_msg");

		return;
	}
	
	msg(msg, attacker, defender, 1);

	cause_damage(attacker, defender, damage, effect);
}

void heart_beat()
{
	int speed, rand;
	object enemy;
	object *stop_fight = allocate(0);
	int has_undefined_ob = 0;
	
	// �R������������
	map_delete(combat, 0);
	
	foreach(object me in sort_array(keys(combat), (: random(2) ? 1 : -1 :)))
	{
		if( !objectp(me) || !arrayp(combat[me]) )
		{
			stop_fight |= ({ me });
			has_undefined_ob = 1;
			continue;
		}

		speed = pow(to_float(speed_value(me)), 0.9) + 40;

		combat[me] -= ({ 0 });

		foreach(enemy in combat[me])
			if( !same_environment(me, enemy) )
				stop_fight(me, enemy);

		do
		{
			// �ĤH�q�q�����F
			if( !sizeof(combat[me]) )
			{
				stop_fight |= ({ me });
				break;
			}
			
			if( me->is_delaying() )
				break;

			rand = random(COMBAT_TICK_BASE);

			// �z�L speed �ƭȭp������t��
			if( speed > rand )
			{
				// �Y���B��L�Ī��p�h��ӧ����U�@��ĤH
				if( combat[me][0]->in_condition(INVINCIBILITY) )
					combat[me] = combat[me][1..] + ({ combat[me][0] });
				
				attack(me, combat[me][0], 0);
			}

		} while((speed -= COMBAT_TICK_BASE) > 0);
	}

	foreach(object me in stop_fight)
		stop_fight(me);
		
	if( has_undefined_ob )
	{
		mapping old_combat = combat;
		
		combat = allocate_mapping(0);
		
		foreach(object ob, mapping data in old_combat)
		{
			if( objectp(ob) )
				combat[ob] = data;
		}
	}	
}

mapping query_combat()
{
	return combat;
}

void create()
{
	combat = allocate_mapping(0);
	
	set_heart_beat(1);
}

int remove()
{
	foreach(object ob, mapping data in combat)
	{
		stop_fight(ob);
		tell(ob, "�԰��t�Χ�s�A����԰��C\n");
	}
}

string query_name()
{
	return "�԰��t�ΥD��(COMBAT_D_MAIN)";
}
