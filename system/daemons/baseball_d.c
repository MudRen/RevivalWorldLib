/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : baseball_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2010-08-14
 * Note   : �βy�t��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <npc.h>
#include <time.h>
#include <skill.h>
#include <baseball.h>
#include <equipment.h>
#include <buff.h>

#define DATA		"/data/daemon/baseball.o"
#define BACKUP_DATA	"/data/daemon/baseball.o.bak"

string *season;
mapping setup;
mapping record;
array games;
private nosave int tick;
private nosave int max_game_times;
int post_season_level;
int post_season_start;
string *post_season_team_8;
string *post_season_team_4;
string *post_season_team_2;
string *post_season_eliminate_teams;
int season_year;

private varargs string *game_finish(int gameindex, string stopid);

private nosave mapping positionname = 
([
	"P" : HIC"��"NOR CYN"��"NOR,
	"C" : HIG"��"NOR GRN"��"NOR,
	"1B" : HIG"�@"NOR GRN"�S��"NOR,
	"2B" : HIG"�G"NOR GRN"�S��"NOR,
	"3B" : HIG"�T"NOR GRN"�S��"NOR,
	"SS" : HIG"��"NOR GRN"����"NOR,
	"RF" : HIY"�k"NOR YEL"�~����"NOR,
	"CF" : HIY"��"NOR YEL"�~����"NOR,
	"LF" : HIY"��"NOR YEL"�~����"NOR,
	"DH" : HIR"��"NOR RED"�w����"NOR,
]);
private nosave string *pitch_ball_types = ({ "fourseam", "twoseam", "curveball", "slider", "forkball", "sinker" });
private nosave mapping pitch_ball_types_name = 
([
	"fourseam":"�|�_�u�ֳt�y",
	"twoseam":"�G�_�u�ֳt�y",
	"curveball":"���y",
	"slider":"�Ʋy",
	"forkball":"���e�y",
	"sinker":"���d�y",
]);

// �վ��O�ƭȡA�i�����ܤƲv�í���b�@�w�d��
int calculate(int value, float decay, int value_max, int to_min, int to_max)
{
	float ratio = (to_max - to_min) / pow(to_float(value_max), decay);
	
	value = to_int( pow(to_float(value), decay) * ratio );
	
	value += to_min;
	
	return value;
}

int calculate_max_game_times(int year)
{
	if( !mapp(record[year]) ) return 0;
		
	max_game_times = 0;
	
	foreach(string team, mapping data in record[year][RECORD_TEAM])
		if( data["win"] + data["lose"] > max_game_times )
			max_game_times = data["win"] + data["lose"];
			
	return max_game_times;
}

int sort_team(string team1, string team2)
{
	int year = season_year;
	int win1, win2, lose1, lose2;
	int limit = (max_game_times-10)/2;
	float winrate1, winrate2;
	
	if( !mapp(record[year]) ) return 0;
		
	if( !mapp(record[year][RECORD_TEAM][team1]) )
		return 1;
		
	if( !mapp(record[year][RECORD_TEAM][team2]) )
		return -1;
		
	win1 = record[year][RECORD_TEAM][team1]["win"];
	win2 = record[year][RECORD_TEAM][team2]["win"];
	lose1 = record[year][RECORD_TEAM][team1]["lose"];
	lose2 = record[year][RECORD_TEAM][team2]["lose"];
	
	if( win1+lose1 == 0 )
		return 1;
	
	if( win2+lose2 == 0 )
		return -1;
		
	winrate1 = to_float(win1)/(win1+lose1);
	winrate2 = to_float(win2)/(win2+lose2);
	
	if( limit < 10 ) limit = 10;
		
	if( win1+lose1 < limit )
	{
		if( win2+lose2 <= limit )
			return winrate1 > winrate2 ? -1 : 1;
				
		return 1;
	}
	
	if( win2+lose2 < limit )
		return -1;

	if( winrate1 == winrate2 )
		return win1 > win2 ? -1 : 1;
	else
		return winrate1 > winrate2 ? -1 : 1;
}

string query_positionname(string code)
{
	return positionname[code];	
}

int save()
{
	save_object(DATA);
}

int backup()
{
	return cp(DATA, BACKUP_DATA);
}

varargs void broadcast(string msg, mapping game)
{
	if( post_season_start && post_season_level > 0 )
	{
		CHANNEL_D->channel_broadcast("sport", msg);
	}
	else
	{
		object user;
		
		foreach(string listener in setup[game[TEAM1]]["listener"] | setup[game[TEAM2]]["listener"] )
			if( objectp(user = find_player(listener)) )
				tell(user, HIG"�i���ɡj"NOR+msg+"\n");
	}
}

// �y���βy������
mixed add_record(int recordtype, string id, string key, mixed value)
{
	int year = season_year;
	
	if( undefinedp(record[year]) )
	{
		record[year] = allocate_mapping(0);
		record[year][RECORD_TEAM] = allocate_mapping(0);
		record[year][RECORD_PLAYER] = allocate_mapping(0);
		record[year][RECORD_SPECIAL] = allocate_mapping(0);
	}
	
	if( undefinedp(record[year][recordtype][id]) )
		record[year][recordtype][id] = allocate_mapping(0);
		
	if( intp(value) || floatp(value) )
		record[year][recordtype][id][key] += value;
	else
		record[year][recordtype][id][key] = value;
	
	return record[year][recordtype][id][key];
}

varargs mixed get_record(int recordtype, string id, string key, int year)
{
	if( undefinedp(year) )
		year = season_year;
	
	if( undefinedp(record[year]) ||  undefinedp(record[year][recordtype][id]) ) return 0;
		
	return record[year][recordtype][id][key];
}

// �W�[�y������
void add_player_record(string id, int number, string key, int value)
{
	string player = setup[id]["roster"][number]["file"];

	add_record(RECORD_PLAYER, player, key, value);
}

// �W�[�y������
void add_team_record(string id, string key, int value)
{
	add_record(RECORD_TEAM, id, key, value);
}

// �T�{�y���]�w�X�k
varargs int valid_setup(string id, int all)
{
	object labor;
	object env;
	mapping roster_data;
	object boss = find_player(id);
	int fldaccuracy, fldrange;
	
	if( !mapp(setup[id]) ) return 0;	
	
	if( objectp(boss) )
	{
		setup[id]["username"] = boss->query_idname();
	}
	else if( !stringp(setup[id]["username"]) )
	{
		boss = load_user(id);
			
		setup[id]["username"] = boss->query_idname();	
			
		if( !userp(boss) )
			destruct(boss);	
	}
	
	if( !stringp(setup[id]["name"]) ) return 0;

	if( all )
	{
		setup[id]["outfield"] = 0;
		setup[id]["infield"] = 0;
	}
	
	for(int i=0;i<10;++i)
	{
		roster_data = setup[id]["roster"][i];
		
		if( !mapp(roster_data) )
			return 0;
			
		if( !objectp(labor = roster_data["object"]) )
		{
			if( !file_exists(roster_data["file"]) || !objectp(labor = load_object(roster_data["file"])) )
			{
				setup[id]["roster"][i] = 0;
				return 0;
			}

			roster_data["object"] = labor;
		}

		if( i==0 && labor->query_skill_level("twohands") > 0 )
			return 0;

		env = environment(labor);
		
		if( !objectp(env) || !env->is_module_room() || env->query_building_type() != "baseball" || query("owner", env) != query("boss", labor) || query("job/type", labor) != SPORTER )
		{
			setup[id]["roster"][i] = 0;
			return 0;
		}
		
		if( all )
		{
			roster_data[BUFF_BATTER_POWER] = labor->query_equipment_buff(BUFF_BATTER_POWER);
			roster_data[BUFF_PITCHER_POWER] = labor->query_equipment_buff(BUFF_PITCHER_POWER);
			roster_data[BUFF_FIELDER_POWER] = labor->query_equipment_buff(BUFF_FIELDER_POWER);

			fldaccuracy = labor->query_skill_level("fldaccuracy") + roster_data[BUFF_FIELDER_POWER];
			fldrange = labor->query_skill_level("fldrange") + roster_data[BUFF_FIELDER_POWER];
				
			switch(roster_data["position"])
			{
				case "P": setup[id]["infield"] += fldaccuracy * 4 / 5 + fldrange * 1 / 5 + labor->query_agi()/2; break;
				case "C": setup[id]["infield"] += fldaccuracy * 4 / 5 + fldrange * 1 / 5 + labor->query_agi()/2; break;
				case "1B": setup[id]["infield"] += fldaccuracy * 2 / 3 + fldrange * 1 / 3 + labor->query_agi()/2; break;
				case "2B": setup[id]["infield"] += fldaccuracy * 2 / 3 + fldrange * 1 / 3 + labor->query_agi()/2; break;
				case "3B": setup[id]["infield"] += fldaccuracy * 2 / 3 + fldrange * 1 / 3 + labor->query_agi()/2; break;
				case "SS": setup[id]["infield"] += fldaccuracy * 2 / 3 + fldrange * 1 / 3 + labor->query_agi()/2; break;
				case "RF": setup[id]["outfield"] += fldaccuracy * 1 / 4 + fldrange * 3 / 4 + labor->query_agi()/2; break;
				case "CF": setup[id]["outfield"] += fldaccuracy * 1 / 4 + fldrange * 3 / 4 + labor->query_agi()/2; break;
				case "LF": setup[id]["outfield"] += fldaccuracy * 1 / 4 + fldrange * 3 / 4 + labor->query_agi()/2; break;
				case "DH": break;
			}
			
			if( i == 9 )
			{
				setup[id]["outfield"] = calculate(setup[id]["outfield"], 0.8, 999*3, 500, 1000);
				setup[id]["infield"] = calculate(setup[id]["infield"], 0.8, 999*6, 500, 1000);
			}	
		}
	}
	
	return 1;
}

// �p��y���C�������������J
int query_income(string id)
{
	int year = season_year;
	int win, lose;
	
	if( !valid_setup(id) || !mapp(record[year]) || !mapp(record[year][RECORD_TEAM][id]) ) 
		return 0;	
	
	win = record[year][RECORD_TEAM][id]["win"];
	lose = record[year][RECORD_TEAM][id]["lose"];

	if( win + lose <= 0 ) return 0;

	return BASEINCOME * win / (win + lose);
}

