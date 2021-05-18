/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _attribution.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-12
 * Note   : �ݩ��~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <message.h>
#include <buff.h>

//
// �˳ƪ��� buff
//
int query_equipment_buff(string key)
{
	int buff = 0;
	int fullsuit = this_object()->is_full_suit();

	foreach(object ob in this_object()->query_equipment_objects())
	{
		buff += query(key, ob) + query_temp(key, ob);
		
		if( fullsuit )
			buff += query(BUFF_FULLSUIT+key, ob) + query_temp(BUFF_FULLSUIT+key, ob);
	}
	
	return buff;
}

//
// ���⪬�A�� buff
//
int query_condition_buff(string key)
{
	int buff = 0;
	mapping condition = query("condition");

	if( mapp(condition) )
	foreach(string c, mapping data in condition)
		buff += query("condition/"+c+"/"+key);
	
	return buff;
}

//
// �Ҧ��[���`��
//
int query_all_buff(string key)
{
	int value;
	
	if( undefinedp(value = query_temp("buff_cache/"+key)) )
	{
		value = query_equipment_buff(key) + query_condition_buff(key) + query_temp("buff/"+key) + query("battlereward/"+key);
		
		if( key == BUFF_COUNTERATTACK_CHANCE )
		{
			string stance = query("combat/stance/type");
			
			if( stance == "defend" )
			{
				value += this_object()->query_skill_level("defend-stance")/10;	
			}
			else if( stance == "defend-adv" )
			{
				value += 10 + this_object()->query_skill_level("defend-stance-adv")/10;
			}
		}
		
		set_temp("buff_cache/"+key, value);
	}
	
	
	
	return value;
}

// ���]�֨�
void reset_buff_cache()
{
	delete_temp("buff_cache");
}

//
// ���j�ݩ�
//
varargs int query_str(int without_buff)
{
	if( without_buff )
		return query("attr/str");

	return query("attr/str") + query_all_buff(BUFF_STR);
}

varargs int query_phy(int without_buff)
{
	if( without_buff )
		return query("attr/phy");

	return query("attr/phy") + query_all_buff(BUFF_PHY);
}

varargs int query_int(int without_buff)
{
	if( without_buff )
		return query("attr/int");

	return query("attr/int") + query_all_buff(BUFF_INT);
}

varargs int query_agi(int without_buff)
{
	if( without_buff )
		return query("attr/agi");

	return query("attr/agi") + query_all_buff(BUFF_AGI);
}

varargs int query_cha(int without_buff) 
{
	if( without_buff )
		return query("attr/cha");

	return query("attr/cha") + query_all_buff(BUFF_CHA);
}


//
// �T�j�ƭȳ̤j��
//
varargs int query_stamina_max(int without_buff)
{
	if( without_buff )
		return query("abi/stamina/max");

	return query("abi/stamina/max") + query_all_buff(BUFF_STAMINA_MAX);
}
varargs int query_health_max(int without_buff)
{
	if( without_buff )
		return query("abi/health/max");

	return query("abi/health/max") + query_all_buff(BUFF_HEALTH_MAX);
}
varargs int query_energy_max(int without_buff)
{
	if( without_buff )
		return query("abi/energy/max");

	return query("abi/energy/max") + query_all_buff(BUFF_ENERGY_MAX);
}


//
// �T�j�ƭȥثe��
//
int query_stamina_cur() { return query("abi/stamina/cur"); }
int query_health_cur() { return query("abi/health/cur"); }
int query_energy_cur() { return query("abi/energy/cur"); }


//
// �N�T�j�ƭȥثe�ȳ]�w���̤j��
//
int set_stamina_full() { return set("abi/stamina/cur", query_stamina_max()); }
int set_health_full() { return set("abi/health/cur", query_health_max()); }
int set_energy_full() { return set("abi/energy/cur", query_energy_max()); }
void set_all_ability_full()
{
	set_stamina_full();
	set_health_full();
	set_energy_full();
}

//
// �N�T�j�ƭȥثe�ȳ]�w���̤p��
//
int set_stamina_zero() { return set("abi/stamina/cur", 0); }
int set_health_zero() {	return set("abi/health/cur", 0); }
int set_energy_zero() {	return set("abi/energy/cur", 0); }
void set_all_ability_zero()
{
	set_stamina_zero();
	set_health_zero();
	set_energy_zero();
}

