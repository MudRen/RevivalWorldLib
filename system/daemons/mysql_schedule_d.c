/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mysql_schedule_d.c
 * Author : Clode@Revival World
 * Date   : 2005-12-24
 * Note   : MySQL Schedule 精靈
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <map.h>
#include <citydata.h>

// 分散處理
void distributed_refresh_city_3dmap(array data, string *all_sql)
{
	if( !sizeof(data) )
	{
		string sql;
		mixed handle;
		
		if( !intp(handle = MYSQL_D->connect()) )
			return;
			
		MYSQL_D->exec(handle, "DELETE FROM city3dmap");

		foreach(sql in all_sql)
			MYSQL_D->exec(handle, sql);

		MYSQL_D->updatetime(handle, "city3dmap");
		MYSQL_D->close(handle);
		
		return;
	}
	else
	{
		string city;
		int num, x, y, floor;
		mapping citydata;
		string *sql_tmp;
		object room;

		city = data[0];
		num = data[1];

		citydata = CITY_D->query_city_data(city, num)["data"];
	
		sql_tmp = ({"INSERT INTO `city3dmap` VALUES("+
			"'" + city + "'," + 
			num + ","+
			"'" + (CITY_D->query_city_idname(city)||"")+"',"+
			"'" + (CITY_D->query_city_idname(city, num)||"")+"',"+
			"NOW(),'"
		});
		
		for(y=0;y<100;y++)
		for(x=0;x<100;x++)
		{
			if( !undefinedp(citydata[y][x][ROOM])  )
			{
				if( citydata[y][x][ROOM] == "wizhall" )
				{
					sql_tmp += ({"bw"+(floor+1)+","});	
				}
				else
				{
					room = load_object(CITY_ROOM_MODULE(city, num, x, y, citydata[y][x][ROOM]));
					floor = query("floor", room);
					sql_tmp += ({"bp"+(floor+1)+"o"+query("owner", room)+","});
				}
			}
			else
				sql_tmp += ({citydata[y][x][TYPE]+","});
		}
		
		sql_tmp += ({"')"});
	
		all_sql += ({ remove_ansi(implode(sql_tmp, "")) });
		
		data = data[2..];
		
		call_out((:distributed_refresh_city_3dmap:), 2, data, all_sql);
	}
	
}


// Called by TIME_D
void refresh_city_3dmap()
{
	string city;
	int num;
	array data = allocate(0);

	foreach(city in sort_array(CITY_D->query_cities(), (: CITY_D->query_city_info($1, "totalflourish") < CITY_D->query_city_info($2, "totalflourish") ? 1: -1 :)))
	for(num=0;CITY_D->city_exist(city, num);num++)
		data += ({ city, num });
	
	distributed_refresh_city_3dmap(data, allocate(0));
}

// 分散處理
void distributed_refresh_city_2dmap(array data, string *all_sql)
{
	if( !sizeof(data) )
	{
		string sql;
		mixed handle;
		
		if( !intp(handle = MYSQL_D->connect()) )
			return;
			
		MYSQL_D->exec(handle, "DELETE FROM city2dmap");

		foreach(sql in all_sql)
			MYSQL_D->exec(handle, sql);

		MYSQL_D->updatetime(handle, "city2dmap");
		MYSQL_D->close(handle);
		
		return;
	}
	else
	{
		string city;
		int num, x, y, floor;
		mapping citydata;
		string *sql_tmp;
		object room;

		city = data[0];
		num = data[1];

		citydata = CITY_D->query_city_data(city, num)["data"];
	
		sql_tmp = ({"INSERT INTO `city3dmap` VALUES("+
			"'" + city + "'," + 
			num + ","+
			"'" + (CITY_D->query_city_idname(city)||"")+"',"+
			"'" + (CITY_D->query_city_idname(city, num)||"")+"',"+
			"NOW(),'"
		});
		
		for(y=0;y<100;y++)
		for(x=0;x<100;x++)
		{
			if( !undefinedp(citydata[y][x][ROOM])  )
			{
				if( citydata[y][x][ROOM] == "wizhall" )
				{
					sql_tmp += ({"bw"+(floor+1)+","});	
				}
				else
				{
					room = load_object(CITY_ROOM_MODULE(city, num, x, y, citydata[y][x][ROOM]));
					floor = query("floor", room);
					sql_tmp += ({"bp"+(floor+1)+"o"+query("owner", room)+","});
				}
			}
			else
				sql_tmp += ({citydata[y][x][TYPE]+","});
		}
		
		sql_tmp += ({"')"});
	
		all_sql += ({ remove_ansi(implode(sql_tmp, "")) });
		
		data = data[2..];
		
		call_out((:distributed_refresh_city_2dmap:), 2, data, all_sql);
	}
	
}
// Called by TIME_D
void refresh_city_2dmap()
{
	string city;
	int num;
	array data = allocate(0);

	foreach(city in CITY_D->query_cities())
	for(num=0;CITY_D->city_exist(city, num);num++)
		data += ({ city, num });
	
	distributed_refresh_city_2dmap(data, allocate(0));
}

void refresh_world_map()
{
	
	
}