string query_player_status(object player)
{
	string id = query("boss", player);
	mapping data;
	
	if( !undefinedp(setup[id]) && arrayp(setup[id]["roster"]) )
	{
		for(int i=0;i<10;++i)
		{
			data = setup[id]["roster"][i];
			
			if( !mapp(data) ) continue;

			if( data["file"] == base_name(player) )
				return positionname[data["position"]]+" "+(i > 0 ? GRN"�� "HIG+i+NOR GRN" �� "NOR:"");
		}	
	}
	
	return "";
}

// �y���]�w
void set_setup(string id, mapping newsetup)
{
	setup[id] += newsetup;
	
	save();
}

// �R���y���]�w
void delete_setup(string id)
{
	map_delete(setup, id);
	
	save();
}

// �[�J�u��
void join_season(string id)
{
	int index;
	
	// �w�g�[�J�u��
	if( member_array(id, season) != -1 ) return;
		
	season = sort_array(season, (: random(2) ? 1 : -1 :));

	foreach(string oldid in season)
	{
		index = random(sizeof(setup[oldid]["opponents"]));
	
		if( member_array(id, setup[oldid]["opponents"]) == -1 )
			setup[oldid]["opponents"] = setup[oldid]["opponents"][0..index] + ({ id }) + setup[oldid]["opponents"][index+1..];
	}
	
	if( !sizeof(setup[id]["opponents"]) )
		setup[id]["opponents"] = copy(sort_array(season, (: random(2) ? 1 : -1 :)));

	setup[id]["listener"] |= ({ id });

	season |= ({ id });		
}

// ��������
varargs void cancel_game(string id, int force)
{
	int gamessize = sizeof(games);
	
	for(int i=0;i<gamessize;++i)
	{
		if( !mapp(games[i]) ) continue;
		
		if( games[i][TEAM1] == id || games[i][TEAM2] == id )
		{
			if( force )
			{
				if( games[i][TEAM1] == id )
					game_finish(i, id);
				else if( games[i][TEAM2] == id )
					game_finish(i, id);
			}
			else
			{
				setup[games[i][TEAM1]]["status"] = STATUS_IDLE;
				setup[games[i][TEAM2]]["status"] = STATUS_IDLE;
				
				games[i] = 0;
			}
		}
	}
	
	games -= ({ 0 });
}

// �h�X�u��
void leave_season(string id)
{
	if( member_array(id, season) == -1 ) return;

	foreach(string oldid in season)
		setup[oldid]["opponents"] -= ({ id });

	cancel_game(id);

	season -= ({ id });
}

// �O�_�[�J�u��
int in_season(string id)
{
	return member_array(id, season) != -1;
}

// �O�_���b���ɤ�
int query_status(string id)
{
	if( !in_season(id) ) return 0;
		
	return setup[id]["status"];
}