//
// �T�j�ƭȤ���_�t��
//
int stamina_regen()
{
	return query_str() + query_temp("rest_regen/stamina") + query_all_buff(BUFF_STAMINA_REGEN);
}
int health_regen()
{
	return query_phy() + query_temp("rest_regen/health") + query_all_buff(BUFF_HEALTH_REGEN);
}
int energy_regen()
{
	return query_int() + query_temp("rest_regen/energy") + query_all_buff(BUFF_ENERGY_REGEN);
}



//
// �ˬd�T�j�ƭȬO�_����
//
int is_stamina_full() { return query_stamina_cur() >= query_stamina_max(); }
int is_health_full() { return query_health_cur() >= query_health_max();	}
int is_energy_full() { return query_energy_cur() >= query_energy_max();	}
int is_all_ability_full()
{
	return is_stamina_full() && is_health_full() && is_energy_full();
}

//
// �ܰʤ��j�ݩ�
//
int add_str(int i) { return addn("attr/str", i); }
int add_phy(int i) { return addn("attr/phy", i); }
int add_int(int i) { return addn("attr/int", i); }
int add_agi(int i) { return addn("attr/agi", i); }
int add_cha(int i) { return addn("attr/cha", i); }


//
// ���j�ݩʥ~�[ buff
//
int add_temp_str(int i) { addn("attr/temp/str", i); }
int add_temp_phy(int i) { addn("attr/temp/phy", i); }
int add_temp_int(int i) { addn("attr/temp/int", i); }
int add_temp_agi(int i) { addn("attr/temp/agi", i); }
int add_temp_cha(int i) { addn("attr/temp/cha", i); }


//
// �������A
//
varargs int query_food_max(int without_buff)
{
	if( without_buff )
		return query("stat/food/max");

	return query("stat/food/max") + query_all_buff(BUFF_FOOD_MAX);
}

int query_food_cur()
{
	return query("stat/food/cur");
}

void cost_food(int i)
{
	if( addn("stat/food/cur", -i) < 0 )
		set("stat/food/cur", 0);
}

int add_food(int i)
{
	if( query_food_cur() + i > query_food_max() )
		return 0;

	if( !userp(this_object()) && !query_heart_beat(this_object()) )
		this_object()->startup_heartbeat();

	return addn("stat/food/cur", i);
}
void reset_food()
{
	set("stat/food/cur", 0);
}

//
// �������A
//

varargs int query_drink_max(int without_buff)
{
	if( without_buff )
		return query("stat/drink/max");

	return query("stat/drink/max") + query_all_buff(BUFF_DRINK_MAX);
}

int query_drink_cur()
{
	return query("stat/drink/cur");
}

int cost_drink(int i)
{
	if( addn("stat/drink/cur", -i) < 0 )
		set("stat/drink/cur", 0);
}

int add_drink(int i)
{
	if( query_drink_cur() + i > query_drink_max() )
		return 0;
	
	if( !userp(this_object()) && !query_heart_beat(this_object()) )
		this_object()->startup_heartbeat();

	return addn("stat/drink/cur", i);
}

void reset_drink()
{
	set("stat/drink/cur", 0);
}

// �̨Ϊ��p
int is_all_attribution_full()
{
	return is_stamina_full() && is_health_full() && is_energy_full() && !query_drink_cur() && !query_food_cur();
}


//
// ���|�g��B��
//
int query_social_exp_cur() { return to_int(query("exp/social/cur")); }
int query_social_exp_tot() { return to_int(query("exp/social/tot")); }
int add_social_exp(int exp)
{
	float fexp = to_float(exp);
	int cur = query_social_exp_cur();
	int tot = query_social_exp_tot();

	if( fexp <= 0 ) return 0;

	// ���U�˳ƪk�N�[��
	fexp += fexp*(query_int()*2-20+query_all_buff(BUFF_SOCIAL_EXP_BONUS))/100.;

	// �`�g��ȨC 5 ���� 1 ���g���
	fexp /= (tot/500000000)+1.;

	exp = ceil(fexp);

	set("exp/social/tot", exp + tot);

	return set("exp/social/cur", exp + cur);
}

int cost_social_exp(int exp)
{
	int newexp;
	int oldexp = to_int(query("exp/social/cur"));
	
	if( exp <= 0 ) return 0;
	
	newexp = oldexp - exp;
	
	if( newexp < 0 )
		return 0;
	
	set("exp/social/cur", newexp);
	
	return 1;
}

