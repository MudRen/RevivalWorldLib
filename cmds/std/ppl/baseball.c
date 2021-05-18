/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : baseball.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : �βy���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <baseball.h>
#include <time.h>

inherit COMMAND;

#define RECORD_SIZE 	30

string help = @HELP
   �βy���O�A�i�H�d�ߴβy���ɬ�����T

   baseball			- ��ܥثe���ɶ���B�Ԫp�B�y���򥻸�T
   
   baseball info		- �d�ߦۤv���ݲy������
   baseball info '���aID'	- �d�ߨ�L���a�y������
   
   baseball listen '���aID'	- ��ť�Y�쪱�a���y�����ɹ�p
   baseball listen -d		- ���ť���ɹ�p
   
   baseball record '����'	- �d�߲y�������Ʀ�(�����]�A home, hit, hit2, hit3, run, k, era, special)
   baseball record '����' �~��	- �d�߲y�������Ʀ�(�Y�@�S�w�~��)

   baseball hint '�t��'		- �U�F�t���A�C���y�ɳ̦h�ϥ� 6 ���t��
   	
   �t�������p�U : 	
   batter 	- ���R�ثe���W���̡A���L�b�o���W�����|���u�ȴ��ɥ����¤O
   pitcher 	- ���R�ثe���W���A���L�P�ثe���̪���M���u�ȴ��ɧ�y�¤O
   bunt		- ���t�������W���̡A�n�D��i��Ĳ���u���A�H���@�S�W�]�̱��i(���p�J�t������)
   walk		- ���t�������W���A�n�D��q���|�a�O�e�ثe����(���p�J�t������)
HELP;