// ���ɵ���
private varargs string *game_finish(int gameindex, string stopid)
{
	int year = season_year;
	string *msg = allocate(0);
	mapping game = games[gameindex];
	string team1name = setup[game[TEAM1]]["name"];
	string team2name = setup[game[TEAM2]]["name"];
	string income_msg = HIG"���J"NOR GRN"���t"NOR;
	string finalscore;
	string winteam, loseteam;
	
	if( !undefinedp(stopid) && stringp(stopid) )
	{
		if( game[TEAM1] == stopid )
			game[TEAM1SCORE] = -1;
		else if( game[TEAM2] == stopid )
			game[TEAM2SCORE] = -1;	
	}

	if( game[TEAM1SCORE] > game[TEAM2SCORE] )
	{
		winteam = game[TEAM1];
		loseteam = game[TEAM2];
	}
	else
	{
		winteam = game[TEAM2];
		loseteam = game[TEAM1];
	}
	finalscore = HIG"����"NOR GRN"���G"NOR" ����"+(game[INNING]>18?"�@�@�񰫤F "+(game[INNING]/2+game[INNING]%2)+" ���A":"")+"�����"+setup[game[TEAM2]]["username"]+"�u"+team2name+" "+game[TEAM2SCORE]+" : "+game[TEAM1SCORE]+" "+team1name+"�v"+setup[game[TEAM1]]["username"];
	
	switch(game[TEAM1SCORE] - game[TEAM2SCORE])
	{
		case -999..-16:		msg += ({ finalscore+"�A�i�����u"+team1name+"�v�D��F���ƪ���{�O��" });	break;
		case -15..-11:		msg += ({ finalscore+"�A�u"+team2name+"�v�������۰d�F���" });			break;
		case -10..-6:		msg += ({ finalscore+"�A�u"+team2name+"�v���d�C���a�h�_�F���" });		break;
		case -5..-3:		msg += ({ finalscore+"�A�u"+team2name+"�v�b�o�����ɤ����P���" });		break;
		case -2..-1:		msg += ({ finalscore+"�A�u"+team2name+"�v�b���I�����" });			break;
		case 1..2:		msg += ({ finalscore+"�A�u"+team1name+"�v�b���I�����" });			break;
		case 3..5:		msg += ({ finalscore+"�A�u"+team1name+"�v�b�o�����ɤ����P���" });		break;
		case 6..10:		msg += ({ finalscore+"�A�u"+team1name+"�v���d�C���a�h�_�F���" });		break;
		case 11..15:		msg += ({ finalscore+"�A�u"+team1name+"�v�������۰d�F���" });			break;
		case 16..999:		msg += ({ finalscore+"�A�i�����u"+team2name+"�v�D��F���ƪ���{�O��" });	break;
		//default: error("error score");
	}
	
	if( !post_season_level )
	{	
		add_team_record(winteam, "win", 1);
		add_team_record(loseteam, "lose", 1);

		if( setup[winteam]["keeploses"] >= 5 )
		{
			msg += ({ "�u"+setup[winteam]["name"]+"�v����F�̪� "HIG+setup[winteam]["keeploses"]+NOR" �s�ѡC" });
		}
		if( setup[loseteam]["keepwins"] >= 5 )
		{
			msg += ({ "�u"+setup[loseteam]["name"]+"�v����F�̪� "HIR+setup[loseteam]["keepwins"]+NOR" �s�ӡC" });
		}
			
		setup[winteam]["keepwins"]++;
		setup[winteam]["keeploses"] = 0;
		
		setup[loseteam]["keepwins"] = 0;
		setup[loseteam]["keeploses"]++;
		
		if( setup[winteam]["keepwins"] > record[year][RECORD_SPECIAL]["keepwins"] )
		{
			record[year][RECORD_SPECIAL]["keepwins"] = setup[winteam]["keepwins"];
			record[year][RECORD_SPECIAL]["keepwinsteam"] = winteam;
			
			msg += ({ HIY"����"NOR YEL"��s "NOR"�h�𰪩����u"+setup[winteam]["name"]+"�v��}���u�̪��s�Ӭ����A"HIR"�s��"NOR RED"����"NOR"�Ө�� "HIR+setup[winteam]["keepwins"]+NOR" ��" });
		}
		else if( setup[winteam]["keepwins"] >= 5 )
		{
			msg += ({ "�u"+setup[winteam]["name"]+"�v���X�@�i "HIR+setup[winteam]["keepwins"]+NOR" �s�ӡA���Z�j�T�D�i" });
		}
		
		if( setup[loseteam]["keeploses"] > record[year][RECORD_SPECIAL]["keeploses"] )
		{
			record[year][RECORD_SPECIAL]["keeploses"] = setup[loseteam]["keeploses"];
			record[year][RECORD_SPECIAL]["keeplosesteam"] = loseteam;
			
			msg += ({ HIY"����"NOR YEL"��s "NOR"�h��C�����u"+setup[loseteam]["name"]+"�v�W�]���u�̪��s�Ѭ����A"HIG"�s��"NOR GRN"����"NOR"�Ө�� "HIG+setup[loseteam]["keeploses"]+NOR" ��" });
		}
		else if( setup[loseteam]["keeploses"] >= 5 )
		{
			msg += ({ "�u"+setup[loseteam]["name"]+"�v�s��]�U "HIG+setup[loseteam]["keeploses"]+NOR" �s�ѡA���Z�j�T�h��" });
		}
	}
	
	// ����̦h�T������
	if( game[TEAM1K] > game[TEAM2K] && game[TEAM1K] > record[year][RECORD_SPECIAL]["maxstrikeouts"] )
	{
		record[year][RECORD_SPECIAL]["maxstrikeouts"] = game[TEAM1K];
		record[year][RECORD_SPECIAL]["maxstrikeoutsplayer"] = setup[game[TEAM1]]["roster"][0]["file"];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team1name+"�v��"+setup[game[TEAM1]]["roster"][0]["object"]->query_idname()+"��}���u������̦h�T�������A�����Ө� "HIY+game[TEAM1K]+NOR YEL"K"NOR" �I�I" });
	}
	else if( game[TEAM2K] > game[TEAM1K] && game[TEAM2K] > record[year][RECORD_SPECIAL]["maxstrikeouts"] )
	{
		record[year][RECORD_SPECIAL]["maxstrikeouts"] = game[TEAM2K];
		record[year][RECORD_SPECIAL]["maxstrikeoutsplayer"] = setup[game[TEAM2]]["roster"][0]["file"];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team2name+"�v��"+setup[game[TEAM2]]["roster"][0]["object"]->query_idname()+"��}���u������̦h�T�������A�����Ө� "HIY+game[TEAM2K]+NOR YEL"K"NOR" �I�I" });
	}
	
	// ����̦h�w������
	if( game[TEAM1HIT] > game[TEAM2HIT] && game[TEAM1HIT] > record[year][RECORD_SPECIAL]["maxhits"] )
	{
		record[year][RECORD_SPECIAL]["maxhits"] = game[TEAM1HIT];
		record[year][RECORD_SPECIAL]["maxhitsteam"] = game[TEAM1];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team1name+"�v��}���u����̦h�w�������A�����Ө� "HIY+game[TEAM1HIT]+NOR YEL" ���w��"NOR" �I�I" });
	}
	else if( game[TEAM2HIT] > game[TEAM1HIT] && game[TEAM2HIT] > record[year][RECORD_SPECIAL]["maxhits"] )
	{
		record[year][RECORD_SPECIAL]["maxhits"] = game[TEAM2HIT];
		record[year][RECORD_SPECIAL]["maxhitsteam"] = game[TEAM2];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team2name+"�v��}���u����̦h�w�������A�����Ө� "HIY+game[TEAM2HIT]+NOR YEL" ���w��"NOR" �I�I" });
	}
		
	// ����̦h�o������
	if( game[TEAM1SCORE] > game[TEAM2SCORE] && game[TEAM1SCORE] > record[year][RECORD_SPECIAL]["maxscores"] )
	{
		record[year][RECORD_SPECIAL]["maxscores"] = game[TEAM1SCORE];
		record[year][RECORD_SPECIAL]["maxscoresteam"] = game[TEAM1];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team1name+"�v��}���u����̦h�o�������A�����Ө� "HIY+game[TEAM1SCORE]+NOR YEL" ��"NOR" �I�I" });
	}
	else if( game[TEAM2SCORE] > game[TEAM1SCORE] && game[TEAM2SCORE] > record[year][RECORD_SPECIAL]["maxscores"] )
	{
		record[year][RECORD_SPECIAL]["maxscores"] = game[TEAM2SCORE];
		record[year][RECORD_SPECIAL]["maxscoresteam"] = game[TEAM2];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team2name+"�v��}���u����̦h�o�������A�����Ө� "HIY+game[TEAM2SCORE]+NOR YEL" ��"NOR" �I�I" });
	}
	
	// ����̦h���Ƭ���
	if( game[INNING] > record[year][RECORD_SPECIAL]["maxinnings"] )
	{
		record[year][RECORD_SPECIAL]["maxinnings"] = game[INNING];
		record[year][RECORD_SPECIAL]["maxinningsteam1"] = game[TEAM1];
		record[year][RECORD_SPECIAL]["maxinningsteam2"] = game[TEAM2];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team1name+"�v�P�u"+team2name+"�v�@�P�гy���u��������ɳ̦h���Ƭ����A�����Ө� "HIY+(game[INNING]/2 + game[INNING]%2)+NOR YEL" ��"NOR" �I�I" });
	}
	
	// ������Ʈt�̦h����
	if( abs(game[TEAM1SCORE] - game[TEAM2SCORE]) > record[year][RECORD_SPECIAL]["maxscorediff"] )
	{
		record[year][RECORD_SPECIAL]["maxscorediff"] = abs(game[TEAM1SCORE] - game[TEAM2SCORE]);
		record[year][RECORD_SPECIAL]["maxscorediffwin"] = game[TEAM1SCORE] > game[TEAM2SCORE] ? game[TEAM1] : game[TEAM2];
		record[year][RECORD_SPECIAL]["maxscoredifflose"] = game[TEAM1SCORE] < game[TEAM2SCORE] ? game[TEAM1] : game[TEAM2];
		
		msg += ({ HIY"����"NOR YEL"��s "NOR"�u"+team1name+"�v�P�u"+team2name+"�v�@�P�гy���u������Ʈt�Z�̤j�����A�����Ө� "HIY+abs(game[TEAM1SCORE] - game[TEAM2SCORE])+NOR YEL" ��"NOR" �I�I" });
	}
	
	setup[game[TEAM1]]["status"] = STATUS_IDLE;
	setup[game[TEAM2]]["status"] = STATUS_IDLE;

	if( !post_season_level )
		for(int i=0;i<sizeof(msg);++i)
			call_out((: CHANNEL_D->channel_broadcast("sport", $(msg)[$(i)]) :), 0);
			
	if( undefinedp(stopid) )
	{
		int income;
		int total_income = query_income(game[TEAM1]) + query_income(game[TEAM2]);
		
		if( file_exists(setup[game[TEAM2]]["master"]) )
		{
			object master = load_object(setup[game[TEAM2]]["master"]);
			
			income = total_income * 10 / 100;
			income += total_income * 80 / 100 * game[TEAM2SCORE] / (game[TEAM1SCORE]+game[TEAM2SCORE]);
			
			if( post_season_level && post_season_start )
				income *= 20;
				
			if( query("owner", master) == game[TEAM2] && objectp(find_player(game[TEAM2])) )
			{
				addn("money", income, master);
				income_msg += "�u"+setup[game[TEAM2]]["name"]+"�v��o���Ц��J "HIY+money(MONEY_D->query_default_money_unit(), income)+NOR;
							
				master->save();
			}
		}
		
		if( file_exists(setup[game[TEAM1]]["master"]) )
		{
			object master = load_object(setup[game[TEAM1]]["master"]);
			
			income = total_income * 10/ 100;
			income += total_income * 80 / 100 * game[TEAM1SCORE] / (game[TEAM1SCORE]+game[TEAM2SCORE]);
			
			if( post_season_level && post_season_start )
				income *= 20;
	
			if( query("owner", master) == game[TEAM1] && objectp(find_player(game[TEAM1])) )
			{
				addn("money", income, master);
				income_msg += "�u"+setup[game[TEAM1]]["name"]+"�v��o���Ц��J "HIY+money(MONEY_D->query_default_money_unit(), income)+" "NOR;
				
				master->save();
			}
		}
		
		if( income_msg != HIG"���J"NOR GRN"���t"NOR )
			msg += ({ income_msg });
	}

	if( post_season_start )
	{		
		switch(post_season_level)
		{
			case 1..4: 
			{
				if( game[TEAM1SCORE] > game[TEAM2SCORE] )
				{
					post_season_team_4 |= ({ game[TEAM1] });
					msg += ({ HIR"�u��"NOR RED"��"NOR" "+setup[game[TEAM1]]["username"]+"��x���u"+setup[game[TEAM1]]["name"]+"�v��o�|�j�ʯšI�I" });
					post_season_eliminate_teams |= ({ game[TEAM2] });
				}
				else
				{
					post_season_team_4 |= ({ game[TEAM2] });
					msg += ({ HIR"�u��"NOR RED"��"NOR" "+setup[game[TEAM2]]["username"]+"��x���u"+setup[game[TEAM2]]["name"]+"�v��o�|�j�ʯšI�I" });
					post_season_eliminate_teams |= ({ game[TEAM1] });
				}
				save();
				break;
			}
			case 5..6:
			{
				if( game[TEAM1SCORE] > game[TEAM2SCORE] )
				{
					post_season_team_2 |= ({ game[TEAM1] });
					msg += ({ HIR"�u��"NOR RED"��"NOR" "+setup[game[TEAM1]]["username"]+"��x���u"+setup[game[TEAM1]]["name"]+"�v��o�ʯ��`�a�x�ɡI�I" });
					post_season_eliminate_teams |= ({ game[TEAM2] });
				}
				else
				{
					post_season_team_2 |= ({ game[TEAM2] });
					msg += ({ HIR"�u��"NOR RED"��"NOR" "+setup[game[TEAM2]]["username"]+"��x���u"+setup[game[TEAM2]]["name"]+"�v��o�ʯ��`�a�x�ɡI�I" });
					post_season_eliminate_teams |= ({ game[TEAM1] });
				}
				save();
				break;
			}
			case 7:
			{
				object user;
				
				if( game[TEAM1SCORE] > game[TEAM2SCORE] )
				{
					msg += ({ HIR"�u��"NOR RED"��"NOR" ����"+setup[game[TEAM1]]["username"]+"��x���u"+setup[game[TEAM1]]["name"]+"�v��o"NOR CYN"�� "HIC+year+NOR CYN" �y�u"HIY"�βy"NOR YEL"�@�ɫa�x"NOR"�I�I" });
					record[year][RECORD_SPECIAL]["champion"] = game[TEAM1];
					record[year][RECORD_SPECIAL]["2rd"] = game[TEAM2];
					post_season_eliminate_teams |= ({ game[TEAM2] });
					user = load_user(game[TEAM1]);
				}
				else
				{
					msg += ({ HIR"�u��"NOR RED"��"NOR" ����"+setup[game[TEAM2]]["username"]+"��x���u"+setup[game[TEAM2]]["name"]+"�v��o"NOR CYN"�� "HIC+year+NOR CYN" �y�u"HIY"�βy"NOR YEL"�@�ɫa�x"NOR"�I�I" });
					record[year][RECORD_SPECIAL]["champion"] = game[TEAM2];
					record[year][RECORD_SPECIAL]["2rd"] = game[TEAM1];
					post_season_eliminate_teams |= ({ game[TEAM1] });
					
					user = load_user(game[TEAM2]);
				}
				
				if( objectp(user) )
				{
					object ring = new("/obj/baseball/champion_ring_"+year);
					
					user->add_title(sprintf(HIC+"%-4d"+HIY"�`"NOR YEL"�a"NOR YEL"�x"NOR, year));
					user->save();				
					
					tell(user, pnoun(2, user)+"��o�@�u�u"+ring->query_idname()+"�v�C\n");
					CHANNEL_D->channel_broadcast("sport", user->query_idname()+"��o�@�u�u"+ring->query_idname()+"�v�C");
					
					ring->set_keep();
					ring->move(user);
					
					if( !userp(user) )
						destruct(user);
				}
					
				save();
					
				break;
			}
		}
	}
	
	if( !random(10) )
	{
		int status;
		object *equipments;
		
		foreach(mapping roster_data in setup[game[TEAM1]]["roster"] + setup[game[TEAM2]]["roster"])
		{
			if( !objectp(roster_data["object"]) ) continue;
				
			equipments = roster_data["object"]->query_equipping_object(EQ_MITT[PART_ID]) || allocate(0);
			equipments |= roster_data["object"]->query_equipping_object(EQ_BAT[PART_ID]) || allocate(0);
			
			if( !sizeof(equipments) ) continue;
				
			equipments = sort_array(equipments, (: random(2) ? 1 : -1 :));
				
			if( !random(25) )
			{
				msg += ({ roster_data["object"]->query_idname()+"�ҨϥΪ�"+equipments[0]->query_idname()+"�b�o�����ɤ��a���F�I�I\n" });
				
				roster_data["object"]->unequip(equipments[0], ref status);
				
				destruct(equipments[0], 1);
			}
		}
	}
			
	games[gameindex] = 0;
	//games -= ({ 0 });

	return msg;
}

