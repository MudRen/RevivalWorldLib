/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _heart_beat_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-26
 * Note   : 生物心跳
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

// Status 變化
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

// Condition 變化
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

// 每 1 秒呼叫一次
nomask void heart_beat()
{
	object me = this_object();

	++heartbeat_tick;

	// 每秒處理一次 Condition
	process_heartbeat_condition();

	if( !objectp(me) ) return;

	// 每秒處理一次 Status
	process_heartbeat_status();

	// 每秒處理一次 Delay Decay
	delay_decay();

	// 每秒處理一次 Stuck Decay
	stuck_decay();
	
	// 每秒處理一次 Command Decay
	if( userp(me) )
		process_heartbeat_command_decay();

	// 每兩秒
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
				
		// 若在昏倒狀態	
		if( me->is_faint() )
		{
			float recover_speed = 1 + me->query_skill_level("consciousness")/100.;
			
			string msg = "";
			
			msg += "體力 "+NOR GRN+me->query_stamina_cur()+NOR"/"HIG+me->query_stamina_max()+NOR" ";
			msg += "生命 "+NOR YEL+me->query_health_cur()+NOR"/"HIY+me->query_health_max()+NOR" ";
			msg += "精神 "+NOR BLU+me->query_energy_cur()+NOR"/"HIB+me->query_energy_max()+NOR;
	
			// 閃避 block msg
			delete_temp("disable/msg");
			tell(me, sprintf(HIW"☆"NOR"☆"WHT"☆ %s "NOR WHT"☆"NOR"☆"HIW"☆\n"NOR, msg));
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
	
	// 玩家角色
	if( interactive(me) )
	{
		// 每六秒
		if( !((heartbeat_tick+2) % 6) )
		{
			if( query_idle(me) > 300 )
			{
				if( !query_temp("startidle") )
				{
					tell(me, pnoun(2, me)+"已經連續發呆超過五分鐘，將開始累積發呆技能經驗值。\n");
					set_temp("startidle", 1);
				}
				me->add_skill_exp("idle", pow(query_idle(me), 0.3));
			}
			else if( query_temp("startidle") ) 
				delete_temp("startidle");
		}
		
		// 防斷線機制(每 60 秒送出一次 NOR 色碼)
		if( !((heartbeat_tick+3) % 60) )
		{
			receive(NOR);
		}
	
		// 遊戲時間 1 小時(即實際時間 120 秒)紀錄一次年齡
		if( !((heartbeat_tick+4) % 120) )
		{
			if( addn("age_hour", 1) == 8760 )
				BIRTHDAY_D->grow_up(me);
		}
	
		
		// 600 秒儲存一次玩家檔案, 
		if( !((heartbeat_tick+5) % 600) )
		{
			object labor_ob;
			string labor, *labors = query("hirelabors");
			string mailfile = read_file(user_mail(me->query_id(1)));
			
			addn("total_online_time", time() - query("last_on/time"));
	
			// 紀錄最後一次在線上的時間, 根據此資料由 CLEAN_D 處理帳號刪除
			set("last_on/time", time());
	
			if( environment() )
				set("quit_place", copy(query_temp("location")) || base_name(environment()));
	
			me->save();
			
			if( sizeof(labors) )
			foreach(labor in labors)
				if( file_exists(labor) && objectp(labor_ob = load_object(labor)) )
					labor_ob->save();
			
			//檢查是否有個人信件
			if( stringp(mailfile) )
			{
				int unread;
				mapping maildata = restore_variable(mailfile);
				
				if( mapp(maildata) )
				foreach(int time, mapping mail in maildata)
					if( undefinedp(mail["read"]) )
						unread++;
				
				if( unread > 0 )
					tell(me, HIG"信"NOR GRN"件"NOR"："+pnoun(2, me)+"共有 "+unread+" 封信件未讀，請到政府開設的郵局閱讀信件。\n");
			}
		}
		
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+6) % 3600) )
			TOP_D->update_top_rich(me);
	
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+7) % 3600) )
			TOP_D->update_top_social_exp(me);
	
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+8) % 3600) )
			TOP_D->update_top_combat_exp(me);
	
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+9) % 3600) )
			TOP_D->update_top_skill(me);
			
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+10) % 3600) )
			TOP_D->update_top_combat(me);

		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+11) % 3600) )
			TOP_D->update_top_attr(me);
			
		// 每一小時更新一次排行榜紀錄
		if( !((heartbeat_tick+12) % 3600) )
			TOP_D->update_top_age(me);
		
		// 每一小時提醒一次新手培育術	
		if( !((heartbeat_tick+13) % 3600) )
		{
			if( query("total_online_time") < 86400*45 && !me->in_condition(NEWBIE) )
				tell(me, HIY"新手"NOR YEL"提醒"NOR"：上線時間未滿 45 天的玩家都可以到各城市市政府的戶政事務所內閱讀新手培育術(read)，可獲得額外 "HIG"50"NOR GRN"%"NOR" 的經驗值加成喔！\n"NOR);
				
		}
	}
/*
	NPC 行為整理表 :
		
	*behavior/reply_say	執行 say 指令時被動呼叫
	(function) fp(object this_player(), string "說話內容")
	
	*behavior/reply_emote	執行表情指令時被動呼叫
	(function) fp(object this_player(), string "表情指令", string "表情附加文字")
	
	*behavior/walking	由 heart_beat 主動依照 walking 設定秒數定期走動
	(int) 秒數

	*behavior/random	由 heart_beat 每次呼叫
	(mapping) random =
	([
		fp 1	:	每個 heart_beat 做一次此指令的機率 1(單位:萬分之一),
		fp 2	:	每個 heart_beat 做一次此指令的機率 2(單位:萬分之一),
	])
*/

	if( me->is_npc()  )
	{
		mapping behavior = query("behavior");
		
		if( !mapp(behavior) )
			return;

		// 走動行為
		if( behavior["walking"] && !(heartbeat_tick % behavior["walking"]) )
			this_object()->behavior_walking();

		// 任意指令行為
		if( sizeof(behavior["random"]) )
		{
			foreach(function fp, int prob in behavior["random"])
			if( prob > random(10000) )
			{
				if( functionp(fp) & FP_OWNER_DESTED )
			        {
			                if( functionp(fp) & FP_NOT_BINDABLE )
			                	error("heart_beat_npc.c reply_say() 函式無法再次連結。\n");
			
			                fp = bind(fp, this_object());
			        }
				evaluate(fp);
			}
		}
	}
}
