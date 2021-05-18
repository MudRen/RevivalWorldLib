/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battle.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : �Գ����O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
   �Գ����O�A�i�H���W�P���}�Գ�

   battle info				- ��ܥثe�Ԫ���T
   battle join				- ���W�ѥ[�U�@�������Գ�
   battle join ���ҽX			- ��J���ҽX�H�����[�J�Գ�
   battle cancel			- �������W
HELP;

private void do_command(object me, string arg)
{
	//if( !wizardp(me) )
	//	return tell(me, "�t�δ��դ��A�ȮɵL�k�[�J�Ԫ��C\n");

	switch(arg)
	{
		case "info":
		{
			string msg;
			string *players = BATTLEFIELD_D->get_sorted_players();
			string score = "";
			int count = 0;
			
			msg = "�ثe�@ "+sizeof(players)+" �쪱�a�ѻP�Ԫ��A"+(member_array(me->query_id(1), players)==-1?pnoun(2, me)+"���ѻP�Ԫ�":pnoun(2, me)+"�O�䤤���@")+"�C\n";
						
			if( sizeof(players) > 0 )
			{
				int attack=0, defend=0, medic=0, speed=0, none=0;
				object player;
		
				foreach(string id in players)
				{
					if( !objectp(player = find_player(id)) )
						continue;
			
					if( ++count <= 3 )
						score += player->query_idname()+"�B";
					
					switch(query("combat/stance/type", player))
					{
						case "attack":
						case "attack-adv":
							attack++;
							break;
						case "defend":
						case "defend-adv":
							defend++;
							break;
						case "speed":
						case "speed-adv":
							speed++;
							break;
						case "medic":
						case "medic-adv":
							medic++;
							break;
						default:
							none++;
							break;
					}
				}
				
				msg += "�ѻP�̱ĥΪ��԰����A�έp�G"
				+STANCE_D->query_stance_name("defend")+"["+defend+"]�B"
				+STANCE_D->query_stance_name("attack")+"["+attack+"]�B"
				+STANCE_D->query_stance_name("speed")+"["+speed+"]�B"
				+STANCE_D->query_stance_name("medic")+"["+medic+"]�B"
				+WHT"�L���A"NOR"["+none+"]�C\n";
				
				msg += "�ثe�Ԫ��Z���ƦW�e�T�W���G"+score[0..<3]+"�C\n";
			}
			
			return tell(me, msg);
			break;
		}
		case "join":
		{
			//string id, code;
			
			if( !BATTLEFIELD_D->is_battle_open() )
				return tell(me, "�ثe�L�k���W�Գ��C\n");

			if( BATTLEFIELD_D->in_battle(me) )
				return tell(me, pnoun(2, me)+"�w�g�b�Գ����F�C\n");
				
			BATTLEFIELD_D->join_battle(me);
			/*	
			id = me->query_id(1);
			code = sprintf("%c%c%c%c%c", range_random(97,122), range_random(97,122), range_random(97,122), range_random(97,122), range_random(97,122));
			
			MYSQL_SCHEDULE_D->set_captcha(id, code);
			set_temp("battlefield_captcha_code", code, me);
			return tell(me, HIY"\n�إ߷s���Գ����ҽX...�ЦܥH�U�������o"+pnoun(2, me)+"�����ҽX�G\n\n"NOR"http://www.revivalworld.org/rw/captcha?id="+id+HIG"\n\n���o���ҽX���J�ubattle join ���ҽX�v�H�[�J�Գ��C\n"NOR);
			*/
			break;
		}
		case "cancel":
			BATTLEFIELD_D->cancel_battle(me);
			break;
		default:
		{
			/*
			if( arg && sscanf(arg, "join %s", arg) == 1 && sizeof(arg) > 0 )
			{
				if( arg == query_temp("battlefield_captcha_code", me) )
				{
					MYSQL_SCHEDULE_D->delete_captcha(me->query_id(1));
					delete_temp("battlefield_captcha_code", me);
					BATTLEFIELD_D->join_battle(me);
					
					tell(me, HIG+"�����ҽX�ɶ����I�@�U�����W���s�i�a�ARW ���]�ơB�����B�q�O�O�ݭn�����_��������X��(�_)\n"HIY"�����G�h�I�L�q�A�C���I��W�����k��s�i�U 3 �����ΡA�I���O��������s�i�����A���d�[�@�I�~�|���ĪG��(��)\n"NOR);
				}
				else
					return tell(me, "���ҽX��J���~�C\n");
			}
			else*/
				return tell(me, "�п�J���T�����O�榡(help battle)�C\n");
		}
	}
}