// Called by TIME_D
void refresh_who()
{
	function filter;
	mixed handle;
	string sql = "";
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	MYSQL_D->exec(handle, "DELETE FROM who");
	
	sql = "INSERT INTO `who` VALUES";
	
	filter = COMMAND_D->find_command_object("who")->query_sort_user_fp();
	foreach(object ob in sort_array(users(), filter) )
	{
		if( !environment(ob) || !ob->is_user_ob() ) continue;
		
		sql += "('"+
			replace_string((CITY_D->query_city_name(query("city", ob))||""),"'", "\\'")+"','"+
			replace_string((ENTERPRISE_D->query_enterprise_color_id(query("enterprise", ob))||""),"'", "\\'")+"','"+
			replace_string((query("title", ob)||""),"'", "\\'")+"','"+
			replace_string((query("nickname", ob)||""),"'", "\\'")+"','"+
			replace_string(ob->query_idname(),"'", "\\'")+"','"+
			replace_string((ob->query_status()||""),"'", "\\'")+"','"+
			replace_string((query("web/avatar", ob)||"/rwpweb/images/nopicture.png"),"'", "\\'")+"','"+
			replace_string((query("web/homepage", ob)||""),"'", "\\'")+"'),";
	}

	sql = remove_ansi(sql[0..<2]);

	
	MYSQL_D->exec(handle, sql);
	MYSQL_D->updatetime(handle, "who");
	MYSQL_D->close(handle);
}

// Called by HISTORY_D
void refresh_history()
{
	array game_history;
	mixed handle;
	string sql = "";
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	MYSQL_D->exec(handle, "DELETE FROM history");
	
	sql = "INSERT INTO `history` VALUES";
	
	game_history = HISTORY_D->query_history();
	
	for(int i=0;i<sizeof(game_history);i+=3)
		sql += "('"+TIME_D->replace_ctime(game_history[i])+"','"+replace_string(game_history[i+1],"'", "\\'")+"','"+replace_string(game_history[i+2],"'", "\\'")+"'),";

	sql = remove_ansi(sql[0..<2]);

	MYSQL_D->exec(handle, sql);
	MYSQL_D->updatetime(handle, "history");
	MYSQL_D->close(handle);
}

int twmudlist_sort(mapping mnlmudlist, string mud1, string mud2)
{
	string mud1_chinese = mnlmudlist[mud1]["MUD_CHINESE_NAME"];
	string mud2_chinese = mnlmudlist[mud2]["MUD_CHINESE_NAME"];
	string mud1_english = mnlmudlist[mud1]["MUD_ENGLISH_NAME"];
	string mud2_english = mnlmudlist[mud2]["MUD_ENGLISH_NAME"];

	if( sizeof(mud1_chinese) != sizeof(mud2_chinese) )
		return sizeof(mud1_chinese) > sizeof(mud2_chinese) ? 1 : -1;
	
	return strcmp(mud1_chinese || mud1_english, mud2_chinese || mud2_english);
}

// Called by TIME_D
void refresh_twmudlist()
{
	mixed handle;
	string ipport;
	string ip;
	int port;
	int status;
	int users;
	string lasttime;
	string sql = "";
	mapping muds;
	mapping data;
	string *mudnamesort;
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	muds = MNLMUDLIST_D->query_mnlmudlist();
	
	MYSQL_D->exec(handle, "DELETE FROM twmudlist");
	
	sql = "INSERT INTO `twmudlist` VALUES";
	
	mudnamesort = sort_array(keys(muds)-({0}), (: twmudlist_sort($(muds), $1, $2) :)) ;
		
	foreach(ipport in mudnamesort)
	{
		data  = muds[ipport];

		if( !stringp(ipport) || !mapp(data) ) continue;
		
		sscanf(ipport, "%s %d", ip, port);
		
		if( data["CONNECT_FAILED_TIMES"] > 0 )
			status = 0;
		else
			status = 1;
		
		if( data["USERS"] > 0 )
			users = data["USERS"];
		else if( !data["USERS_COUNT_PARSE"] )
			users = -1;
		else
			users = 0;

		if( data["LAST_CONTACT_TIME"] )
			lasttime = TIME_D->replace_ctime(data["LAST_CONTACT_TIME"]);
		else
			lasttime = "";
			
		sql += "('"+
			(data["MUD_CHINESE_NAME"]||"--未知名稱--")+"','"+
			(data["MUD_ENGLISH_NAME"]||"--Unknown--")+"','"+
			ip+"',"+
			port+","+
			status+","+
			users+",'"+
			lasttime+"'),";
		
	}
	
	sql = remove_ansi(sql[0..<2]);
	MYSQL_D->exec(handle, sql);
	MYSQL_D->updatetime(handle, "twmudlist");
	MYSQL_D->close(handle);
}

// Called by TIME_D
void refresh_top_rich()
{
	mixed handle;
	string sql = "";
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	MYSQL_D->exec(handle, "DELETE FROM top_rich");
	
	sql = "INSERT INTO `top_rich` VALUES";
	
	foreach(array data in TOP_D->query_top("rich"))
	{
		sql += "('"+
			replace_string(data[3],"'", "\\'")+"','"+
			replace_string((CITY_D->query_city_name(data[4]) || ""),"'", "\\'")+"','"+
			NUMBER_D->number_symbol(data[1])+"','"+
			TIME_D->replace_ctime(data[2])[0..9]+"'),";
	}

	sql = remove_ansi(sql[0..<2]);

	MYSQL_D->exec(handle, sql);
	MYSQL_D->updatetime(handle, "top_rich");
	MYSQL_D->close(handle);
}

void set_captcha(string id, string code)
{
	mixed handle;
	string sql;
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	MYSQL_D->exec(handle, "DELETE FROM captcha WHERE id='"+id+"'");
	
	sql = "INSERT INTO `captcha` VALUES ('"+id+"','"+code+"')";
	
	MYSQL_D->exec(handle, sql);
	MYSQL_D->close(handle);
}

void delete_captcha(string id)
{
	mixed handle;
	
	if( !intp(handle = MYSQL_D->connect()) )
		return;
	
	MYSQL_D->exec(handle, "DELETE FROM captcha WHERE id='"+id+"'");
	
	MYSQL_D->close(handle);
}

void create()
{


}
string query_name()
{
	return "MYSQL 排程系統(MYSQL_SCHEDULE_D)";
}