private void do_command(object me, string arg)
{
	string value;
	int year = BASEBALL_D->query_season_year();
	string myid = me->query_id(1);
	string *msg = allocate(0);
	
	if( !arg )
	{
		string *season = BASEBALL_D->query_season();
		mapping setup = BASEBALL_D->query_setup();
		mapping record = BASEBALL_D->query_record();
		
		int win, lose;
		int sep = 0;
		string opponent;
		string statustext;
		int rank = 0;
		int limit;
		
		msg = ({ "\n �� "HIC+year+NOR" �y�u�ثe�@�� "HIY+sizeof(season)+NOR" �Ӱ��ɲy���A�ثe���Z�ƦW�p�U�G" });
		msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
		msg += ({ sprintf(HIW" %-4s%-13s%-13s%4s%5s%5s%6s %-12s%s"NOR, "", "�y���W��", "����", "����", "�ӳ�", "�ѳ�", "�Ӳv", "�ثe���", "�ɪp") });
		msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
		
		limit = (BASEBALL_D->calculate_max_game_times(year)-10) / 2;
		
		if( limit < 10 ) limit = 10;
			
		foreach(string id in sort_array(season, (: BASEBALL_D->sort_team($1, $2) :) ))
		{
			if( mapp(record[year]) && mapp(record[year][RECORD_TEAM][id]) )
			{
				win = record[year][RECORD_TEAM][id]["win"];
				lose = record[year][RECORD_TEAM][id]["lose"];
			}
			else
			{
				win = 0;
				lose = 0;
			}
			
			if( !mapp(setup[id]) ) continue;
				
			if( win+lose < limit && sep == 0 )
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w "NOR"�����ƶq���󤣨�"WHT" �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				sep = 1;
			}
			
			if( rank == 8 )
				msg += ({ NOR RED"�w�w�w�w�w�w�w�w�w�w�w�w�w�w "HIR"�u���ɮʯű���"NOR RED" �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
			
			//else if( rank == 12 )
			//	msg += ({ NOR RED"�w�w�w�w�w�w�w�w�w�w�w�w�w�w "HIR"�u���ɮʯű���"NOR RED" �w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
			
			opponent = BASEBALL_D->query_opponent(id);
			
			switch(BASEBALL_D->query_status(id))
			{
				case STATUS_IDLE:
				{
					if( BASEBALL_D->is_in_post_season() )
					{
						if( member_array(id, BASEBALL_D->query_post_season_eliminate_teams()) != -1 )
							statustext = RED"�^�O"NOR;
						else if( member_array(id, BASEBALL_D->query_post_season_team(2)) != -1 )
							statustext = HIY"�ʯ�"NOR YEL"�`�a�x��"NOR;		
						else if( member_array(id, BASEBALL_D->query_post_season_team(4)) != -1 )
							statustext = HIG"�ʯ�"NOR GRN"�|�j��"NOR;
						else if( member_array(id, BASEBALL_D->query_post_season_team(8)) != -1 )
							statustext = HIC"�ʯ�"NOR CYN"�K�j��"NOR;
						else
							statustext = WHT"����"NOR;
					}
					else
						statustext = HIG"�t"NOR GRN"�襤"NOR;
					break;
				}
				case STATUS_PLAYING:
				{
					mapping game = BASEBALL_D->query_game(id);
						
					if( id == game[TEAM1] )
						statustext = sprintf(CYN"%-2d"NOR":"HIC"%2d"NOR, game[TEAM2SCORE], game[TEAM1SCORE]);
					else
						statustext = sprintf(HIC"%-2d"NOR":"CYN"%2d"NOR, game[TEAM2SCORE], game[TEAM1SCORE]);
						   
					statustext += sprintf(" "WHT"%s"HIW"%d"NOR, (game[INNING]%2?"��":"��"), (game[INNING]/2+game[INNING]%2));
					break;	
				}
				case STATUS_PREPARING:
				{
					mapping game = BASEBALL_D->query_game(id);
					statustext = HIY"��"NOR YEL"�Ƥ�"NOR+" "+HIY+((PREPARETIME - game[TICK])/60+1)+NOR YEL+"m"NOR;
					break;	
				}
			}
			msg += ({ sprintf(" %-4s%-13s"HIC"%-13s"HIW"%4d"HIR"%5d"HIG"%5d"HIY+(win>0&&lose==0?"%6.1f":"%6.2f")+NOR CYN" %-12s"NOR"%s", sep == 0 ? to_string(++rank):"", setup[id]["name"], capitalize(id), win+lose, win, lose, lose+win > 0. ? (win*100./(lose+win)) : 0., opponent ? capitalize(opponent) : "", statustext) });
		}
		
		msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
		
		if( !undefinedp(setup[myid]) )
		{
			if( setup[myid]["keepwins"] > 1 )
				msg += ({ pnoun(2, me)+"���y���̪񥴥X "HIR+setup[myid]["keepwins"]+NOR" �s�ӡA�h�𰪩�" });
			if( setup[myid]["keeploses"] > 1 )
				msg += ({ pnoun(2, me)+"���y���̪�W�] "HIG+setup[myid]["keeploses"]+NOR" �s�ѡA�h��C��" });
		}

		if( BASEBALL_D->is_in_post_season() )
			msg += ({ " �ثe���b�i��� "HIC+year+NOR" �y�u�u����" });
		else 
		{
			int eta = 1282456800 + 604800*year - time();
			
			if( eta < 0 )
				msg += ({ " �� "HIC+year+NOR" �y�u�u���ɷǳƤ�"NOR });
			else
				msg += ({ " �Z���� "HIC+year+NOR" �y�u�u�����٦� "HIY+(eta/60/60/24)+" �� "+(eta/60/60%24)+" �p�� "+(eta/60%60)+" ��"NOR });
		}
			
		msg += ({ " �d�߲y���Ʀ���� / ��ť�U�y���Y�ɾԪp / �U�F�t�� - help baseball" });
		msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
		return tell(me, implode(msg, "\n")+"\n");
	}
	else if( sscanf(arg, "record %s", value) == 1 )
	{
		string *recordsort;
		mapping record = BASEBALL_D->query_record();
		mapping playerrecord;
		int count;
		
		object labor;
		
		sscanf(value, "%s %d", value, year);
		
		if( !mapp(record[year]) )
			return tell(me, "�èS���� "+year+" �y�u�������C\n");
			
		playerrecord = record[year][RECORD_PLAYER];
			
		msg += ({ " �� "HIC+year+NOR" �y�u�Ҧ��y�������ƦW�p�U�G" });
		
		switch(lower_case(value))
		{
			case "home":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW ���S����                    �y������      ���"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["hit4"] > 0 :)), (: $(playerrecord)[$1]["hit4"] > $(playerrecord)[$2]["hit4"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["hit4"]) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "hit":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW �w����                      �y������      ���       �����v"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["hit"] > 0 :)), (: $(playerrecord)[$1]["hit"] > $(playerrecord)[$2]["hit"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"HIY"%13.2f"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["hit"], 10.*playerrecord[file]["hit"]/playerrecord[file]["ab"]) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "hit2":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW �G�S�w����                  �y������      ���       �����v"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["hit2"] > 0 :)), (: $(playerrecord)[$1]["hit2"] > $(playerrecord)[$2]["hit2"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"HIY"%13.2f"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["hit2"], 10.*playerrecord[file]["hit2"]/playerrecord[file]["ab"]) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "hit3":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW �T�S�w����                  �y������      ���       �����v"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["hit3"] > 0 :)), (: $(playerrecord)[$1]["hit3"] > $(playerrecord)[$2]["hit3"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"HIY"%13.2f"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["hit3"], 10.*playerrecord[file]["hit3"]/playerrecord[file]["ab"]) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "run":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW ���I��                      �y������      ���I"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["rbi"] > 0 :)), (: $(playerrecord)[$1]["rbi"] > $(playerrecord)[$2]["rbi"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["rbi"]) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "k":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW �T����                      �y������      �T��       �T���v"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["strikeout"] > 0 :)), (: $(playerrecord)[$1]["strikeout"] > $(playerrecord)[$2]["strikeout"] ? -1 : 1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"HIY"%12.2f%%"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["strikeout"], playerrecord[file]["bf"]>0?100.*playerrecord[file]["strikeout"]/playerrecord[file]["bf"]:0.) });
					
					if( --count == 0 ) break;
				}
				
				break;
			}
			case "era":
			{
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �ƦW ���m�v��                    �y������       ����(>45)     �۳d���v"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				
				recordsort = sort_array( filter_array(keys(playerrecord), (: $(playerrecord)[$1]["out"] > 135 :)), (: to_float($(playerrecord)[$1]["run"])/$(playerrecord)[$1]["out"] > to_float($(playerrecord)[$2]["run"])/$(playerrecord)[$2]["out"] ? 1 : -1 :) );
					
				count = RECORD_SIZE;
				foreach(string file in recordsort)
				{
					if( !file_exists(file) ) continue;
						
					labor = load_object(file);
					
					if( !query("boss", labor) ) continue;
						
					msg += ({ sprintf(" "HIW"%-5d"NOR"%-28s"HIC"%-12s"HIY"%6d"HIY"%19.2f"NOR, RECORD_SIZE+1-count, labor->query_idname(), capitalize(query("boss", labor)), playerrecord[file]["out"]/3, playerrecord[file]["out"]>0?27.*playerrecord[file]["run"]/playerrecord[file]["out"]:0.) });
					
					if( --count == 0 ) break;
				}
		
				break;
			}
			case "special":
			{
				mapping specialrecord = record[year][RECORD_SPECIAL];
				
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
				msg += ({ HIW" �S�����             ����   �F������ / �y��               ����"NOR });
				msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });

				if( stringp(specialrecord["champion"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["champion"]);
					msg += ({ sprintf(NOR YEL" �y�u�`�a�x                  %-30s"HIC"%s"NOR, mapp(setup) ? setup["name"] : specialrecord["champion"], capitalize(specialrecord["champion"])) });
				}
				
				if( stringp(specialrecord["2rd"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["2rd"]);
					msg += ({ sprintf(NOR YEL" �y�u�ȭx                    %-30s"HIC"%s"NOR, mapp(setup) ? setup["name"] : specialrecord["2rd"], capitalize(specialrecord["2rd"])) });
					msg += ({ "" });
				}
				
				if( stringp(specialrecord["keepwinsteam"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["keepwinsteam"]);
					msg += ({ sprintf(NOR YEL" �̪��s�ӳ���        "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["keepwins"], mapp(setup) ? setup["name"] : specialrecord["keepwinsteam"], capitalize(specialrecord["keepwinsteam"])) });
				}
				
				if( stringp(specialrecord["keeplosesteam"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["keeplosesteam"]);
					msg += ({ sprintf(NOR YEL" �̪��s�ѳ���        "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["keeploses"], mapp(setup) ? setup["name"] : specialrecord["keeplosesteam"], capitalize(specialrecord["keeplosesteam"])) });
				}
				
				if( stringp(specialrecord["maxstrikeoutsplayer"]) )
				{
					object player;
					
					if( file_exists(specialrecord["maxstrikeoutsplayer"]) )
						player = load_object(specialrecord["maxstrikeoutsplayer"]);
						
					msg += ({ sprintf(NOR YEL" ����̦h�T������    "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxstrikeouts"], objectp(player) ? player->query_idname() : "�w����", objectp(player) ? capitalize(query("boss", player)) : "") });
				}
				if( stringp(specialrecord["maxhomerundistanceplayer"]) )
				{
					object player;
					
					if( file_exists(specialrecord["maxhomerundistanceplayer"]) )
						player = load_object(specialrecord["maxhomerundistanceplayer"]);
						
					msg += ({ sprintf(NOR YEL" ���S���̪��Z��      "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxhomerundistance"], objectp(player) ? player->query_idname() : "�w����", objectp(player) ? capitalize(query("boss", player)) : "") });
				}
				if( stringp(specialrecord["maxhitsteam"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["maxhitsteam"]);
					msg += ({ sprintf(NOR YEL" ����̦h�w����      "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxhits"], mapp(setup) ? setup["name"] : specialrecord["maxhitsteam"], capitalize(specialrecord["maxhitsteam"])) });
				}
				
				if( stringp(specialrecord["maxscoresteam"]) )
				{
					mapping setup = BASEBALL_D->query_setup(specialrecord["maxscoresteam"]);
					msg += ({ sprintf(NOR YEL" ����̦h�o����      "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxscores"], mapp(setup) ? setup["name"] : specialrecord["maxscoresteam"], capitalize(specialrecord["maxscoresteam"])) });
				}

				if( stringp(specialrecord["maxinningsteam1"]) && stringp(specialrecord["maxinningsteam2"]) )
				{
					msg += ({ sprintf(NOR YEL" �����ɳ̪�����      "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxinnings"]/2+specialrecord["maxinnings"]%2, (mapp(BASEBALL_D->query_setup(specialrecord["maxinningsteam1"])) ? BASEBALL_D->query_setup(specialrecord["maxinningsteam1"])["name"] : specialrecord["maxinningsteam1"])+" & "+(mapp(BASEBALL_D->query_setup(specialrecord["maxinningsteam2"])) ? BASEBALL_D->query_setup(specialrecord["maxinningsteam2"])["name"] : specialrecord["maxinningsteam2"]), capitalize(specialrecord["maxinningsteam1"]) +" & "+capitalize(specialrecord["maxinningsteam2"])) });
				}
				
				if( stringp(specialrecord["maxscorediffwin"]) && stringp(specialrecord["maxscoredifflose"]) )
				{
					msg += ({ sprintf(NOR YEL" ������Ƴ̤j�t�Z    "HIY"%5d"NOR"   %-30s"HIC"%s"NOR, specialrecord["maxscorediff"], (mapp(BASEBALL_D->query_setup(specialrecord["maxscorediffwin"])) ? BASEBALL_D->query_setup(specialrecord["maxscorediffwin"])["name"] : specialrecord["maxscorediffwin"])+" & "+(mapp(BASEBALL_D->query_setup(specialrecord["maxscoredifflose"])) ? BASEBALL_D->query_setup(specialrecord["maxscoredifflose"])["name"] : specialrecord["maxscoredifflose"]), capitalize(specialrecord["maxscorediffwin"]) +" & "+capitalize(specialrecord["maxscoredifflose"])) });
				}
				break;
			}
			default:
				return tell(me, "�п�X���T����������(home, hit, hit2, hit3, run, k, era, special)�C\n");
		}
		msg += ({ WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
		
		return me->more(implode(msg, "\n")+"\n");
	}
	else if( arg == "info" || sscanf(arg, "info %s", value) == 1 )
	{
		string target;
		mapping setup;
		string text;
		object labor;
		string position;
		string file;
		int win;
		int lose;
		int out, bf, run, strikeout;
		int handside = HANDSIDE_NONE;
		string handsidename;
		
		if( value )
			target = value;
		else
			target = myid;
			
		if( !BASEBALL_D->exists(target) )
			return tell(me, "�S�� "+target+" �o�쪱�a�A�θӪ��a�å��إ߲y���C\n");

		setup = BASEBALL_D->query_setup(target);

		text = (target == myid ? pnoun(2, me) : find_player(target) ? find_player(target)->query_idname() : capitalize(target)+" ")+"���y���W�٬��u"+(setup["name"]||"���R�W")+"�v���A�ثe�w�ƪ��y���u�Ʀ�m�P�����Φ���Ʀp�U�G\n";
		
		text += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		text += sprintf(HIW"%-4s %-9s%5s%5s%5s%5s%10s%10s %s\n"NOR, "", "�u�Ʀ�m", "�۳d", "����", "�|�a", "�T��", "�T���v", "��V", "�y���W��" );
		text += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		labor = 0;
		position = 0;

		if( mapp(setup["roster"][0]) )
		{
			file = setup["roster"][0]["file"];
			
			if( file_exists(file) )
				labor = load_object(file);

			handside = BASEBALL_D->get_handside_type(labor);
			position = BASEBALL_D->query_positionname(setup["roster"][0]["position"]);
		}

		switch(handside)
		{
			case HANDSIDE_NONE: handsidename = ""; break;
			case HANDSIDE_LEFTHAND: handsidename = HIC"��"NOR CYN"��"NOR; break;
			case HANDSIDE_RIGHTHAND: handsidename = HIG"�k"NOR GRN"��"NOR; break;
			case HANDSIDE_TWOHANDS: handsidename = "���~"; break;
		}
		
		out = BASEBALL_D->get_record(RECORD_PLAYER, file, "out", year);
		bf = BASEBALL_D->get_record(RECORD_PLAYER, file, "bf", year);
		run = BASEBALL_D->get_record(RECORD_PLAYER, file, "run", year);
		strikeout = BASEBALL_D->get_record(RECORD_PLAYER, file, "strikeout", year);
		
		text += sprintf(" %-3s %-9s"HIG"%5.2f"HIC"%5d"HIM"%5d"HIY"%5d"HIY"%9.2f%%"NOR"%10s %s\n", 
			"",
			position || WHT"���]�w"NOR, 
			out > 0 ? run * 9. / (out / 3.): 0.,
			out / 3,
			BASEBALL_D->get_record(RECORD_PLAYER, file, "fourball", year),
			strikeout,
			bf > 0 ? 100. * strikeout / bf : 0.,
			handsidename,
			objectp(labor) ? labor->query_idname() : WHT"���]�w"NOR
		);
			
		text += WHT"\n"NOR;
		text += sprintf(HIW"%-4s %-9s%5s%5s%5s%5s%5s%5s%5s%5s %s\n"NOR, "�Φ�", "�u�Ʀ�m", "���v", "�w��", "�G�w", "�T�w", "���S", "���I", "�O�e", "��V", "�y���W��");
		text += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		for(int i=1;i<10;++i)
		{
			labor = 0;
			position = 0;
			handside = HANDSIDE_NONE;
			
			if( mapp(setup["roster"][i]) )
			{
				file = setup["roster"][i]["file"];
				
				if( file_exists(file) )
					labor = load_object(file);

				handside = BASEBALL_D->get_handside_type(labor);
				position = BASEBALL_D->query_positionname(setup["roster"][i]["position"]);
			}

			switch(handside)
			{
				case HANDSIDE_NONE: handsidename = ""; break;
				case HANDSIDE_LEFTHAND: handsidename = HIC"��"NOR CYN"��"NOR; break;
				case HANDSIDE_RIGHTHAND: handsidename =HIG"�k"NOR GRN"��"NOR; break;
				case HANDSIDE_TWOHANDS: handsidename = HIY"��"NOR YEL"�k"NOR; break;
			}
		
			text += sprintf(" %-3s %-9s"HIG"%5.2f"HIC"%5d"HIY"%5d"HIY"%5d"HIY"%5d"HIW"%5d"HIM"%5d"NOR" %4s %s\n", 
				HIG+i+NOR, 
				position || WHT"���]�w"NOR, 
				BASEBALL_D->get_hit_rate(target, i, year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "hit", year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "hit2", year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "hit3", year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "hit4", year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "rbi", year),
				BASEBALL_D->get_record(RECORD_PLAYER, file, "walk", year),
				handsidename,
				objectp(labor) ? labor->query_idname() : WHT"���]�w"NOR
			);
		}
	
		text += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		for(int y=year;y>=1;--y)
		{
			win = BASEBALL_D->get_record(RECORD_TEAM, target, "win", y);
			lose = BASEBALL_D->get_record(RECORD_TEAM, target, "lose", y);
		
			text += WHT"�� "HIW+y+NOR WHT" �y�u"NOR"�u�ɳӳ��ơG"HIR+win+NOR" / �ѳ��ơG"HIG+lose+NOR" / �Ӳv�G"HIY+sprintf("%.2f", win+lose > 0 ? 100.*win/(win+lose) : 0.)+NOR YEL"%"NOR"\n";
		}
		text += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		me->more(text);
		
		return;
	}
	else if( sscanf(arg, "listen %s", arg) == 1 )
	{
		if( arg == "-d" )
		{
			BASEBALL_D->set_listener(myid, 0);
			return tell(me, pnoun(2, me)+"���ť�Ҧ����ɹ�p�C\n");
		}
		
		if( !arg || !arg[0] )
			return tell(me, "�п�J���T�����O�榡�C\n");

		if( !BASEBALL_D->in_season(arg) )
			return tell(me, arg+" �èS���y���[�J�u�ɡC\n");
			
		BASEBALL_D->set_listener(myid, arg);
		
		return tell(me, pnoun(2, me)+"�}�l��ť "+arg+" ���y�����ɹ�p�C\n");
	}
	else if( sscanf(arg, "hint %s", arg) == 1 )
	{
		string *signals = ({ NOR WHT"|"HIW"o"NOR WHT"|"NOR, NOR WHT"\\"HIW"o"NOR WHT"\\"NOR, NOR WHT"/"HIW"o"NOR WHT"/"NOR, NOR WHT"\\"HIW"o"NOR WHT"/"NOR, NOR WHT"-"HIW"o"NOR WHT"-"NOR  });
		string signal;
		mapping setup;
		mapping game;
		
		if( BASEBALL_D->query_status(myid) != STATUS_PLAYING )
			return tell(me, pnoun(2, me)+"���y���ثe�å��i��y�ɡC\n");
		
		setup = BASEBALL_D->query_setup(myid);
				
		if( setup["hint"] > 0 )
			return tell(me, pnoun(2, me)+"�e�@���U�F���t���ثe���M���ġA�L�k���ФU�F�t���C\n");

		game = BASEBALL_D->query_game(myid);
		
		signal = signals[random(5)]+"  "+signals[random(5)]+"  "+signals[random(5)];

		switch(arg)
		{
			case "batter":
				
				if( setup["hinttimes"] <= 0 ) 
					return tell(me, pnoun(2, me)+"�o���y�ɪ��t���U�F���Ƥw�F�W���C\n");
			
				if( ((game[INNING]%2) > 0 && game[TEAM2] == myid) || ((game[INNING]%2) == 0 && game[TEAM1] == myid) )
				{
					tell(me, pnoun(2, me)+"�U�F�t�����R���b���������̡A�o�@�����u�������¤O�u�ȴ��ɡC\n");
					BASEBALL_D->add_message(myid, me->query_idname()+"��i�a��ۥ��̤�F�T�ӳs���̳��ݤ���������աu "+signal+" �v");
					setup["hint"] = HINTTYPE_BATTER;
					
					setup["hinttimes"]--;
				}
				else
					return tell(me, "�ثe"+pnoun(2, me)+"�O���u��A�L�k�若�̤U�F�t���C\n");

				break;
			case "bunt":
				if( (game[INNING]%2) > 0 && game[TEAM2] == myid || ((game[INNING]%2) == 0 && game[TEAM1] == myid) )
				{
					tell(me, pnoun(2, me)+"�U�F�t�������b���������̡A�n�D��i��Ĳ���u���H���@�S�W�]�̱��i�C\n");
					BASEBALL_D->add_message(myid, me->query_idname()+"��i�a��ۥ��̤�F�T�ӳs���̳��ݤ���������աu "+signal+" �v");
					setup["hint"] = HINTTYPE_BUNT;
				}
				else
					return tell(me, "�ثe"+pnoun(2, me)+"�O���u��A�L�k�若�̤U�F�t���C\n");
					
				break;
			case "pitcher":
				
				if( setup["hinttimes"] <= 0 ) 
					return tell(me, pnoun(2, me)+"�o���y�ɪ��t���U�F���Ƥw�F�W���C\n");
			
				if( (game[INNING]%2) == 0 && game[TEAM2] == myid || ((game[INNING]%2) > 0 && game[TEAM1] == myid) )
				{
					tell(me, pnoun(2, me)+"�U�F�t�����R���b��y�����A����ثe���̪���y�¤O�u�ȴ��ɡC\n");
					BASEBALL_D->add_message(myid, me->query_idname()+"��i�a��ۧ���F�T�ӳs��ⳣ�ݤ���������աu "+signal+" �v");
					setup["hint"] = HINTTYPE_PITCHER;
					
					setup["hinttimes"]--;
				}
				else
					return tell(me, "�ثe"+pnoun(2, me)+"�O������A�L�k����U�F�t���C\n");
					
				break;
			case "walk":
				if( (game[INNING]%2) == 0 && game[TEAM2] == myid || ((game[INNING]%2) > 0 && game[TEAM1] == myid) )
				{
					tell(me, pnoun(2, me)+"�U�F�t�����R���b��y�����A�n�D��q���|�a�O�e�ثe���̡C\n");
					BASEBALL_D->add_message(myid, me->query_idname()+"��i�a��ۧ���F�T�ӳs��ⳣ�ݤ���������աu "+signal+" �v");
					setup["hint"] = HINTTYPE_WALK;
				}
				else
					return tell(me, "�ثe"+pnoun(2, me)+"�O������A�L�k����U�F�t���C\n");
					
				break;
	
			default:
				return tell(me, "�S�� "+arg+" �o�طt�������C\n");
		}
		
		
		
		if( setup["hinttimes"] == 0 )
			tell(me, pnoun(2, me)+"�o���y�ɪ��t���U�F���Ƥw�F�W���C\n");
		else
			tell(me, pnoun(2, me)+"�o���y�ɳѤU "+setup["hinttimes"]+" ���U�F�t�������|�C\n");

		BASEBALL_D->set_setup(myid, setup);
		return;
	}
			
	return tell(me, "�п�J���T�����O�榡(help baseball)�C\n");
}
