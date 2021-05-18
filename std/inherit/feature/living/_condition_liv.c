/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _condition_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : ���A
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <buff.h>
#include <condition.h>

// �߰ݪ��A
nomask int in_condition(string condition_file)
{
	return mapp(query("condition/"+replace_string(condition_file, "/","#")));
}

// �Ұʪ��A, 1 �����
varargs nomask void start_condition(string condition_file, int time, int heart_beat, object caster)
{
	int cur_stack;
	int max_stack = condition_file->stackable();
	mapping condition_setup;

	if( !max_stack )
	{
		condition_setup = allocate_mapping(0);

		// �Y�w�g�b�ۦP���A���h��������e���A
		if( in_condition(condition_file) )
			condition_file->stop_effect(this_object());
	}
	else
		condition_setup = query("condition/"+replace_string(condition_file, "/", "#"));

	if( !mapp(condition_setup) )
		condition_setup = allocate_mapping(0);

	if( objectp(caster) )
		condition_setup["caster"] = caster;

	// �]�w���A����ɶ��Ψϥιw�]�ɶ�
	if( !undefinedp(time) && time > 0 )
		condition_setup["time"] = time;
	else
		condition_setup["time"] = condition_file->query_default_condition_time();
		
	if( query("battlereward/conditiontime_bonus") > 0 && condition_file->query_condition_type() == CONDITION_TYPE_POSITIVE )
		condition_setup["time"] = to_int(condition_setup["time"] * (1.+query("battlereward/conditiontime_bonus")*0.1));
		
	if( in_condition(GODS_TRANSFORMER) && condition_file->query_condition_type() == CONDITION_TYPE_COUNTDOWN )
		condition_setup["time"] = to_int(condition_setup["time"] * 9 / 10);
		
	// �]�w���A�߸��ɶ��Ψϥιw�]�߸��ɶ�
	if( !undefinedp(heart_beat) && heart_beat > 0 )
		condition_setup["heart_beat"] = heart_beat;
	else
		condition_setup["heart_beat"] = condition_file->query_default_condition_heart_beat();

	cur_stack = condition_setup["stack"];
	
	if( cur_stack < max_stack )
		condition_setup["stack"]++;

	set("condition/"+replace_string(condition_file, "/","#"), condition_setup);	
	
	if( !max_stack || cur_stack < max_stack )
		condition_file->start_effect(this_object());
	else
		tell(this_object(), pnoun(2, this_object())+"�w�g��F�u"+condition_file->query_condition_name()+"�v���̤j�ĪG�|�[���ơA�L�k�A�W�[�F�C\n");
		
	this_object()->reset_buff_cache();
	
	if( this_object()->is_npc() )
		this_object()->startup_heartbeat();
}


// �������A
varargs nomask void stop_condition(string input)
{
	object me = this_object();

	if( !sizeof(query("condition")) )
		return;

	if( undefinedp(input) )
	{
		foreach(string condition, mixed data in query("condition"))
		{
			replace_string(condition, "#", "/")->stop_effect(me);
		}
			
		delete("condition");
	}
	else switch(input)
	{
		case CONDITION_TYPE_POSITIVE:
		case CONDITION_TYPE_NEGATIVE:
		case CONDITION_TYPE_SPECIAL:
		{
			string condition;
			foreach(string key, mixed data in query("condition"))
			{
				condition = replace_string(key, "#", "/");
				
				if( condition->query_condition_type() == input )
				{
					condition->stop_effect(me);
					delete("condition/"+key);
				}
			}
			break;
		}
		default:
		{
			string condition = input;
			string key = replace_string(condition, "/", "#");
			
			if( undefinedp(query("condition/"+key)) )
				return;
	
			delete("condition/"+key);
			
			// ���檬�A�����ɪ��ĪG
			condition->stop_effect(me);
			break;
		}
	}

	// �ˬd�O�_�z��
	if( me->query_stamina_cur() > me->query_stamina_max() )
		me->set_stamina_full();
	if( me->query_health_cur() > me->query_health_max() )
		me->set_health_full();
	if( me->query_energy_cur() > me->query_energy_max() )
		me->set_energy_full();
	if( me->query_food_cur() > me->query_food_max() )
		me->cost_food(me->query_food_cur() - me->query_food_max());
	if( me->query_drink_cur() > me->query_drink_max() )
		me->cost_drink(me->query_drink_cur() - me->query_drink_max());
		
	this_object()->reset_buff_cache();
}

// ���ܪ��A�ɶ�
nomask void change_condition_time(string condition_file, int time)
{
	if( !in_condition(condition_file) ) return;
		
	addn("condition/"+replace_string(condition_file, "/","#")+"/time", time);
}

// �^�Ǫ��A���
varargs nomask mapping query_condition(string condition_file)
{
	if( undefinedp(condition_file) )
		return query("condition");

	return query("condition/"+replace_string(condition_file, "/","#"));
}
