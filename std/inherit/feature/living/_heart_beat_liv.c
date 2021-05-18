/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _heart_beat_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-26
 * Note   : �ͪ��߸�
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <function.h>
#include <condition.h>

private nosave int heartbeat_tick = random(2048);

private void delay_decay();
private void stuck_decay();
private void process_heartbeat_command_decay();

// Status �ܤ�
private nomask void process_heartbeat_status()
{
	mapping status = query_temp("status");

	if( !mapp(status) || !sizeof(status) ) return;

	foreach(string key, int time in status)
	{
		if( time == -1 ) continue;

		if( --time <= 0 )
			map_delete(status, key);
		else
			status[key] = time;
	}

	if( !sizeof(status) )
		delete_temp("status");
	else
		set_temp("status", status);
}

// Condition �ܤ�
private nomask void process_heartbeat_condition()
{
	mapping condition;

	if( mapp(condition = copy(query("condition"))) )
	{	
		foreach( string key, mapping value in condition )
		{
			if( value["time"] > 0 )
			{
				if( value["heart_beat"] > 0 && !(value["time"] % value["heart_beat"]) )
					catch(replace_string(key, "#", "/")->heart_beat_effect(this_object()));
	
				addn("condition/"+key+"/time", -1);
			}
			else
			{
				catch(replace_string(key, "#","/")->stop_effect(this_object()));
				delete("condition/"+key);
				this_object()->reset_buff_cache();
			}
		}
		
		if( !sizeof(query("condition")) )
			delete("condition");
	}	
}

// �C 1 ��I�s�@��
nomask void heart_beat()
{
	object me = this_object();

	++heartbeat_tick;

	// �C��B�z�@�� Condition
	process_heartbeat_condition();

	if( !objectp(me) ) return;

	// �C��B�z�@�� Status
	process_heartbeat_status();

	// �C��B�z�@�� Delay Decay
	delay_decay();

	// �C��B�z�@�� Stuck Decay
	stuck_decay();
	
	// �C��B�z�@�� Command Decay
	if( userp(me) )
		process_heartbeat_command_decay();

	// �C���
	if( !((heartbeat_tick+1) % 2) )
	{
		if( !me->is_dead() )
		{
			if( COMBAT_D->in_fight(me) )
			{
				string stance_type = query("combat/stance/type", me);

				if( stance_type == "medic" || stance_type == "medic-adv" )
				{
					int medic_level;
					
					if( stance_type == "medic" )
					{
						medic_level = me->query_skill_level("medic-stance");
						if( medic_level > 0 )
						{
							if( !me->is_stamina_full() )
								me->earn_stamina(me->stamina_regen()*medic_level/500);
							if( !me->is_health_full() )
								me->earn_health(me->health_regen()*medic_level/500);
							if( !me->is_energy_full() )
								me->earn_energy(me->energy_regen()*medic_level/500);
						}
					}
					else
					{
						medic_level = me->query_skill_level("medic-stance-adv");
						if( medic_level > 0 )
						{
							if( !me->is_stamina_full() )
								me->earn_stamina(me->stamina_regen()*medic_level/360);
							if( !me->is_health_full() )
								me->earn_health(me->health_regen()*medic_level/360);
							if( !me->is_energy_full() )
								me->earn_energy(me->energy_regen()*medic_level/360);
						}
					}
				}
			}
			else
			{		
				if( !me->is_stamina_full() )
					me->earn_stamina(me->stamina_regen());
				if( !me->is_health_full() )
					me->earn_health(me->health_regen());
				if( !me->is_energy_full() )
					me->earn_energy(me->energy_regen());
			}

			if( me->query_food_cur() > 0 )
				me->cost_food(2);
			if( me->query_drink_cur() > 0 )
				me->cost_drink(2);
		}
				
		// �Y�b���˪��A	
		if( me->is_faint() )
		{
			float recover_speed = 1 + me->query_skill_level("consciousness")/100.;
			
			string msg = "";
			
			msg += "��O "+NOR GRN+me->query_stamina_cur()+NOR"/"HIG+me->query_stamina_max()+NOR" ";
			msg += "�ͩR "+NOR YEL+me->query_health_cur()+NOR"/"HIY+me->query_health_max()+NOR" ";
			msg += "�믫 "+NOR BLU+me->query_energy_cur()+NOR"/"HIB+me->query_energy_max()+NOR;
	
			// �{�� block msg
			delete_temp("disable/msg");
			tell(me, sprintf(HIW"��"NOR"��"WHT"�� %s "NOR WHT"��"NOR"��"HIW"��\n"NOR, msg));
			set_temp("disable/msg",1);
				
			if( me->query_stamina_cur()*recover_speed >= me->query_stamina_max()/2 )
				me->wakeup();
		}

		if( me->is_npc() )
		{
			if( !query("heartbeat") && me->is_all_attribution_full() && !me->is_delaying() && !me->is_stucking() && !sizeof(query("condition", me)) && !sizeof(query_temp("status", me)) )
				me->stop_heartbeat();
		}
	}
	
	// ���a����
	if( interactive(me) )
	{
		// �C����
		if( !((heartbeat_tick+2) % 6) )
		{
			if( query_idle(me) > 300 )
			{
				if( !query_temp("startidle") )
				{
					tell(me, pnoun(2, me)+"�w�g�s��o�b�W�L�������A�N�}�l�ֿn�o�b�ޯ�g��ȡC\n");
					set_temp("startidle", 1);
				}
				me->add_skill_exp("idle", pow(query_idle(me), 0.3));
			}
			else if( query_temp("startidle") ) 
				delete_temp("startidle");
		}
		
		// ���_�u����(�C 60 ��e�X�@�� NOR ��X)
		if( !((heartbeat_tick+3) % 60) )
		{
			receive(NOR);
		}
	
		// �C���ɶ� 1 �p��(�Y��ڮɶ� 120 ��)�����@���~��
		if( !((heartbeat_tick+4) % 120) )
		{
			if( addn("age_hour", 1) == 8760 )
				BIRTHDAY_D->grow_up(me);
		}
	
		
		// 600 ���x�s�@�����a�ɮ�, 
		if( !((heartbeat_tick+5) % 600) )
		{
			object labor_ob;
			string labor, *labors = query("hirelabors");
			string mailfile = read_file(user_mail(me->query_id(1)));
			
			addn("total_online_time", time() - query("last_on/time"));
	
			// �����̫�@���b�u�W���ɶ�, �ھڦ���ƥ� CLEAN_D �B�z�b���R��
			set("last_on/time", time());
	
			if( environment() )
				set("quit_place", copy(query_temp("location")) || base_name(environment()));
	
			me->save();
			
			if( sizeof(labors) )
			foreach(labor in labors)
				if( file_exists(labor) && objectp(labor_ob = load_object(labor)) )
					labor_ob->save();
			
			//�ˬd�O�_���ӤH�H��
			if( stringp(mailfile) )
			{
				int unread;
				mapping maildata = restore_variable(mailfile);
				
				if( mapp(maildata) )
				foreach(int time, mapping mail in maildata)
					if( undefinedp(mail["read"]) )
						unread++;
				
				if( unread > 0 )
					tell(me, HIG"�H"NOR GRN"��"NOR"�G"+pnoun(2, me)+"�@�� "+unread+" �ʫH��Ū�A�Ш�F���}�]���l���\Ū�H��C\n");
			}
		}
		
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+6) % 3600) )
			TOP_D->update_top_rich(me);
	
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+7) % 3600) )
			TOP_D->update_top_social_exp(me);
	
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+8) % 3600) )
			TOP_D->update_top_combat_exp(me);
	
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+9) % 3600) )
			TOP_D->update_top_skill(me);
			
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+10) % 3600) )
			TOP_D->update_top_combat(me);

		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+11) % 3600) )
			TOP_D->update_top_attr(me);
			
		// �C�@�p�ɧ�s�@���Ʀ�]����
		if( !((heartbeat_tick+12) % 3600) )
			TOP_D->update_top_age(me);
		
		// �C�@�p�ɴ����@���s����|�N	
		if( !((heartbeat_tick+13) % 3600) )
		{
			if( query("total_online_time") < 86400*45 && !me->in_condition(NEWBIE) )
				tell(me, HIY"�s��"NOR YEL"����"NOR"�G�W�u�ɶ����� 45 �Ѫ����a���i�H��U�������F������F�ưȩҤ��\Ū�s����|�N(read)�A�i��o�B�~ "HIG"50"NOR GRN"%"NOR" ���g��ȥ[����I\n"NOR);
				
		}
	}
