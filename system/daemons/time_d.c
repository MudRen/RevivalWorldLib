/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : time_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-03-04
 * Note   : 時間精靈, 實際一小時==遊戲一天, 經濟世界中的時間處理需要
 *          十分的精確, 故採用 heart_beat 0.1 秒處理時間內容
 * Update :
 *  o 2002-09-29 Clode 修正因 0.1 秒 heart_beat() 精確度不足造成 gametime 的誤差  
 *  o 2003-08-29 Clode 新增 crontab 工作排程功能
 -----------------------------------------
 */

#define DATA_PATH		"/data/daemon/time.o"

#include <ansi.h>
#include <daemon.h>
#include <localtime.h>
#include <time.h>
#include <mudlib.h>
#include <message.h>

private int gametime;
private nosave int realtime;

private nosave int tick;
private nosave mapping event = allocate_mapping(0);

private nosave int *real_time = allocate(6);
private nosave int *game_time = allocate(6);

void happy_game_new_year()
{
	string msg = "\n"MUD_CHINESE_NAME HIW+CHINESE_D->chinese_number(game_time[YEAR])+HIC"年終於來到，舉世歡騰！！！\n"NOR;
	
	shout(msg+msg+msg);
}

void happy_real_new_year()
{
	string msg = WHT"\n西元" HIW+CHINESE_D->chinese_number(real_time[YEAR])+WHT"年終於來到，舉世歡騰！！！\n"NOR;
	
	shout(msg+msg+msg);
}

// 遊戲時間的工作排程
private nosave array game_crontab = ({
//"min hour wday mday mon year" : function
"0 * * * * *"		, (: NATURE_D->game_hour_process() :),			"每小時自然環境變動",
"0 0 * 0 0 *"		, (: happy_game_new_year :),					"每年發一次紅包",
"0 0 * 0 * *"		, (: SALARY_D->game_month_process() :),			"每月處理一次薪資",
"0 0 * 0 * *"		, (: NATURE_D->game_month_process() :),			"每月處理一次季節變化",
"30 0 * 0 * *"		, (: MONEY_D->calculate_all_GNP() :),			"每月算一次GNP",
"40 0 * 0 * *"		, (: CITY_D->start_fund_handler() :),	"每月處理一次政府經費支出與稅收收入",
});

// 真實時間的工作排程
private nosave array real_crontab = ({
//"min hour wday mday mon year" : function
"*/4 * * * * *"		, (: CITY_D->time_distributed_save() :),		"每 4 分鐘城市資料分散儲存",
"*/10 * * * * *"	, (: NEWS_D->broadcast_news() :),			"每 10 分鐘廣播新聞",
"*/11 * * * * *"	, (: save_object(DATA_PATH) :),				"每 11 分鐘紀錄一次 TIME_D 資料",
"*/12 * * * * *"	, (: MNLMUDLIST_D->distributed_connect() :),            "每 12 分鐘更新 MNLMUDLIST 紀錄",
"*/13 * * * * *"	, (: TOP_D->save() :),					"每 13 分鐘紀錄一次 TOP_D 資料",
"*/14 * * * * *"	, (: MYSQL_SCHEDULE_D->refresh_twmudlist() :),          "每 14 分鐘更新 mysql twmudlist",
"3 * * * * *"		, (: LOGIN_D->time_schedule() :),			"每小時紀錄與重整登入資料",
"4 * * * * *"		, (: LABOR_D->save_data() :),				"每小時紀錄失業勞工資料",
"5 * * * * *"		, (: SYSDATABASE_D->save() :),				"每小時紀錄系統資料庫",
"6 * * * * *"		, (: MAP_D->save() :),					"每小時紀錄一次地圖上的物品",
"7 * * * * *"		, (: HTML_D->create_html() :),				"每小時建置一次網頁頁面",
"8 * * * * *"		, (: LOGIN_D->reset_login_count(real_time[HOUR]) :), 	"每小時計算一次 login count",
"9 * * * * *"		, (: MNLMUDLIST_D->reset_maxuser(real_time[HOUR]) :), 	"每小時計算一次 maxuser",
"10 * * * * *"		, (: MYSQL_SCHEDULE_D->refresh_who() :),		"每小時更新一次 mysql who",
"11 * * * * *"		, (: MYSQL_SCHEDULE_D->refresh_city_3dmap() :),		"每小時更新一次 3dmap",
"12 * * * * *"		, (: MYSQL_SCHEDULE_D->refresh_top_rich() :),		"每小時更新一次 top_rich",
"13 * * * * *"		, (: EXCHANGE_D->process_per_hour() :),			"每小時處理一次匯率變動",
"14 * * * * *"		, (: EMOTE_D->save() :),				"每小時儲存一次 EMOTE_D 資料",
"15 * * * * *"		, (: AREA_D->time_distributed_save() :),		"每小時郊區資料分散儲存",
"16 * * * * *"		, (: PRODUCT_D->save() :),				"每小時儲存一次產品資料",
"17 * * * * *"		, (: BOSS_D->process_per_hour() :),			"每小時處理一次 BOSS 程序",
"0 */12 * * * *"	, (: AREA_D->downgrade_section_influence() :),		"每 12 小時下降所有城市 20% 的郊區資源佔領點數",
"57 2 * * * *"		, (: PPL_LOGIN_D->reset_newchar() :),			"每天重設一次新角色註冊數",
"57 4 * * * *"		, (: SYSTEM_D->prepare_to_save_all(3) :), 		"每天儲存所有資料一次",
"57 5 * * * *"		, (: CLEAN_D->clean_user() :),				"每天清除一次玩家資料",
"50 15 * * * *"		, (: STOCK_D->refresh_stock_data() :),			"每天更新一次台股資料",
"10 13 * * * *"		, (: BATTLEFIELD_D->start_join() :),			"開放戰場報名",
"40 20 * * * *"		, (: BATTLEFIELD_D->start_join() :),			"開放戰場報名",
"40 21 * * * *"		, (: BATTLEFIELD_D->start_join() :),			"開放戰場報名",
"5 0 * 0 * *"		, (: FUTURES_D->process_per_month() :),			"當月期貨損益結算",
"0 0 * 0 0 *"		, (: happy_real_new_year :),				"真實時間過年",
});

