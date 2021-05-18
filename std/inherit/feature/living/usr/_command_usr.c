/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-10-08
 * Note   : 私人特殊指令系統
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <function.h>

private nosave int total_timecost = 0;
private nosave int total_commands = 0;
private nosave int force_quit_times = 0;
private nosave int timecost_record;

private nomask void do_action(mapping fit_actions, string verb, string arg);

#define MAX_TIMECOST				5000000
#define TIMECOST_DECAY_PER_SECOND		50000
#define MAX_COMMANDS				500
#define MAX_COMMANDS_WITH_MCCP			1000
#define COMMANDS_DECAY_PER_SECOND		2
#define COMMANDS_DECAY_PER_SECOND_WITH_MCCP	4
#define FORCE_QUIT_LIMIT			30

int query_timecost_record()
{
	return timecost_record;
}

//
// 解析指令
//
private nomask void evaluate_command(string verb, string args)
{
	function command_fp;
	mapping all_actions;
	object ob;
	object env = environment();
	object *env_action_ob;
	mapping inv_ob_actions;
	
	all_actions = this_object()->query_actions() || allocate_mapping(0);

	if( objectp(env) )
	{
		env_action_ob = present_objects(this_object());

		if( !env->is_maproom() )
			env_action_ob += ({ env });
	}

	if( !mapp(all_actions[verb]) )
		all_actions[verb] = allocate_mapping(0);

	/* 將所有 actions 整合起來 */
	if( sizeof(env_action_ob) )
	foreach( ob in env_action_ob )
		if( !userp(ob) && mapp(inv_ob_actions = ob->query_actions()) && functionp(inv_ob_actions[verb]) )
			all_actions[verb] += ([ ob : inv_ob_actions[verb] ]);
	
	// 執行 Action
	if( sizeof(all_actions[verb]) )
	{
		do_action(all_actions[verb], verb, args);
	}
	// 執行系統指令
	else if( functionp(command_fp = COMMAND_D->find_ppl_command(verb)) || wizardp(this_object()) && functionp(command_fp = COMMAND_D->find_wiz_command(verb)) )
	{
		if( functionp(command_fp) & FP_OWNER_DESTED )
		{
			CHANNEL_D->channel_broadcast("sys", "錯誤："+this_object()->query_idname(1)+" "+verb+" "+args+" 之指令函式指標擁有者已被摧毀");
			tell(this_object(), "「"+verb+"」之指令程式發生錯誤，系統已通知巫師處理。\n"); 
			return;
		}
	
		evaluate(command_fp, this_object(), args);
	}
	else 
		tell(this_object(), "沒有「"+remove_ansi(verb)+"」這個指令，請參考 Command 指令。\n");
}


//
// 執行指令
//
private nomask void process_command(string verb, string args, string input)
{
	object me = this_object();
	int command_timecost;

	if( !objectp(me) ) return;

	// 檢查是否被限制指令速度
	if( query("cmdlimit") )
	{
		if( query_temp("command_delay") > time() )
		{
			tell(me, pnoun(2, me)+"的指令下達速度遭到系統限制，可能是因為違反了某些遊戲規則而遭到處罰。\n"
				+pnoun(2, me)+"必須等待 "+(query_temp("command_delay") - time())+" 後才能再下指令，有任何意見請與巫師聯絡。\n");
			
			return;
		}
		else
			set_temp("command_delay", copy(query("cmdlimit"))+time());
	}

	if( force_quit_times > FORCE_QUIT_LIMIT )
	{
		tell(me, pnoun(2, me)+"連續 "+FORCE_QUIT_LIMIT+" 次無視系統警告，對系統造成過大的負擔，強制離線。\n");
		CHANNEL_D->channel_broadcast("sys", "LOGOUT_D: "+me->query_idname()+"連續 "+FORCE_QUIT_LIMIT+" 次無視系統警告，對系統造成過大的負擔，強制離線。\n");
		set_temp("quiting", 1);
		LOGOUT_D->quit(me, "");
		return;
	}

	// 速度太快(有開 mccp)
	if( compressedp(me) )
	{
		if( total_commands > MAX_COMMANDS_WITH_MCCP && !wizardp(me))
		{
			tell(me, pnoun(2, me)+"的指令速度超過系統限制("+COMMANDS_DECAY_PER_SECOND_WITH_MCCP+" 指令數/秒)，"+(((total_commands - MAX_COMMANDS_WITH_MCCP)/COMMANDS_DECAY_PER_SECOND_WITH_MCCP)||1)+" 秒後才能再下達指令。\n");
	
			force_quit_times++;
			
			return;
		}
	}
	// 速度太快(未開 mccp)
	else if( total_commands > MAX_COMMANDS && !wizardp(me) )
	{
		tell(me, pnoun(2, me)+"未啟動 MCCP 功能，指令速度超過系統限制("+COMMANDS_DECAY_PER_SECOND+" 指令數/秒)，"+(((total_commands - MAX_COMMANDS)/COMMANDS_DECAY_PER_SECOND)||1)+" 秒後才能再下達指令。\n");
		
		force_quit_times++;
		
		return;
	}

	force_quit_times = 0;

	command_timecost = time_expression 
	{ 
		evaluate_command(verb, args);
	};

	// Quit 之後可能發生的情形
	if( !objectp(me) ) return;
		
	if( command_timecost > 100000 )
		CHANNEL_D->channel_broadcast("nch", sprintf("%s下達指令 %s 超過系統限制，執行時間(%fs) \n", me->query_idname(1), input, to_float(command_timecost)/1000000));

	// 累計 timecost
	timecost_record += command_timecost;

	total_timecost += command_timecost;
	total_commands++;
}

int query_total_timecost()
{
	return total_timecost;
}

int query_total_command()
{
	return total_commands;
}

private void process_heartbeat_command_decay()
{
	if( total_timecost > TIMECOST_DECAY_PER_SECOND )
		total_timecost -= TIMECOST_DECAY_PER_SECOND;

	if( total_commands > COMMANDS_DECAY_PER_SECOND_WITH_MCCP && compressedp(this_object()) )
		total_commands -= COMMANDS_DECAY_PER_SECOND_WITH_MCCP;

	else if( total_commands > COMMANDS_DECAY_PER_SECOND )
		total_commands -= COMMANDS_DECAY_PER_SECOND;
}