//
// �԰��g��B��
//
int query_combat_exp_cur() { return to_int(query("exp/combat/cur")); }
int query_combat_exp_tot() { return to_int(query("exp/combat/tot")); }
int add_combat_exp(int exp)
{
	float fexp = to_float(exp);
	int cur = query_combat_exp_cur();
	int tot = query_combat_exp_tot();

	if( fexp <= 0 ) return 0;

	// ���U�˳ƪk�N�[��
	fexp += fexp*(query_int()*2-20+query_all_buff(BUFF_COMBAT_EXP_BONUS))/100.;

	// �`�g��ȨC 5 ���� 1 ���g���
	fexp /= (tot/500000000)+1.;

	exp = ceil(fexp);

	set("exp/combat/tot", exp + tot);

	return set("exp/combat/cur", exp + cur);
}

int cost_combat_exp(int exp)
{
	int newexp;
	int oldexp = to_int(query("exp/combat/cur"));
	
	if( exp <= 0 ) return 0;
	
	newexp = oldexp - exp;
	
	if( newexp < 0 )
		return 0;
	
	set("exp/combat/cur", newexp);
	
	return 1;
}
//
// ��O�B��
//
int cost_stamina(int str)
{
	int curstr = query_stamina_cur();
	
	if( curstr < str ) return 0;
	
	addn("abi/stamina/cur", -str);

	if( !userp(this_object()) && !query_heart_beat(this_object()) )
		this_object()->startup_heartbeat();

	return 1;	
}

void earn_stamina(int str)
{
	int curstr = query_stamina_cur();
	int maxstr = query_stamina_max();
	
	if( curstr == maxstr ) return;
	
	if( curstr + str >= maxstr )
		set_stamina_full();
	else
		addn("abi/stamina/cur", str);
}


//
// �ͩR�B��
//
varargs int cost_health(int hp, int kill)
{
	int curhp = query_health_cur();

	if( !userp(this_object()) && !query_heart_beat(this_object()) )
		this_object()->startup_heartbeat();

	if( curhp <= hp )
	{
		if( kill )
			this_object()->die();
		else
			this_object()->faint();

		return 0;
	}

	addn("abi/health/cur", -hp);

	return 1;
}
void earn_health(int hp)
{
	int curhp = query_health_cur();
	int maxhp = query_health_max();
	
	if( curhp == maxhp ) return;
	
	if( curhp + hp >= maxhp )
		set_health_full();
	else
		addn("abi/health/cur", hp);
}



//
// �믫�B��
//
int cost_energy(int ep)
{
	int curep = query_energy_cur();
	
	if( curep < ep ) return 0;
	
	addn("abi/energy/cur", -ep);

	if( !userp(this_object()) && !query_heart_beat(this_object()) )
		this_object()->startup_heartbeat();

	return 1;	
}

void earn_energy(int ep)
{
	int curep = query_energy_cur();
	int maxep = query_energy_max();
	
	if( curep == maxep ) return;
	
	if( curep + ep >= maxep )
		set_energy_full();
	else
		addn("abi/energy/cur", ep);
}

//
// �h�ҹB��
//
int add_fatigue(int fatigue)
{
	int curfatigue = query("fatigue");
	int result = curfatigue + fatigue;

	if( curfatigue == 1000 )
		return curfatigue;

	if( result > 1000 )
		result = 1000;

	tell(this_object(), pnoun(2, this_object())+"�Pı�쨭�骺�h�ҫ׼W�[�F("+HIY+"+"+fatigue+"�B"NOR YEL+result+"/1000"NOR")�C\n");

	set("fatigue", result);
	
	return result;
}
int reset_fatigue()
{
	set("fatigue", 0);

	return 1;
}
int query_fatigue()
{
	return query("fatigue");
}
int is_fatigue_full()
{
	return query("fatigue") == 1000;
}

// ��o�n��
int earn_prestige(int prestige)
{
	int result = addn("prestige", prestige);

	tell(this_object(), NOR YEL+pnoun(2, this_object())+"��o "HIY+prestige+NOR YEL" �I�n��C\n"NOR);
	
	return result;	
}

int cost_prestige(int prestige)
{
	int curprestige = query("prestige");
	
	if( curprestige < prestige )
		return 0;
	
	addn("prestige", -prestige);

	return 1;
}

// �P���ͪ��԰��ɥi��o���g���
int query_fighting_exp()
{
	int exp = 0;
	
	exp += query_str();
	exp += query_phy();
	exp += query_int();
	exp += query_agi();
	exp += query_cha();
	
	exp += query_stamina_max()/10;
	exp += query_health_max()/10;
	exp += query_energy_max()/10;
	
	return exp / 10;
}