private nosave mapping crontab_process = allocate_mapping(0);

// 可取代 call_out 之功能, 並以 0.1 秒為時間單位
void add_event(function fevent, int delay_time)
{
	if( !sizeof(event) || !event[fevent] ) 
		event = ([ fevent : delay_time ]);
	else
		event[fevent] = delay_time;
}

// 執行事件
void exec_event(function fevent)
{
	if( objectp(function_owner(fevent)) )
		evaluate(fevent);

	map_delete(event, fevent);
}

string replace_ctime(int t)
{
	string month, ctime = ctime(t);

	switch(ctime[4..6])
	{
		case "Jan"	:	month = "01";break;
		case "Feb"	:	month = "02";break;
		case "Mar"	:	month = "03";break;
		case "Apr"	:	month = "04";break;
		case "May"	:	month = "05";break;
		case "Jun"	:	month = "06";break;
		case "Jul"	:	month = "07";break;
		case "Aug"	:	month = "08";break;
		case "Sep"	:	month = "09";break;
		case "Oct"	:	month = "10";break;
		case "Nov"	:	month = "11";break;
		case "Dec"	:	month = "12";break;
	}

	return sprintf("%s/%s/%s %s", month, (ctime[8]==' '?"0"+ctime[9..9]:ctime[8..9]), ctime[<4..<1], ctime[11..15]);
}

string season_period(int m)
{
	switch(m)
	{
		case 2..4:	return "春";
		case 5..7:	return "夏";
		case 8..10:	return "秋";
		case 11:	return "冬";
		case 0..1:	return "冬";
		default:	return 0;
	}
}

string week_period(int w)
{
	switch(w)
	{       
		case 0: 	return "日";
		case 1: 	return "一";
		case 2: 	return "二";
		case 3: 	return "三";
		case 4: 	return "四";
		case 5: 	return "五";
		case 6: 	return "六";
		default:	return 0;
	}
}

string hour_period(int h)
{
	switch(h)
	{
		case 0..5: 	return "凌晨";
		case 6..11: 	return "上午";
		case 12:	return "中午";
		case 13..18:	return "下午";
		case 19..23:	return "晚上";
		default:	return 0;
	}
}

/* 回傳時間陣列 ({ 分 時 禮 日 月 年 }) */
int *query_gametime_array()
{
	return game_time;
}	

int *query_realtime_array()
{
	return real_time;
}

/* 回傳數位時間表示 下午 3:39 */
string gametime_digital_clock()
{
	int h = game_time[HOUR];
	int m = game_time[MIN];

	return hour_period(h)+" "+(h==12 || (h%=12)>9?""+h:" "+h)+":"+(m>9?""+m:"0"+m);
}
string realtime_digital_clock()
{
	int h = real_time[HOUR];
	int m = real_time[MIN];

	return hour_period(h)+" "+(h==12 || (h%=12)>9?""+h:" "+h)+":"+(m>9?""+m:"0"+m);
}

string time_description(string title, int *t)
{
	return sprintf( NOR WHT+title+NOR"%s年﹐%s﹐%s月%s日星期%s﹐%s%s點%s分"NOR
	    ,t[YEAR]==1?"元":CHINESE_D->chinese_number(t[YEAR])
	    ,season_period(t[MON])
	    ,!t[MON]?"元":CHINESE_D->chinese_number(t[MON]+1)
	    ,CHINESE_D->chinese_number(t[MDAY]+1)
	    ,week_period(t[WDAY])
	    ,hour_period(t[HOUR])
	    ,CHINESE_D->chinese_number(t[HOUR] > 12 ? t[HOUR]%12 : t[HOUR])
	    ,CHINESE_D->chinese_number(t[MIN])
	);
}

string game_time_description()
{
	return time_description(MUD_CHINESE_NAME, game_time);
}