// �p�����
private string *new_score(int gameindex, string attacker, string defender, int score, int scoretype)
{
	string *msg = allocate(0);
	mapping game = games[gameindex];
	int attacknumber;
	
	// �p��o��
	if( attacker == game[TEAM1] )
	{
		game[TEAM1SCORE] += score;
		attacknumber = game[TEAM1NUMBER];
	}
	else
	{
		game[TEAM2SCORE] += score;
		attacknumber = game[TEAM2NUMBER];
	}
		
	add_player_record(defender, 0, "run", score);
	add_player_record(attacker, attacknumber, "rbi", score);
	
	msg += ({ "�u"+setup[game[TEAM2]]["name"]+" "+game[TEAM2SCORE]+" : "+game[TEAM1SCORE]+" "+setup[game[TEAM1]]["name"] +"�v" });
	
	// �U�b���A�A���o��
	if( game[INNING] >= 18 && (game[INNING] % 2) == 0 && game[TEAM1SCORE] > game[TEAM2SCORE] )
	{
		switch(scoretype)
		{
			case SCORETYPE_HOMERUN:		msg += ({ "�o�O�@���A�����S���I�I" }); 	break;
			case SCORETYPE_FOURBALL:	msg += ({ "�o�O�@�ӦA���|�a�I�I" }); 	break;
			case SCORETYPE_SACRIFICE:	msg += ({ "�o�O�@���A���묹���I�I" });	break;
			case SCORETYPE_HIT:		msg += ({ "�o�O�@���A���w���I�I" }); 	break;
			default: error("error scoretype");
		}

		msg += game_finish(gameindex);
	}
	
	return msg;
}

// �p��s���X����
private string *new_out(int gameindex, string attacker, string defender, int outcount, int outtype)
{
	string *msg = allocate(0);
	mapping game = games[gameindex];
	int attackernumber = attacker == game[TEAM1] ? game[TEAM1NUMBER] : game[TEAM2NUMBER];
		
	game[STRIKE] = 0;
	game[BALL] = 0;

	// ���u���Ƭ���
	add_player_record(attacker, attackernumber, "ab", 1);
	
	// ����M�H������
	add_player_record(defender, 0, "bf", 1);
	
	// ����M�X����
	add_player_record(defender, 0, "out", outcount);
	
	if( outtype == BASETYPE_STRIKEOUT )
		add_player_record(defender, 0, "strikeout", 1);	
			
	if( (game[OUT]+=outcount) >= 3 )
	{			
		game[OUT] = 0;

		if( game[INNING] >= 17 && (((game[INNING] % 2) && game[TEAM1SCORE] > game[TEAM2SCORE]) || (!(game[INNING] % 2) && game[TEAM1SCORE] != game[TEAM2SCORE])))
			return game_finish(gameindex);
		
		game[INNING]++;
		
		msg += ({ "�u"+setup[game[TEAM2]]["name"]+" "+game[TEAM2SCORE]+" : "+game[TEAM1SCORE]+" "+setup[game[TEAM1]]["name"] +"�v��u����A���ɶi�J��"+(game[INNING]>=19?"������":"")+"�� "+( game[INNING]%2 ? ((game[INNING]+1)/2)+" ���W�b" : ((game[INNING]+1)/2)+" ���U�b" ) });
			
		game[RUNNER1ST] = 0;
		game[RUNNER2ND] = 0;
		game[RUNNER3RD] = 0;
	}
	else
	{	
		// ���, �����|�i�S
		if( outtype == BASETYPE_BUNT || outtype == BASETYPE_TOUCHKILL )
		{
			// ���S�h�ʱ��T�S�]��
			if( game[RUNNER3RD] > 0 && game[RUNNER2ND] > 0 && game[RUNNER1ST] > 0 )
			{
				game[RUNNER3RD] = game[RUNNER2ND];
				game[RUNNER2ND] = game[RUNNER1ST];
				game[RUNNER1ST] = attackernumber;
			}
			else
			{
				// �T�S�L�H �G�S���H, �i�S
				if( game[RUNNER3RD] == 0 && game[RUNNER2ND] > 0 )
				{
					game[RUNNER3RD] = game[RUNNER2ND];
					game[RUNNER2ND] = 0;
				}
				
				// �G�S�L�H �@�S���H, �i�S
				if( game[RUNNER2ND] == 0 && game[RUNNER1ST] > 0 )
				{
					game[RUNNER2ND] = game[RUNNER1ST];
					game[RUNNER1ST] = 0;
				}
			}	
		}
		// ����, �����|�i�S
		else if( outtype == BASETYPE_CATCHKILL )
		{
			if( game[RUNNER3RD] > 0 )
			{
				msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̽Ħ^���S�o���I" });
				msg += new_score(gameindex, attacker, defender, 1, SCORETYPE_SACRIFICE);
				
				if( !games[gameindex] )
					return msg;

				game[RUNNER3RD] = 0;
			}
		}
		// ����, �����|�i�S
		else if( outtype == BASETYPE_DOUBLEPLAY )
		{
			// �T�S�L�H �G�S���H, �i�S
			if( game[RUNNER2ND] > 0 )
			{
				game[RUNNER3RD] = game[RUNNER2ND];
				game[RUNNER2ND] = 0;
			}
			
			game[RUNNER1ST] = 0;
		}
		// �T�� , �z�פW���|����즹(��������)
		//else if( outtype == BASETYPE_TRIPLEPLAY ) { }
		
		if( game[RUNNER3RD] > 0 || game[RUNNER2ND] > 0 || game[RUNNER1ST] > 0 )
			msg += ({ "�u"+setup[attacker]["name"]+"�v�ثe�S�W���]�̱��ά� [ "+(game[RUNNER3RD] > 0?HIW"��"NOR:WHT"��"NOR)+" "+(game[RUNNER2ND] > 0?HIW"��"NOR:WHT"��"NOR)+" "+(game[RUNNER1ST] > 0?HIW"��"NOR:WHT"��"NOR)+" ] "HIW+game[OUT]+NOR WHT"out"NOR });
	}

	// ���U�@��
	if( attacker == game[TEAM1] )
		game[TEAM1NUMBER] = 1 + (game[TEAM1NUMBER] % 9);
	else
		game[TEAM2NUMBER] = 1 + (game[TEAM2NUMBER] % 9);
		
	setup[attacker]["hint"] = 0;
	setup[defender]["hint"] = 0;
	
	games[gameindex] = game;
	
	return msg;
}

