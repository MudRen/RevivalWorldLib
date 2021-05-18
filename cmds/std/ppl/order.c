/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : order.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-19
 * Note   : �R�O���O
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <npc.h>
#include <skill.h>
#include <secure.h>

inherit COMMAND;

#define MAX_SOLDIER		5
#define MAX_SPORTER		10
#define MAX_ADVENTURER	5

string help = @HELP
   �o�i�H���A�U�R�O���ۤv���H�O�A�ثe�}�񪺩R�O�p�U�G

�S��R�O���O
  order '���u' to number '�s��'		- �]�w���u�s���A�i���нs���Alabor �C��̦��s���Ƨ�
  order '���u' to name '����'		- �����u������W�r
  order '���u' to id '�^��'		- �����u���^��W�r

  order '���u' to work '�u�@'		- �R�O���u�q�Ƥu�@(�Ҧp������ clerk)
                                          �u�@������ clerk / worker / rd / soldier / sporter / adventurer
  order '���u' to work -d		- �R�O���u����u�@
  
  order '���u' to learn '�ޯ�'		- �R�O���u�V�z�ǲߧޯ�(�z����������Ƥf�~�ޯ�P�оǧޯ�)
  order '���u' to learn '�ޯ�' 50%	- �@���ӶO 50% ���믫�ӱоɭ��u
 
  order '���u' to giveup '�ޯ�' 10	- �R�O���u�N�Y���ޯ୰�Ŭ� 10 ��(���� 0 �ūh�O�������)