string real_time_description()
{
	return time_description("公元", real_time);
}

int query_game_time()
{
	return gametime*60;
}

int query_real_time()
{
	return realtime;
}

int *analyse_time(int t)
{
	int *ret = allocate(6);
	string ctime;

	ctime = ctime(t);

	sscanf(ctime,"%*s %*s %d %d:%d:%*d %d",ret[MDAY], ret[HOUR], ret[MIN], ret[YEAR]);

	switch(ctime[0..2])
	{
		case "Sun": ret[WDAY] = 0; break;
		case "Mon": ret[WDAY] = 1; break;
		case "Tue": ret[WDAY] = 2; break;
		case "Wed": ret[WDAY] = 3; break;
		case "Thu": ret[WDAY] = 4; break;
		case "Fri": ret[WDAY] = 5; break;
		case "Sat": ret[WDAY] = 6; break;
		default	  : return 0;
	}

	switch(ctime[4..6])
	{
		case "Jan": ret[MON] = 0; break;
		case "Feb": ret[MON] = 1; break;
		case "Mar": ret[MON] = 2; break;
		case "Apr": ret[MON] = 3; break;
		case "May": ret[MON] = 4; break;
		case "Jun": ret[MON] = 5; break;
		case "Jul": ret[MON] = 6; break;
		case "Aug": ret[MON] = 7; break;
		case "Sep": ret[MON] = 8; break;
		case "Oct": ret[MON] = 9; break;
		case "Nov": ret[MON] = 10; break;
		case "Dec": ret[MON] = 11; break;
		default	  : return 0;
	}
	ret[MDAY] -= 1;
	return ret;
}

void process_crontab(array crontab, int *timearray)
{
	int i, j, row, divider, fit, timecost, crontabsize;
	string s, script, note, *timescript;
	function fp;

	crontabsize = sizeof(crontab);

	for(row=0;row<crontabsize;row+=3)
	{
		reset_eval_cost();
		script = crontab[row];
		fp = crontab[row+1];
		note = crontab[row+2];

		timescript = allocate(6);
		
		if( sscanf(trim(script), "%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s%*(( |\t)+)%s", 
			timescript[0],
			timescript[1],
			timescript[2],
			timescript[3],
			timescript[4],
			timescript[5]) != 11 ) continue;
		
		for(i=0;i<6;i++)
		{
			fit = 0;

			foreach(s in explode(timescript[i], ","))
			{
				j = to_int(s);
				
				if( !undefinedp(j) )
				{
					if( j == timearray[i] )
					{
						fit = 1;
						break;
					}
				}
				else if( s == "*" || (sscanf(s, "*/%d", divider) && !(timearray[i]%divider)) )
				{
					fit = 1;
					break;
				}
			}
			
			if( !fit ) break;
		}
		
		if( !fit ) 
			continue;

		reset_eval_cost();
		timecost = time_expression {
			catch(evaluate(fp));
		};
		
		if( timecost > 1000000 )
			CHANNEL_D->channel_broadcast("sys", sprintf("TIME_D: crontab '%s'(%.6f sec) %s", script, timecost/1000000., note));
	}
}

// 獨立出來每秒執行函式
void process_per_second()
{

	
}

int reset_gametime(int time)
{
	gametime = time;
	return save_object(DATA_PATH);	
}

/* 遊戲時間每一分鐘(即實際時間每2.5秒)執行一次 process_gametime */
void process_gametime()
{
	game_time = analyse_time(++gametime * 60);
	game_time[YEAR] -= 1970;

	process_crontab(game_crontab, game_time);
}

/* 真實時間每一秒鐘執行一次 process_realtime */
void process_realtime()
{
	array localtime = localtime(realtime);
	
	real_time = ({ 
		localtime[LT_MIN], 
		localtime[LT_HOUR], 
		localtime[LT_WDAY], 
		localtime[LT_MDAY]-1, 
		localtime[LT_MON], 
		localtime[LT_YEAR] });
	
	process_per_second();

	if( !localtime[LT_SEC] )
		process_crontab(real_crontab, real_time);
}

// 每 1 秒運算一次
// 實際一天等於遊戲一月

void heart_beat()
{
	// 避免因系統延遲而遺失
	while(realtime < time())
	{
		realtime++;
		process_realtime();
	}

	// 執行 event 處理
	if( sizeof(event) )
	foreach( function fevent, int delay_time in event )
		if( !--event[fevent] )
			exec_event(fevent);

	// 每 2 秒相當於遊戲一分鐘, time 每增加 1 代表遊戲一分鐘
	if( !(++tick % 2) ) 
		process_gametime();
}

private void create()
{
	if( !restore_object(DATA_PATH) )
		save_object(DATA_PATH);
	
	game_time = allocate(6);
	real_time = allocate(6);
	event = allocate_mapping(0);
	
	realtime = time();

	process_gametime();
	set_heart_beat(1);
}

int remove()
{
	return save_object(DATA_PATH);
}

string query_name()
{
	return "時間系統(TIME_D)";
}