// �p��s���i�S
private string *new_base(int gameindex, string attacker, string defender, int basecount, int basetype)
{
	string *msg = allocate(0);
	mapping game = games[gameindex];

	int score = 0;
	int attacknumber = attacker == game[TEAM1] ? game[TEAM1NUMBER] : game[TEAM2NUMBER];
	
	if( basecount <= 0 ) error("basecount_error");

	game[STRIKE] = 0;
	game[BALL] = 0;

	// ����M�H��
	add_player_record(defender, 0, "bf", 1);

	if( basetype == BASETYPE_HIT ) // �w���i�S
	{
		int scoretype;
		
		// ���̥������Ƭ���
		add_player_record(attacker, attacknumber, "ab", 1);

		if( attacker == game[TEAM1] )
			game[TEAM1HIT]++;
		else
			game[TEAM2HIT]++;
			
		switch(basecount)
		{
			case 1:
			{
				add_player_record(attacker, attacknumber, "hit1", 1);
				add_player_record(attacker, attacknumber, "hit", 1);
				
				if( game[RUNNER3RD] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̩b�^���S�o���I" }); game[RUNNER3RD] = 0; }	
				if( game[RUNNER2ND] > 0 )
				{
					if( random(setup[attacker]["roster"][game[RUNNER2ND]]["object"]->query_int()) > random(400) )
					{
						score++; 
						msg += ({ "�u"+setup[attacker]["name"]+"�v�G�S�]�̩b�^���S�o���I" });	
					
						game[RUNNER2ND] = 0; 
					}
					else
					{
						game[RUNNER3RD] = game[RUNNER2ND];
						game[RUNNER2ND] = 0; 
					}
				}
						
				// �@�S���H�A�i�S
				if( game[RUNNER1ST] > 0 )
				{
					if( game[RUNNER3RD] > 0 )
						game[RUNNER2ND] = game[RUNNER1ST];
					else
						game[RUNNER3RD] = game[RUNNER1ST];
						
					game[RUNNER1ST] = 0;
				}
				
				// ���̤W��@�S
				game[RUNNER1ST] = (attacker == game[TEAM1] ? game[TEAM1NUMBER] : game[TEAM2NUMBER]);
				
				scoretype = SCORETYPE_HIT;
				
				break;
			}
			case 2:
			{
				add_player_record(attacker, attacknumber, "hit2", 1);
				add_player_record(attacker, attacknumber, "hit", 1);
				
				if( game[RUNNER3RD] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̩b�^���S�o���I" }); game[RUNNER3RD] = 0; }	
				if( game[RUNNER2ND] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�G�S�]�̩b�^���S�o���I" }); game[RUNNER2ND] = 0; }
				if( game[RUNNER1ST] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�@�S�]�̩b�^���S�o���I" }); game[RUNNER1ST] = 0; }
				
				// ���̤W��G�S
				game[RUNNER2ND] = (attacker == game[TEAM1] ? game[TEAM1NUMBER] : game[TEAM2NUMBER]);
	
				scoretype = SCORETYPE_HIT;
				
				break;	
			}
			case 3:
			{
				add_player_record(attacker, attacknumber, "hit3", 1);
				add_player_record(attacker, attacknumber, "hit", 1);
				
				if( game[RUNNER3RD] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̩b�^���S�o���I" }); game[RUNNER3RD] = 0; }	
				if( game[RUNNER2ND] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�G�S�]�̩b�^���S�o���I" }); game[RUNNER2ND] = 0; }
				if( game[RUNNER1ST] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�@�S�]�̩b�^���S�o���I" }); game[RUNNER1ST] = 0; }
							
				// ���̤W��T�S
				game[RUNNER3RD] = (attacker == game[TEAM1] ? game[TEAM1NUMBER] : game[TEAM2NUMBER]);
				
				scoretype = SCORETYPE_HIT;
				
				break;	
			}
			case 4:
			{
				add_player_record(attacker, attacknumber, "hit4", 1);
				add_player_record(attacker, attacknumber, "hit", 1);
				
				if( game[RUNNER3RD] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̩b�^���S�o���I" }); game[RUNNER3RD] = 0; }	
				if( game[RUNNER2ND] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�G�S�]�̩b�^���S�o���I" }); game[RUNNER2ND] = 0; }
				if( game[RUNNER1ST] > 0 ) {	score++; msg += ({ "�u"+setup[attacker]["name"]+"�v�@�S�]�̩b�^���S�o���I" }); game[RUNNER1ST] = 0; }
					
				msg += ({ "�u"+setup[attacker]["name"]+"�v���̩b�^���S�o���I" });
				score++;
	
				scoretype = SCORETYPE_HOMERUN;
				
				break;
			}
		}
		
		msg += new_score(gameindex, attacker, defender, score, scoretype);
			
		if( !games[gameindex] )
			return msg;
	}
	else if( basetype == BASETYPE_BALL ) // �O�e�i�S
	{
		add_player_record(defender, 0, "fourball", 1);
		add_player_record(attacker, attacknumber, "walk", 1);

		// �Y�@�S���H
		if( game[RUNNER1ST] > 0 )
		{
			// �Y�G�S�]���H
			if( game[RUNNER2ND] > 0 )
			{
				// �Y�T�S�]���H
				if( game[RUNNER3RD] > 0 )
				{
					msg += ({ "�u"+setup[attacker]["name"]+"�v�T�S�]�̲@���O�O�a���^���S�o���I" });
					msg += new_score(gameindex, attacker, defender, 1, SCORETYPE_FOURBALL);
					
					if( !games[gameindex] )
						return msg;
				}

				game[RUNNER3RD] = game[RUNNER2ND];	
			}

			game[RUNNER2ND] = game[RUNNER1ST];
		}

		game[RUNNER1ST] = attacknumber;	
	}

	// ���U�@��
	if( attacker == game[TEAM1] )
		game[TEAM1NUMBER] = 1 + (game[TEAM1NUMBER] % 9);
	else
		game[TEAM2NUMBER] = 1 + (game[TEAM2NUMBER] % 9);

	setup[attacker]["hint"] = 0;
	setup[defender]["hint"] = 0;
	
	if( game[RUNNER3RD] > 0 || game[RUNNER2ND] > 0 || game[RUNNER1ST] > 0 )
		msg += ({ "�u"+setup[attacker]["name"]+"�v�ثe�S�W���]�̱��ά� [ "+(game[RUNNER3RD] > 0?HIW"��"NOR:WHT"��"NOR)+" "+(game[RUNNER2ND] > 0?HIW"��"NOR:WHT"��"NOR)+" "+(game[RUNNER1ST] > 0?HIW"��"NOR:WHT"��"NOR)+" ] "HIW+game[OUT]+NOR WHT"out"NOR });
		
	games[gameindex] = game;

	return msg;
}

private void check_cache(object player)
{
	if( query_temp("baseballcache/handside/time", player) < time() - 300 )
	{
		int type;
		int level;
		
		if( (level = player->query_skill_level("twohands")) > 0 )
			type = HANDSIDE_TWOHANDS;
		else if( (level = player->query_skill_level("lefthand")) > 0 )
			type = HANDSIDE_LEFTHAND;
		else if( (level = player->query_skill_level("righthand")) > 0 )
			type = HANDSIDE_RIGHTHAND;
		else
			type = HANDSIDE_NONE;
			
		set_temp("baseballcache/handside", ([ "time":time(), "type":type, "level":level ]), player);
	}
}

int get_handside_type(object player)
{
	check_cache(player);
	return query_temp("baseballcache/handside/type", player);
}

int get_handside_level(object player)
{
	check_cache(player);
	return query_temp("baseballcache/handside/level", player);
}

// ���̻P��⥪�k�誺�t�� (-60~70) ����
int get_handside_diff(object pitcher, object batter)
{
	int base_level_diff = (get_handside_level(batter) - get_handside_level(pitcher))/20;
	int batter_handside_type = get_handside_type(batter);
	int pitcher_handside_type = get_handside_type(pitcher);
	
	if( pitcher_handside_type == HANDSIDE_NONE )
		return 20 + base_level_diff;
	else if( batter_handside_type == HANDSIDE_TWOHANDS )
		return 8 + base_level_diff;
	else if( batter_handside_type != pitcher_handside_type )
		return 20 + base_level_diff;
	else
		return -10 + base_level_diff;
}

// ���o�����v
varargs float get_hit_rate(string teamid, int num, int year)
{
	int hit, ab;
	
	if( undefinedp(year) )
		year = season_year;

	if( undefinedp(setup[teamid]) || !mapp(setup[teamid]["roster"][num]) )
		return 0.;

	hit = get_record(RECORD_PLAYER, setup[teamid]["roster"][num]["file"], "hit", year);
	ab = get_record(RECORD_PLAYER, setup[teamid]["roster"][num]["file"], "ab", year);

	if( !hit || !ab ) return 0.;
		
	return 10. * hit / ab;
}

// �i��y��
void play_game()
{
	string *msg;
	mapping attacksetup, defendsetup;
	string attackteamname, defendteamname;
	int attacknumber, defendnumber;
	string attacker, defender;
	string team1, team2;
	mapping game;
	
	games -= ({ 0 });

	for(int gameindex=0;gameindex<sizeof(games);++gameindex)
	{
		msg = allocate(0);

		game = games[gameindex];
			
		if( ++game[TICK] < PREPARETIME ) continue;

		if( post_season_start && post_season_level )
		{
			if( (game[TICK]+gameindex) % POSTSEASON_TICKTIME ) continue;
		}
		else if( (game[TICK]+gameindex) % TICKTIME ) continue;

		team1 = game[TEAM1];
		team2 = game[TEAM2];
	
		if( !setup[team1] || !setup[team2] ) continue;

		if( !valid_setup(team1) )
		{
			broadcast(HIG"�i���ɡj"NOR"�u"+setup[team1]["name"]+"�v���y����Ƥ�����A������ɡC\n", game);
			cancel_game(team1, 1);
			continue;
		}
			
		if( !valid_setup(team2) )
		{
			broadcast(HIG"�i���ɡj"NOR"�u"+setup[team2]["name"]+"�v���y����Ƥ�����A������ɡC\n", game);
			cancel_game(team2, 1);
			continue;
		}
		
		if( game[TICK] <= PREPARETIME + TICKTIME && game[TICK] >= PREPARETIME )
		{
			setup[team1]["status"] = STATUS_PLAYING;
			setup[team2]["status"] = STATUS_PLAYING;
		}
		
		if( (game[INNING] % 2) > 0 )
		{	
			attacksetup = setup[team2];
			defendsetup = setup[team1];
			
			attacknumber = game[TEAM2NUMBER];
			defendnumber = game[TEAM1NUMBER];
			
			attacker = team2;
			defender = team1;
		}
		else
		{
			attacksetup = setup[team1];
			defendsetup = setup[team2];
			
			attacknumber = game[TEAM1NUMBER];
			defendnumber = game[TEAM2NUMBER];
			
			attacker = team1;
			defender = team2;
		}

		attackteamname = "�u"+attacksetup["name"]+"�v";
		defendteamname = "�u"+defendsetup["name"]+"�v";
		
		if( game[INNING] == 0 )
		{
			string defendhandside, attackhandside;
			
			switch(get_handside_type(defendsetup["roster"][0]["object"]))
			{
				case HANDSIDE_NONE: defendhandside = ""; break;
				case HANDSIDE_LEFTHAND: defendhandside = HIC"��"NOR CYN"��"NOR; break;
				case HANDSIDE_RIGHTHAND: defendhandside = HIG"�k"NOR GRN"��"NOR; break;
				default: defendhandside = HIR"���~"NOR; break;
			}
			switch(get_handside_type(attacksetup["roster"][0]["object"]))
			{
				case HANDSIDE_NONE: attackhandside = ""; break;
				case HANDSIDE_LEFTHAND: attackhandside = HIC"��"NOR CYN"��"NOR; break;
				case HANDSIDE_RIGHTHAND: attackhandside = HIG"�k"NOR GRN"��"NOR; break;
				default: attackhandside = HIR"���~"NOR; break;
			}
			
			msg += ({ attacksetup["username"]+"��"+attackteamname+"�P"+defendsetup["username"]+"��"+defendteamname+"���ɥ����}�l�C" });
			msg += ({ "���謣�X�����o�����O��"+attackteamname+attackhandside+attacksetup["roster"][0]["object"]->query_idname()+"�P"+defendteamname+defendhandside+defendsetup["roster"][0]["object"]->query_idname()+"�C" });
			
			game[INNING]++;
		}
		else
		{
			object pitcher = defendsetup["roster"][0]["object"];
			object batter = attacksetup["roster"][attacknumber]["object"];
			
			string defend_msg="", attack_msg="", info_msg="", *special_msg = allocate(0);
			string pitcherballtype;
			
			int pitcherbase = get_handside_level(pitcher) + pitcher->query_str() * 2;		// ���򥻯�O(�w���d�� 10~1500 ����)
			int batterbase = get_handside_level(batter) + batter->query_str() * 2;			// ���̰򥻯�O(�w���d�� 10~1500 ����)
			
			string *availableballtypes;
			int pitchballpower;
			int pitchballspeed;
			int pitchswingeffect = 0;
			int pitchhiteffect = 0;
			int pitchgoodhiteffect = 0;
			
			availableballtypes = filter_array(pitch_ball_types, (: $(pitcher)->query_skill_level($1) > 0 :));
			
			
				
			if( sizeof(availableballtypes) )
			{
				string balltype = availableballtypes[random(sizeof(availableballtypes))];
				
				pitchballpower = pitcher->query_skill_level(balltype);
				pitcherballtype = pitch_ball_types_name[balltype];
				
				switch(balltype)
				{
					case "fourseam":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 120, 157) + random(6); 
						pitchswingeffect = -20;
						pitchhiteffect = 10;
						pitchgoodhiteffect = 10;
						break;
					case "twoseam":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 115, 151) + random(6);
						pitchswingeffect = -10;
						pitchhiteffect = 0;
						pitchgoodhiteffect = 10;
						break;
					case "curveball":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 90, 127) + random(6);
						pitchswingeffect = 10;
						pitchhiteffect = -15;
						pitchgoodhiteffect = 5;
						break;
					case "slider":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 110, 148) + random(6);
						pitchswingeffect = -5;
						pitchhiteffect = 10;
						pitchgoodhiteffect = -5;
						break;
					case "forkball":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 100, 137) + random(6);
						pitchswingeffect = 0;
						pitchhiteffect = -10;
						pitchgoodhiteffect = 10;
						break;
					case "sinker":
						pitchballspeed = calculate(pitchballpower, 0.8, 999, 110, 148) + random(6);
						pitchswingeffect = 5;
						pitchhiteffect = 20;
						pitchgoodhiteffect = -25;
						break;
				}
				
				// �y�طU�h�A�¤O�U�j
				if( sizeof(availableballtypes) == 1 )
					pitchballpower -= 200;
				else
					pitchballpower += sizeof(availableballtypes) * 30;
			}
			else
			{
				pitchballpower = 0;
				pitcherballtype = "�n�z���y";
				
				pitchballspeed = 100+random(15);
			}
			
			pitchballpower += defendsetup["roster"][0][BUFF_PITCHER_POWER];
			
			if( pitchballpower <= 0 )
				pitchballpower = 0;
				
			pitchballpower = calculate(pitchballpower, 0.8, 999, 0, 100);
			
			if( objectp(find_player(defender)) )
			switch(get_handside_type(pitcher))
			{
				case HANDSIDE_LEFTHAND: pitcher->add_skill_exp("lefthand", random(100)); break;
				case HANDSIDE_RIGHTHAND: pitcher->add_skill_exp("righthand", random(100)); break;
			}
			
			if( objectp(find_player(attacker)) )
			switch(get_handside_type(batter))
			{
				case HANDSIDE_LEFTHAND: batter->add_skill_exp("lefthand", random(900)); break;
				case HANDSIDE_RIGHTHAND: batter->add_skill_exp("righthand", random(900)); break;
				case HANDSIDE_TWOHANDS: batter->add_skill_exp("twohands", random(900)); break;
			}
			
			if( defendsetup["hint"] == HINTTYPE_PITCHER )
			{
				pitchballpower += 70;
				pitchballspeed += range_random(2, 5);
				
				defend_msg = defendteamname+pitcher->query_idname()+HIY"����"NOR YEL"�e�`"NOR+(pitchballspeed>=155?"�t":"��")+"�X"+(pitchballspeed>=160?"��H��":"")+" "+pitchballspeed+"km "+ pitcherballtype;
			}
			else
			{
				defend_msg = defendteamname+pitcher->query_idname()+(pitchballspeed>=155?"�t":"��")+"�X"+(pitchballspeed>=160?"��H��":"")+" "+pitchballspeed+"km "+ pitcherballtype;
			}

			
			attack_msg = attackteamname+attacknumber+" ��"+batter->query_idname();
			
			// �q���|�a
			if( defendsetup["hint"] == HINTTYPE_WALK )
			{
				if( ++game[BALL] == 4 )
				{
					defend_msg = defendteamname+pitcher->query_idname()+"���@�ǻ����߲y������A"HIG"�a�y"NOR"�I�ި��[���@�}�N�n";
					attack_msg += "��o�q���|�a�O�e�W�S�C";
	
					// �i�@�S
					special_msg += new_base(gameindex, attacker, defender, 1, BASETYPE_BALL);
				}
				else
				{
					defend_msg = defendteamname+pitcher->query_idname()+"���@�ǻ����߲y������A"HIG"�a�y"NOR"�I�ި��[���@�}�N�n";
					attack_msg += "�@�ߦa���ݥ|�a�O�e";
	
					info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
				}				
			}
			// �i��Ĳ���u��
			else if( attacksetup["hint"] == HINTTYPE_BUNT )
			{
				// �����w��!
				if( random(batter->query_int()) > random(600) )
				{
					attack_msg += "�\�X�_ŧ�u�αN�y���X�I�o�O�@�䤺���w���I�I";
					
					special_msg += new_base(gameindex, attacker, defender, 1, BASETYPE_HIT);
				}
				else if( !random(5) )
				{
					attack_msg += "�\�X�u�αN�y���X�I���o�I�X�@���n�z���y�Q�����I";
					
					info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;
					
					special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_CATCHKILL);
				}
				else
				{
					// ���S
					if( game[RUNNER1ST] > 0 && game[RUNNER2ND] > 0 && game[RUNNER3RD] > 0 )
					{
						if( game[OUT] < 2 )								
							attack_msg += "�\�X�u�αN�y���X�I�T�S�]�̳Q�ʱ��b���S�I";
						else
							attack_msg += "�\�X�u�αN�y���X�I���Q�������d��è���b�@�S�I";
					}
					else
						attack_msg += "�\�X�u�αN�y���X�I���Q�������d��è���b�@�S�I";
													
					info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;
					
					special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_BUNT);
				}
			}
			// ��X�n�y
			else if( calculate(pitcherbase, 0.8, 1500, 400, 700) > random(1000) )
			{
				// ���̺�ǿ�y�B�X�Ϊ����v 550 + (0~100) + (-60~70) - (0~100) + (-20~5)
				int swingchance = 550 + calculate(batter->query_skill_level("hitaccuracy"), 0.8, 999, 0, 100) + get_handside_diff(pitcher, batter) - pitchballpower + pitchswingeffect;
								
				if( attacksetup["hint"] == HINTTYPE_BATTER )
				{
					attack_msg += HIY"����"NOR YEL"�e�`"NOR;
					swingchance += 40;
				}

				defend_msg += HIR"�n�y"NOR"�I";

				if( game[TEAM1SCORE] > game[TEAM2SCORE] && game[TEAM2] == attacker )
					swingchance += 3 * (game[TEAM1SCORE] - game[TEAM2SCORE]);

				if( game[TEAM2SCORE] > game[TEAM1SCORE] && game[TEAM1] == attacker )
					swingchance += 3 * (game[TEAM2SCORE] - game[TEAM1SCORE]);
						
				// �n�y�B�X��
				if( swingchance > random(1000) )
				{
					// ���X�y�����v 700 + (0~100) - (0~100)
					int hitchance = 700 + calculate(batter->query_skill_level("hitrange"), 0.7, 999, 0, 100) - pitchballpower;
						
					if( attacksetup["hint"] == HINTTYPE_BATTER )
						hitchance += 40;
					
					if( game[INNING] > 18 )
						hitchance += 3 * (game[INNING]/2-9);
								
					hitchance += attacksetup["roster"][attacknumber][BUFF_BATTER_POWER];
		
					switch(attacknumber)
					{
						case 1: hitchance += 50; break;
						case 2: hitchance += 20; break;
						case 3: hitchance += 40; break;
						case 4: hitchance += 80; break;
						case 5: hitchance += 40; break;
						case 6: hitchance += 10;break;
						case 7: hitchance -= 10; break;
						case 8: hitchance -= 10; break;
						case 9: hitchance -= 10; break;
					}
							
					// �����X�h
					if( hitchance + pitchhiteffect > random(1000) )
					{
						//tell(find_player("clode"), attacker+(hitchance + pitchgoodhiteffect)+"/"+defender+(defendsetup["outfield"]+defendsetup["infield"])+"\n");
						
						// �w��
						if( hitchance + pitchgoodhiteffect + 100 > random(800 + defendsetup["outfield"] + defendsetup["infield"] ) )
						{												
							// ���X�y���t��(500~3300)
							int hitpower = batter->query_skill_level("hitpower");
							int ballspeed = 500 + hitpower + batterbase;
							
							ballspeed = calculate(ballspeed, 0.7, 3000, 0, 1000);

							// ���S��
							if( ballspeed > random(8000) )
							{
								int distance;
								
								// �i�S
								special_msg += new_base(gameindex, attacker, defender, 4, BASETYPE_HIT);
								
								if( hitpower > random(5000) )
								{
									if( hitpower > random(5000) )
									{
										if( hitpower > random(5000) )
											distance = range_random(180, 190);
										else
											distance = range_random(170, 180);
									}
									else
										distance = range_random(160, 170);
										
									attack_msg += "�F�X�F�@���W�j��"+( game[RUNNER1ST] > 0 && game[RUNNER2ND] > 0 && game[RUNNER3RD] > 0 ? "���e" : "" )+"���S��("+distance+"m)�I�I";
								}
								else
								{
									distance = range_random(140, 160);
									attack_msg += "���X"+( game[RUNNER1ST] > 0 && game[RUNNER2ND] > 0 && game[RUNNER3RD] > 0 ? "���e" : "" )+"���S��("+distance+"m)�I�I";
								}
								
								if( distance > record[season_year][RECORD_SPECIAL]["maxhomerundistance"] )
								{
									record[season_year][RECORD_SPECIAL]["maxhomerundistance"] = distance;
									record[season_year][RECORD_SPECIAL]["maxhomerundistanceplayer"] = attacksetup["roster"][attacknumber]["file"];
									
									CHANNEL_D->channel_broadcast("sport", HIY"����"NOR YEL"��s "NOR+attackteamname+"��"+batter->query_idname()+"���X���u�̤j�����S���A�Z���Ө� "HIY+distance+NOR YEL"m"NOR" �I�I" );
								}
								
								attack_msg += "("HIY+get_record(RECORD_PLAYER, attacksetup["roster"][attacknumber]["file"], "hit4", season_year)+NOR")";
							}
							// �T�S��
							else if( ballspeed > random(10000) )
							{
								// �i�S
								special_msg += new_base(gameindex, attacker, defender, 3, BASETYPE_HIT);
								
								if( random(defendsetup["outfield"]) < 100 )
									attack_msg += "���X�������y�A�~����ĤO���y���M�C�F�@�B�A�Φ��T�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
								else
									attack_msg += "���X�T�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
							}
							// �G�S��
							else if( ballspeed > random(3000) )
							{
								// �i�S
								special_msg += new_base(gameindex, attacker, defender, 2, BASETYPE_HIT);
								
								if( random(defendsetup["outfield"]) < 100 )
									attack_msg += "���X����i���y���~����\�\�ӿ�A�Φ��G�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
								else
									attack_msg += "���X�G�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
							}
							// �@�S��
							else
							{
								// �i�S
								special_msg += new_base(gameindex, attacker, defender, 1, BASETYPE_HIT);
								
								if( random(defendsetup["infield"]) < 100 )
									attack_msg += "���X���u�a�y�w�O�q�������u����V�X�h�A�Φ��@�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
								else
									attack_msg += "���X�@�S�w���I("HIG+sprintf("%.2f", get_hit_rate(attacker, attacknumber))+NOR")";
							}								
						}
						// �ɥ~�y
						else if( hitchance > random(5000) )
						{
							if( game[STRIKE] < 2 )
							{
								attack_msg += "�����ɥ~�y";

								game[STRIKE]++;

								info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
							}
							else
							{
								attack_msg += "�����ɥ~�y";

								info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
							}
						}
						// �����X�h�D�����Ψ��
						else
						{						
							// ���y
							if( random(2) )
							{
								attack_msg += "�N�y���������X�h�I���Q����";
								
								info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;
									
								special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_CATCHKILL);
							}
							// �u�a
							else
							{
								if( !random(15) && game[RUNNER2ND] > 0 && game[RUNNER1ST] > 0 && game[OUT] == 0 )
								{
									attack_msg += "�N�y�����X�h�I�o�O�@��u�����T�����I�I";
									
									info_msg = " "HIW+(game[OUT]+3)+NOR WHT"out"NOR;
									
									special_msg += new_out(gameindex, attacker, defender, 3, BASETYPE_TRIPLEPLAY);
								}
								else if( random(2) && game[RUNNER3RD] == 0 && game[RUNNER1ST] > 0 && game[OUT] < 2 )
								{
									attack_msg += "�N�y�����X�h�I�o�O�@���������I�I";
									
									info_msg = " "HIW+(game[OUT]+2)+NOR WHT"out"NOR;
									
									special_msg += new_out(gameindex, attacker, defender, 2, BASETYPE_DOUBLEPLAY);
								}
								else
								{								
									// ���S
									if( game[RUNNER1ST] > 0 && game[RUNNER2ND] > 0 && game[RUNNER3RD] > 0 )
									{
										if( game[OUT] < 2 )								
											attack_msg += "�N�y�����X�h�I�T�S�]�̳Q�ʱ��b���S�I";
										else
											attack_msg += "�N�y�����X�h�I���Q�������d��è���b�@�S�I";
									}
									else
										attack_msg += "�N�y�����X�h�I���Q�������d��è���b�@�S�I";
									
									info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;
									
									special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_TOUCHKILL);
								}
							}
						}

					}
					// ���θ���
					else
					{
						if( ++game[STRIKE] == 3 )
						{							
							defender == game[TEAM1] ? game[TEAM1K]++ : game[TEAM2K]++;

							attack_msg += "���θ��žD��T���X��";

							info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out "HIY + (defender == game[TEAM1] ? game[TEAM1K] : game[TEAM2K]) +NOR YEL"K!"NOR;

							special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_STRIKEOUT);
						}
						else
						{
							attack_msg += "���θ���";	

							info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
						}
					}	
				}
				// �n�y���X��
				else
				{
					if( ++game[STRIKE] == 3 )
					{					
						defender == game[TEAM1] ? game[TEAM1K]++ : game[TEAM2K]++;

						attack_msg += "���۵o�b�D��T���X��";

						info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out "HIY + (defender == game[TEAM1] ? game[TEAM1K] : game[TEAM2K]) +NOR YEL"K!"NOR;

						special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_STRIKEOUT);
					}
					else
					{
						attack_msg += "���۵o�b";

						info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
					}
				}
			}
			// ��X�a�y
			else
			{
				// ���̬ݷ��a�y�B���X�Ϊ����v 750 + (0~100) + (-60~70)
				int swingchance = 750 + calculate(batter->query_skill_level("hitaccuracy"), 0.8, 999, 0, 100) + get_handside_diff(pitcher, batter) + pitchswingeffect;

				if( setup[attacker]["hint"] == HINTTYPE_BATTER )
				{
					attack_msg += HIY"����"NOR YEL"�e�`"NOR;
					swingchance += 50;
				}

				if( game[TEAM1SCORE] > game[TEAM2SCORE] && game[TEAM2] == attacker )
					swingchance += 3 * (game[TEAM1SCORE] - game[TEAM2SCORE]);

				if( game[TEAM2SCORE] > game[TEAM1SCORE] && game[TEAM1] == attacker )
					swingchance += 3 * (game[TEAM2SCORE] - game[TEAM1SCORE]);

				defend_msg += HIG"�a�y"NOR"�A";

				// �a�y�B�S�X��
				if( swingchance > random(1000) )
				{
					if( random(200 + pitcherbase) < 5 )
					{
							attack_msg += "�Q�ɧ�y�����I��oĲ���O�e�W�S";

							// �i�@�S
							special_msg += new_base(gameindex, attacker, defender, 1, BASETYPE_BALL);
					}
					else
					{
						if( ++game[BALL] == 4 )
						{
							attack_msg += "�Ԧ�S�����ΡA��o�O�e�W�S";

							// �i�@�S
							special_msg += new_base(gameindex, attacker, defender, 1, BASETYPE_BALL);
						}
						else
						{
							attack_msg += "�Ԧ�S������";

							info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
						}
					}
				}
				// �a�y�����X��
				else
				{
					if( 200 + pitchballpower > random(800) )
					{
						if( ++game[STRIKE] == 3 )
						{						
							defender == game[TEAM1] ? game[TEAM1K]++ : game[TEAM2K]++;

							attack_msg += "���θ��žD��T���X��";

							info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out "HIY + (defender == game[TEAM1] ? game[TEAM1K] : game[TEAM2K]) +NOR YEL"K!"NOR;

							special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_STRIKEOUT);	
						}
						else
						{
							attack_msg += "�j�O�@���A���θ���";

							info_msg = " "HIR+game[STRIKE]+NOR" "HIG+game[BALL]+" "HIW+game[OUT]+NOR;
						}
					}
					else
					{
						// ���y
						if( random(2) )
						{						
							attack_msg += "�N�y���������X�h�I���Q����";

							info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;

							special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_CATCHKILL);
						}
						// �u�a
						else
						{
							if( !random(15) && game[RUNNER2ND] > 0 && game[RUNNER1ST] > 0 && game[OUT] == 0 )
							{
								attack_msg += "�N�y�����X�h�I�o�O�@��u�����T�����I�I";

								info_msg = " "HIW+(game[OUT]+3)+NOR WHT"out"NOR;

								special_msg += new_out(gameindex, attacker, defender, 3, BASETYPE_TRIPLEPLAY);
							}
							else if( random(2) && game[RUNNER3RD] == 0 && game[RUNNER1ST] > 0 && game[OUT] <= 1 )
							{
								attack_msg += "�N�y�����X�h�I�o�O�@���������I�I";

								info_msg = " "HIW+(game[OUT]+2)+NOR WHT"out"NOR;

								special_msg += new_out(gameindex, attacker, defender, 2, BASETYPE_DOUBLEPLAY);
							}
							else
							{
								// ���S
								if( game[RUNNER1ST] > 0 && game[RUNNER2ND] > 0 && game[RUNNER3RD] > 0 )
								{
									if( game[OUT] < 2 )								
										attack_msg += "�N�y�����X�h�I�T�S�]�̳Q�ʱ��b���S�I";
									else
										attack_msg += "�N�y�����X�h�I���Q�������d��è���b�@�S�I";
								}
								else
									attack_msg += "�N�y�����X�h�I���Q�������d��è���b�@�S�I";

								info_msg = " "HIW+(game[OUT]+1)+NOR WHT"out"NOR;

								special_msg += new_out(gameindex, attacker, defender, 1, BASETYPE_TOUCHKILL);
							}
						}
					}
				}
			}

			if( !game ) continue;

			msg += ({ defend_msg+attack_msg+info_msg });
			
			if( sizeof(special_msg) > 0 )
				msg += special_msg;
		}

		if( sizeof(attacksetup["messages"]) )
		{
			msg = attacksetup["messages"] + msg;
			attacksetup["messages"] = 0;
		}

		if( sizeof(defendsetup["messages"]) )
		{
			msg = defendsetup["messages"] + msg;
			defendsetup["messages"] = 0;
		}
				
		if( sizeof(msg) )
		{
			foreach(string m in msg)
			{
				broadcast(m, game);
			}
		}
	}
}