�зǨt�Ϋ��O
  order '���u' to command		- �d�߭��u�i�H�ϥΪ��t�Ϋ��O
  order '���u' to '���O'		- �R�O���u����Y�Өt�Ϋ��O(���O�Ϊk�P���a���`���Ϊk�ۦP)


�������O: labor
HELP;

private void do_command(object me, string arg)
{
	string id, key, value;
	object npc, env;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�R�O�֡H\n");

	if( sscanf(arg, "%s to %s", id, key) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n�R�O�ְ�����ơH(�� order boy to name �k��)\n");
		
	env = environment(me);

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( BATTLEFIELD_D->inside_battlefield(me) )
		return tell(me, "�Գ����L�k�ϥΩR�O���O�C\n");

	if( !objectp(npc = present(id, env)) )
		return tell(me, "�o�̨S�� "+id+" �o�ӤH�C\n");
	
	if( !npc->is_living() )
		return tell(me, pnoun(2, me)+"�L�k�R�O�@�ӵL�ͩR���F��C\n");
	
	if( !npc->is_npc() || (!wizardp(me) && query("boss", npc) != me->query_id(1)) )
		return tell(me, npc->query_idname()+"�ä��O"+pnoun(2, me)+"���Ϊ��C\n");

	sscanf(key, "%s %s", key, value);

	if( npc->is_faint() )
		return tell(me, pnoun(2, me)+"�L�k�R�O���ˤ������u������Ʊ��C\n");
	else if( npc->is_dead() )
		return tell(me, pnoun(2, me)+"�L�k�R�O���`�����u������Ʊ��C\n");
		
	npc->startup_living();

	switch(lower_case(key))
	{
		case "name":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��"+npc->query_idname()+"�����򤤤�W�r�H\n");

			value = kill_repeat_ansi(value+NOR);

			if( !is_chinese(value) )
				return tell(me, "��"+pnoun(2, me)+"�Ρu����v���W�r�C\n");
	
			if( noansi_strlen(value) > 12 || noansi_strlen(value) < 2 )
				return tell(me, "����W�r�����b 1 �� 6 �Ӥ���r�����C\n");

			msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"�H�ᤤ��W�l�N��W�s���u"+value+"�v�a�I\n", me, npc, 1);
			npc->set_idname(0, value);
			npc->do_command("say ���¦������ڨ��s������W�r�A�ڤ@�w�|���t�z������...");

			npc->save();				
			break;
		}
		case "id":
		{
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��"+npc->query_idname()+"������^��W�r�H\n");
			
			value = replace_string(kill_repeat_ansi(lower_case(value)+NOR)," ", "");
			
			if( !is_english(value) )
				return tell(me, "��"+pnoun(2, me)+"�Ρu�^��v���W�r�C\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "�^��W�r�����b 3 �� 16 �ӭ^��r�Ƥ����C\n");
				
			if( user_exists(remove_ansi(value)) )
				return tell(me, "���o���W���{�����a���W�r�C\n");
				
			msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"�H��^��W�l�N��W�s���u"+value+"�v�a�I\n", me, npc, 1);
			npc->set_idname(value, 0);
			npc->do_command("say ���¦������ڨ��s���^��W�r�A�ڤ@�w�|���t�z������...");
			
			npc->save();
			break;
		}
		case "work":
		{
			int job;
			string job_name;

			if( COMBAT_D->in_fight(npc) )
				return tell(me, npc->query_idname()+"���b�԰����A�L�k�i��u�@�����C\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�s"+npc->query_idname()+"������˪��u�@(clerk, worker, rd, soldier)�H\n");

			if( time() < query("last_die_time", npc) + 300 )
				return tell(me, "���u���`�ᥲ������ 5 �����~�୫�s�����u�@�C\n");

			if( query("jog/type", npc) == SPORTER && BASEBALL_D->is_playing(npc) )
				return tell(me, npc->query_idname()+"���b�i��y�ɡA�ثe�L�k�ഫ�u�@�C\n");

			value = lower_case(value);

			switch( value )
			{
				case "-d":
					if( !query("job", npc) )
						return tell(me, npc->query_idname()+"�쥻�N�S���b�i��u�@�C\n");
					
					msg("$ME�s$YOU�������Ҧ��u�@�C\n", me, npc, 1);
					
					delete("job", npc);

					if( objectp(query_temp("assisting", npc)) )
						npc->do_command("assist -d");

					npc->do_command("say �O������C");
					return;
				case "rd":
					job = RD;		
					job_name = HIR"��"NOR RED"�o"NOR;
					break;
				case "worker":
					job = WORKER;
					job_name = HIY"�u"NOR YEL"�H"NOR;
					break;
				case "clerk":
				{
					int limit = LABOR_D->get_labor_limit(me);
					
					if( sizeof(LABOR_D->get_labors(me, CLERK)) >= (limit/2)+(limit%2) )
						return tell(me, pnoun(2, me)+"�������ƶq�w�g�F���u�H�ƤW���� 50%�A�L�k�A�W���H��C\n");
						
					job = CLERK;
					job_name = HIC"��"NOR CYN"��"NOR;
					break;
				}
				case "adventurer":
				{
					if( sizeof(LABOR_D->get_labors(me, ADVENTURER)) >= MAX_ADVENTURER )
						return tell(me, pnoun(2, me)+"�����I���ƶq�w�g��F "+MAX_ADVENTURER+" ��A�L�k�A�W���H��C\n");
					
					job = ADVENTURER;
					job_name = HIM"��"NOR MAG"�I��"NOR;
					break;
				}
				case "soldier":
				{
					if( sizeof(LABOR_D->get_labors(me, SOLDIER)) >= MAX_SOLDIER )				
						return tell(me, pnoun(2, me)+"���x�H�ƶq�w�g��F "+MAX_SOLDIER+" ��A�L�k�A�W���H��C\n");
						
					job = SOLDIER;
					job_name = HIG"�x"NOR GRN"�H"NOR;
					break;
				}
				case "sporter":
				{			
					if( sizeof(LABOR_D->get_labors(me, SPORTER)) >= MAX_SPORTER	)
						return tell(me, pnoun(2, me)+"���y���ƶq�w�g��F "+MAX_SPORTER+" ��A�L�k�A�W���H��C\n");

					job = SPORTER;
					job_name = HIW"�y"NOR WHT"��"NOR;
					break;
				}
				default:
					return tell(me, "�n����ؤu�@(clerk, worker, rd, soldier)�H\n"NOR);
			}
					
			msg("$ME�h�J$YOU�D�G�q���Ѱ_"+pnoun(2, npc)+"�N���u"+job_name+"�v���u�@�a�I\n", me, npc, 1);
			npc->do_command("say �n���I�ڤ@�w�|�ɤߺɤO���u�@���I");
	
			set("job/name", job_name, npc);
			set("job/type", job, npc);

			if( job == SOLDIER )
			{
				npc->do_command("follow "+me->query_id(1));
				npc->do_command("assist "+me->query_id(1));
			}
			else
			{
				npc->do_command("follow -d");
				if( objectp(query_temp("assisting", npc)) )
					npc->do_command("assist -d");
			}

			npc->save();
			break;
		}
		case "number":
		{
			int number = to_int(value);
			int limit = LABOR_D->get_labor_limit(me);
			
			if( number < 1 || number > limit )
				return tell(me, "�ثe"+pnoun(2, me)+"�����u�s���]�w�u�श�� 1 �� "+limit+" �����C\n");
			
			set("number", number, npc);
			
			msg("$ME�h�J$YOU�D�G"+pnoun(2, npc)+"�������s���{�b�O "+number+" ���F�I\n", me, npc, 1);
			npc->do_command("say �O���A����...");
			
			npc->save();
			
			break;	
		}
		case "giveup":
		{
			int npc_level;
			int to_level = 0;

			if( !value )
				return tell(me, pnoun(2, me)+"�Q�n"+npc->query_idname()+"��󤰻�ޯ�H\n");
			
			if( sscanf(value, "%s %d", value, to_level) != 2 )
				return tell(me, pnoun(2, me)+"�Q�n"+npc->query_idname()+"��󤰻�ޯ�ܴX�šH\n");
							
			if( !SKILL_D->skill_exists(value) )
				return tell(me, "�èS�� "+value+" �o�اޯ�C\n");
							
			npc_level = npc->query_skill_level(value);
			
			if( npc_level < 1 )
				return tell(me, npc->query_idname()+"�쥻�K�S�������ޯ�C\n");
			
			if( to_level < 0 || to_level >= npc_level )
				return tell(me, npc->query_idname()+"�L�k�N���ޯ୰�� "+to_level+" �šC\n");
				
			if( to_level == 0 )
			{
				msg("$ME�h�J$YOU�D�G���ϥ�"+(SKILL(value))->query_idname()+"���ޯ�a�I\n", me, npc, 1);
				npc->do_command("say �O���A����...");
				
				npc->add_skill_exp(value, -1000000000000);
				
				delete("skills/"+value, npc);
			}
			else if( to_level > 0 )
			{
				int from_exp = to_int(query("skills/"+value+"/exp", npc));
				int to_exp = SKILL_D->query_level_exp(to_level);
				
				msg("$ME�h�J$YOU�D�G�N"+(SKILL(value))->query_idname()+"���ޯ୰�� "+to_level+" �ŧa�I\n", me, npc, 1);
				npc->do_command("say �O���A����...");
				
				npc->add_skill_exp(value, (to_exp - from_exp)/(SKILL(value))->exp_multiple());
			}

			npc->save();
			break;
		}
		case "learn":
		{
			int percent;
			int energy;
			int max_level;
			int npc_level;
			int teach_level = me->query_skill_level("teach");
			
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n"+npc->query_idname()+"�ǲߤ���ޯ�H\n");

			if( sscanf(value, "%s %d%%", value, percent) == 2 )
			{
				if( percent <= 0 || percent > 100 )
					return tell(me, "�п�J���T���믫�ӶO�ʤ���C\n");
					
				energy = percent * me->query_energy_max() / 100;
			}
			else
				energy = 50;

			value = lower_case(value);

			if( !SKILL_D->skill_exists(value) )
				return tell(me, "�èS�� "+value+" �o�اޯ�C\n");
	
			npc_level = npc->query_skill_level(value);

			if( teach_level < 100 && npc_level >= teach_level )
				return tell(me, pnoun(2, me)+"��"+(SKILL("teach"))->query_idname()+"���Ť����A"+npc->query_idname()+"��"+(SKILL(value))->query_idname()+"���Ť���A���W���ɡC\n");

			max_level = (SKILL(value))->max_level(npc);
			
			if( npc_level >= max_level )
				return tell(me, npc->query_idname()+"��"+(SKILL(value))->query_idname()+"�ޯ൥�Ťw�g��F�W���C\n");

			if( !(SKILL(value))->allowable_learn(npc) )
				return tell(me, npc->query_idname()+"�L�k�ǲ߳o���ޯ�C\n");
				
			if( !me->cost_energy(energy) )
				return tell(me, pnoun(2, me)+"���믫���� "+energy+"�A�L�k�A�Ǳ§ޯ�F�C\n");
							
			msg("$ME�ӶO "+energy+" ���믫�J�Ӧa�Ǳ�$YOU����"+(SKILL(value))->query_idname()+"���g��C\n"+npc->query_idname()+"��"+(SKILL(value))->query_idname()+"���F��i�@�B���A�ѡC\n", me, npc, 1);
			
			npc->add_skill_exp(value, pow((me->query_skill_level("eloquence")+1) * energy, 1.001));
			
			npc_level = npc->query_skill_level(value);
			
			if( npc_level >= max_level )
			{
				set("skills/"+value+"/level", max_level, npc);
				set("skills/"+value+"/exp", (SKILL(value))->level_exp(max_level), npc);
				//tell(me, npc->query_idname()+"��"+(SKILL(value))->query_idname()+"�ޯ൥�Ťw�g����A���W���ɡC\n");
			}
			
			npc->delay_save(300);
			break;
		}
		case "command":
		{
			npc->do_command("say �ڥثe�i�H��檺�t�Ϋ��O���G"+implode(COMMAND_D->query_available_command(STANDARD), ", "));
			break;
		}

		default:
		{
			msg("$ME�R�O$YOU�G�u"HIY+(value?key+" "+value:key)+NOR"�v�C\n", me, npc, 1);

			if( !wizardp(me) && (COMMAND_D->find_chn_command(key) || COMMAND_D->find_emt_command(key)) )
				return npc->do_command("say �ڤ���b�W�D�W�o���]����@��������O");

			npc->do_command(value?key+" "+value:key);
			break;
		}
	}
}