/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _command_usr.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-10-08
 * Note   : �p�H�S����O�t��
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
// �ѪR���O
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

	/* �N�Ҧ� actions ��X�_�� */
	if( sizeof(env_action_ob) )
	foreach( ob in env_action_ob )
		if( !userp(ob) && mapp(inv_ob_actions = ob->query_actions()) && functionp(inv_ob_actions[verb]) )
			all_actions[verb] += ([ ob : inv_ob_actions[verb] ]);
	
	// ���� Action
	if( sizeof(all_actions[verb]) )
	{
		do_action(all_actions[verb], verb, args);
	}
	// ����t�Ϋ��O
	else if( functionp(command_fp = COMMAND_D->find_ppl_command(verb)) || wizardp(this_object()) && functionp(command_fp = COMMAND_D->find_wiz_command(verb)) )
	{
		if( functionp(command_fp) & FP_OWNER_DESTED )
		{
			CHANNEL_D->channel_broadcast("sys", "���~�G"+this_object()->query_idname(1)+" "+verb+" "+args+" �����O�禡���о֦��̤w�Q�R��");
			tell(this_object(), "�u"+verb+"�v�����O�{���o�Ϳ��~�A�t�Τw�q���Ův�B�z�C\n"); 
			return;
		}
	
		evaluate(command_fp, this_object(), args);
	}
	else 
		tell(this_object(), "�S���u"+remove_ansi(verb)+"�v�o�ӫ��O�A�аѦ� Command ���O�C\n");
}


//
// ������O
//
private nomask void process_command(string verb, string args, string input)
{
	object me = this_object();
	int command_timecost;

	if( !objectp(me) ) return;

	// �ˬd�O�_�Q������O�t��
	if( query("cmdlimit") )
	{
		if( query_temp("command_delay") > time() )
		{
			tell(me, pnoun(2, me)+"�����O�U�F�t�׾D��t�έ���A�i��O�]���H�ϤF�Y�ǹC���W�h�ӾD��B�@�C\n"
				+pnoun(2, me)+"�������� "+(query_temp("command_delay") - time())+" ��~��A�U���O�A������N���лP�Ův�p���C\n");
			
			return;
		}
		else
			set_temp("command_delay", copy(query("cmdlimit"))+time());
	}

	if( force_quit_times > FORCE_QUIT_LIMIT )
	{
		tell(me, pnoun(2, me)+"�s�� "+FORCE_QUIT_LIMIT+" ���L���t��ĵ�i�A��t�γy���L�j���t��A�j�����u�C\n");
		CHANNEL_D->channel_broadcast("sys", "LOGOUT_D: "+me->query_idname()+"�s�� "+FORCE_QUIT_LIMIT+" ���L���t��ĵ�i�A��t�γy���L�j���t��A�j�����u�C\n");
		set_temp("quiting", 1);
		LOGOUT_D->quit(me, "");
		return;
	}

	// �t�פӧ�(���} mccp)
	if( compressedp(me) )
	{
		if( total_commands > MAX_COMMANDS_WITH_MCCP && !wizardp(me))
		{
			tell(me, pnoun(2, me)+"�����O�t�׶W�L�t�έ���("+COMMANDS_DECAY_PER_SECOND_WITH_MCCP+" ���O��/��)�A"+(((total_commands - MAX_COMMANDS_WITH_MCCP)/COMMANDS_DECAY_PER_SECOND_WITH_MCCP)||1)+" ���~��A�U�F���O�C\n");
	
			force_quit_times++;
			
			return;
		}
	}
	// �t�פӧ�(���} mccp)
	else if( total_commands > MAX_COMMANDS && !wizardp(me) )
	{
		tell(me, pnoun(2, me)+"���Ұ� MCCP �\��A���O�t�׶W�L�t�έ���("+COMMANDS_DECAY_PER_SECOND+" ���O��/��)�A"+(((total_commands - MAX_COMMANDS)/COMMANDS_DECAY_PER_SECOND)||1)+" ���~��A�U�F���O�C\n");
		
		force_quit_times++;
		
		return;
	}

	force_quit_times = 0;

	command_timecost = time_expression 
	{ 
		evaluate_command(verb, args);
	};

	// Quit ����i��o�ͪ�����
	if( !objectp(me) ) return;
		
	if( command_timecost > 100000 )
		CHANNEL_D->channel_broadcast("nch", sprintf("%s�U�F���O %s �W�L�t�έ���A����ɶ�(%fs) \n", me->query_idname(1), input, to_float(command_timecost)/1000000));

	// �֭p timecost
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