// �s�y�ɷǳƶ}�l, �e�̬��D��, ��̬��ȳ�
void new_game(string id1, string id2)
{
	mapping game = allocate_mapping(0);
	object user;
	
	game[TEAM1] = id1;
	game[TEAM2] = id2;
	game[TEAM1NUMBER] = 1;
	game[TEAM2NUMBER] = 1;
	
	if( post_season_start && post_season_level )
	{
		if( objectp(find_player(id1)) )
			tell(find_player(id1), "\a");
		
		if( objectp(find_player(id2)) )
			tell(find_player(id2), "\a");
			
		game[TICK] = PREPARETIME*4/5 - random(POSTSEASON_TICKTIME);
		
	}
	else
		game[TICK] = random(TICKTIME);

	games += ({ game });

	setup[id1]["status"] = STATUS_PREPARING;
	setup[id2]["status"] = STATUS_PREPARING;

	if( post_season_start && post_season_level )
	{
		setup[id1]["hinttimes"] = HINTTIMES * 2;
		setup[id2]["hinttimes"] = HINTTIMES * 2;
	}
	else
	{
		setup[id1]["hinttimes"] = HINTTIMES;
		setup[id2]["hinttimes"] = HINTTIMES;
	}

	setup[id1]["opponents"] -= ({ id2 });
	setup[id2]["opponents"] -= ({ id1 });
	
	if( objectp(user = find_player(id1)) )
		tell(user, pnoun(2, user)+"���βy����Y�N�P"+setup[id2]["username"]+"���u"+setup[id2]["name"]+"�v�i����ɡC\n");
	
	if( objectp(user = find_player(id2)) )
		tell(user, pnoun(2, user)+"���βy����Y�N�P"+setup[id1]["username"]+"���u"+setup[id1]["name"]+"�v�i����ɡC\n");
}