/*
	NPC �欰��z�� :
		
	*behavior/reply_say	���� say ���O�ɳQ�ʩI�s
	(function) fp(object this_player(), string "���ܤ��e")
	
	*behavior/reply_emote	��������O�ɳQ�ʩI�s
	(function) fp(object this_player(), string "�����O", string "�����[��r")
	
	*behavior/walking	�� heart_beat �D�ʨ̷� walking �]�w��Ʃw������
	(int) ���

	*behavior/random	�� heart_beat �C���I�s
	(mapping) random =
	([
		fp 1	:	�C�� heart_beat ���@�������O�����v 1(���:�U�����@),
		fp 2	:	�C�� heart_beat ���@�������O�����v 2(���:�U�����@),
	])
*/

	if( me->is_npc()  )
	{
		mapping behavior = query("behavior");
		
		if( !mapp(behavior) )
			return;

		// ���ʦ欰
		if( behavior["walking"] && !(heartbeat_tick % behavior["walking"]) )
			this_object()->behavior_walking();

		// ���N���O�欰
		if( sizeof(behavior["random"]) )
		{
			foreach(function fp, int prob in behavior["random"])
			if( prob > random(10000) )
			{
				if( functionp(fp) & FP_OWNER_DESTED )
			        {
			                if( functionp(fp) & FP_NOT_BINDABLE )
			                	error("heart_beat_npc.c reply_say() �禡�L�k�A���s���C\n");
			
			                fp = bind(fp, this_object());
			        }
				evaluate(fp);
			}
		}
	}
}