void start_game(string id)
{
	foreach(mapping ref game in games)
	{
		if( game[TEAM1] == id || game[TEAM2] == id )
		{
			if( game[TICK] < PREPARETIME )
				game[TICK] = PREPARETIME - 1 - random(TICKTIME);
		}
	}
}

string query_opponent(string id)
{
	foreach(mapping game in games)
	{
		if( !mapp(game) ) continue;

		if( game[TEAM1] == id )
			return game[TEAM2];
		if( game[TEAM2] == id )
			return game[TEAM1];
	}
	
	return 0;
}

varargs mixed query_game(string id)
{
	if( undefinedp(id) ) 
		return games;

	foreach(mapping game in games)
	{
		if( !mapp(game) ) continue;

		if( game[TEAM1] == id || game[TEAM2] == id )
			return game;
	}
	
	return 0;
}

string *query_season()
{
	return season;
}

varargs mapping query_setup(string id)
{
	if( undefinedp(id) )
		return setup;
	else
	{
		// ��l�Ƴ]�w
		if( undefinedp(setup[id]) )
		{
			setup[id] = allocate_mapping(0);
			setup[id]["listener"] = ({ id });
			setup[id]["roster"] = allocate(10);
		}

		return setup[id];
	}
}

int exists(string id)
{
	return !undefinedp(setup[id]);
}

// ���s�]�w�Ҧ��y�������
void reset_all_opponents()
{
	foreach(string id, mapping data in setup)
		setup[id]["opponents"] = copy(sort_array(season, (: random(2) ? 1 : -1 :))) - ({ id });	
}

int is_playing(object labor)
{
	string id = query("boss", labor);
	string file = base_name(labor);

	if( undefinedp(setup[id]) ) return 0;

	if( setup[id]["status"] != STATUS_PLAYING ) return 0;

	for(int i=0;i<10;++i)
		if( setup[id]["roster"][i]["file"] == file )
			return 1;

	return 0;
}

mapping query_record()
{
	return record;
}

// �߸��禡
void heart_beat()
{
	int *realtime = TIME_D->query_realtime_array();

	if( !(++tick % 1855) ) save();

	if( !post_season_start && realtime[WDAY] == 0 && realtime[HOUR] == 14 )
	{
		CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ����u�ɰt��A�ثe���b�i�檺���ɥ���������� "HIC+season_year+NOR" �y�u�u���ɧY�N�}�l�C");
		post_season_start = 1;	
	}
	
	if( !(tick%10) )
	{
		if( post_season_start )
		{		
			// �|�����ɶi�椤
			if( sizeof(games) ) return;
			
			switch(post_season_level)
			{
				// �}�l�u����
				case 0:
				{
					post_season_team_8 = sort_array(filter_array(season, (: valid_setup($1, 1) :)), (: sort_team($1, $2) :))[0..7];
					post_season_team_4 = allocate(0);
					post_season_team_2 = allocate(0);
					post_season_eliminate_teams = allocate(0);

					// �u���ɶ���� 8 ��
					if( sizeof(post_season_team_8) != 8 )
					{
						if( realtime[WDAY] == 0 && realtime[HOUR] == 20 )
						{
							CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �u���ɶ�����K��y���A�ŧi�����u���ɨ����C");
							post_season_level = 8;
						}
						return;
					}
					
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �Ҧ����W�u�ɤw�g�����A�H�U��X���y�u�Ӳv�̰����e�K�j����i��u���ɡC");
					
					for(int i=0;i<sizeof(post_season_team_8);++i)
					{
						if( objectp(find_player(post_season_team_8[i])) )
							tell(find_player(post_season_team_8[i]), "\a");

						CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" "NOR YEL"��"HIY+(i+1)+NOR YEL"�W"NOR"�G�u"+setup[post_season_team_8[i]]["name"]+"�v�A��"+setup[post_season_team_8[i]]["username"]+"��x�C");
					}
		
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �H�W�K���l�ȱN�ѥ[���y�u���̲רM�ɡA�����βy�ɳ̰��aģ�u"HIY"�@"NOR YEL"��"HIY"�a"NOR YEL"�x"NOR"�v�C");
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �u���ɩҦ��ɨƬҤ@�ԩw�ӭt�A�`�@ 7 �����ɱN��B���W�D���@�ɦP�B��p�༽�C");
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �u���ɪ����ɹL�{���N�i�֦� 2 �����t���ϥΦ��ơA�P�ɱN�֦� 20 �������Ц��J�C");
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" �����i�� "HIC"A �դK�j��"NOR"���O�ƦW�� 1 ���u"+setup[post_season_team_8[0]]["name"]+"�v��W�ƦW�� 8 ���u"+setup[post_season_team_8[7]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
				
					post_season_level++;
					new_game(post_season_team_8[0], post_season_team_8[7]);
					
					break;
				}
				case 1:
				{
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i�� "HIC"B �դK�j��"NOR"���O�ƦW�� 2 ���u"+setup[post_season_team_8[1]]["name"]+"�v��W�ƦW�� 7 ���u"+setup[post_season_team_8[6]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_8[1], post_season_team_8[6]);
					
					break;
				}
				case 2:
				{
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i�� "HIC"C �դK�j��"NOR"���O�ƦW�� 3 ���u"+setup[post_season_team_8[2]]["name"]+"�v��W�ƦW�� 6 ���u"+setup[post_season_team_8[5]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_8[2], post_season_team_8[5]);
					
					break;
				}
				case 3:
				{
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i�� "HIC"D �դK�j��"NOR"���O�ƦW�� 4 ���u"+setup[post_season_team_8[3]]["name"]+"�v��W�ƦW�� 5 ���u"+setup[post_season_team_8[4]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_8[3], post_season_team_8[4]);
					
					break;
				}
				case 4:
				{
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i��"HIY"�|�j�ǨM��"NOR"���O A �ծʯŶ���u"+setup[post_season_team_4[0]]["name"]+"�v��W D �ծʯŶ���u"+setup[post_season_team_4[3]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_4[0], post_season_team_4[3]);
					
					break;
				}
				case 5:
				{
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i��"HIY"�|�j�ǨM��"NOR"���O B �ծʯŶ���u"+setup[post_season_team_4[1]]["name"]+"�v��W C �ծʯŶ���u"+setup[post_season_team_4[2]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_4[1], post_season_team_4[2]);
					
					break;
				}
				case 6:
				{
				CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���۶i��� "HIC+season_year+NOR" �y�u"HIY"�`"NOR YEL"�a�x��"NOR"���O�u"+setup[post_season_team_2[0]]["name"]+"�v��W�u"+setup[post_season_team_2[1]]["name"]+"�v�A���ɱN�b 2 ������}�l�C");
					
					post_season_level++;
					new_game(post_season_team_2[0], post_season_team_2[1]);
					
					break;
				}
				case 7:
				{
					post_season_level++;
					CHANNEL_D->channel_broadcast("sport", HIR"�u��"NOR RED"��"NOR" ���y�u�Ҧ��u�ɻP�u���ɤw�g���������A�ڭ̤U�@�Ӳy�u�A���I�I");
					break;
				}
				case 8:
				{
					post_season_level = 0;
					post_season_start = 0;
					season_year++;
					
					CHANNEL_D->channel_broadcast("sport", HIG"���s"NOR GRN"�y�u"NOR" �s���� "HIC+season_year+NOR" �y�u�}�l�A�Ҧ����ɲy���}�l�i����ɰt��C");
					
					post_season_team_8 = allocate(0);
					post_season_team_4 = allocate(0);
					post_season_team_2 = allocate(0);
					post_season_eliminate_teams = allocate(0);
					
					record[season_year] = allocate_mapping(0);
					record[season_year][RECORD_TEAM] = allocate_mapping(0);
					record[season_year][RECORD_PLAYER] = allocate_mapping(0);
					record[season_year][RECORD_SPECIAL] = allocate_mapping(0);
		
					reset_all_opponents();
					
					foreach(string id in season)
					{
						setup[id]["keepwins"] = 0;
						setup[id]["keeploses"] = 0;
					}	
			
					save();
					break;	
				}
			}
		}
		else
		{
			// �B�z���ɰt��
			foreach(string id in season)
			{
				if( undefinedp(setup[id]) ) continue;
		
				// �M����A�s�����ɷǳƶ}�l
				if(setup[id]["status"] == STATUS_IDLE )
				{
					string opponent_id = 0;
		
					// �y���]�w���~�A�����t��
					if( !valid_setup(id, 1) )
					{
						leave_season(id);
						continue;
					}
		
					// �Y�w�L���A���s�C��
					if( !sizeof(setup[id]["opponents"]) )
						setup[id]["opponents"] = copy(sort_array(season, (: random(2) ? 1 : -1 :))) - ({ id });
		
					// �j�M�ثe�S�����ɪ����
					foreach(string teamid in setup[id]["opponents"])
					{
						if( setup[teamid]["status"] == STATUS_IDLE )
						{
							if( !valid_setup(teamid, 1) )
							{
								leave_season(teamid);
								continue;
							}
		
							opponent_id = teamid;
							break;
						}
					}
		
					// �䤣����A���ݤU�� heartbeat �A���s�j�M
					if( !stringp(opponent_id) )
					{
						// �s�� 180 ��(40min)�䤣����A���s�C��
						if( ++setup[id]["no_opponent"] > 240 )
						{
							setup[id]["no_opponent"] = 0;
							setup[id]["opponents"] = copy(sort_array(season, (: random(2) ? 1 : -1 :))) - ({ id });
						}
						continue;
					}
		
					setup[id]["no_opponent"] = 0;
					setup[opponent_id]["no_opponent"] = 0;
					
					// �H���M�w�D�ȳ�
					if( !random(2) )
					{
						//CHANNEL_D->channel_broadcast("sport", HIC+"�u��"NOR CYN"�t�� "NOR+setup[id]["username"]+"���u"+setup[id]["name"]+"�v�P"+setup[opponent_id]["username"]+"���u"+setup[opponent_id]["name"]+"�v�N�� "+(PREPARETIME/60)+" ������i��y�ɡC");
						new_game(id, opponent_id);
					}
					else
					{
						//CHANNEL_D->channel_broadcast("sport", HIC+"�u��"NOR CYN"�t�� "NOR+setup[opponent_id]["username"]+"���u"+setup[opponent_id]["name"]+"�v�P"+setup[id]["username"]+"���u"+setup[id]["name"]+"�v�N�� "+(PREPARETIME/60)+" ������i��y�ɡC");
						new_game(opponent_id, id);
					}
				}
			}
		}
	}
	play_game();
}

// �����Y�ɾԪp
void set_listener(string listener_id, string target_team)
{	
	if( !stringp(target_team) )
	{
		foreach(string team, mapping data in setup)
			setup[team]["listener"] -= ({ listener_id });
	}
	else
	{
		setup[target_team]["listener"] |= ({ listener_id });
	}
}

int remove()
{
	return save();
}

void fix_setup()
{
	/*
	foreach(string id, mapping data in setup)
	{

	}
*/
	save();
}

void fix_record()
{

}

int query_season_year()
{
	return season_year;	
}

int is_in_post_season()
{
	return post_season_start && post_season_level;	
}

string *query_post_season_team(int number)
{
	switch(number)
	{
		case 8:
			return post_season_team_8;
			break;
		case 4:
			return post_season_team_4;
			break;
		case 2:
			return post_season_team_2;
			break;
	}
}

string *query_post_season_eliminate_teams()
{
	return post_season_eliminate_teams;
}

void add_message(string id, string message)
{
	if( undefinedp(setup[id]) ) return;	
	
	if( !arrayp(setup[id]["messages"]) )
		setup[id]["messages"] = ({ message });
	else
		setup[id]["messages"] += ({ message });
}

void create()
{
	backup();
	
	if( !restore_object(DATA) )
	{
		setup = allocate_mapping(0);
		season = allocate(0);
		games = allocate(0);
		record = allocate_mapping(0);
		
		save();
	}

	set_heart_beat(1);
}

string query_name()
{
	return "�βy�t��(BASEBALL_D)";
}
