/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : estate.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-17
 * Note   : estate 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>
#include <citydata.h>
#include <roommodule.h>
#include <map.h>

inherit COMMAND;

string help = @HELP
    房地產查詢指令。

estate			- 查詢自己的房地產資料
estate '玩家ID'		- 查詢其他市民的房地產資料(市長與官員指令)

HELP;

string showtime(int time)
{
	if(time > 86400)
		return time/86400+"D";
	else
		return time/3600+"H";
}

string time_description(string type, int regtime)
{
	if( type == "land" )
		return HIR+showtime( regtime + 3*24*60*60 - time() )+NOR;
	else if( time() - regtime > 365*24*60*60 )
		return GRN+"(古蹟)"+HIG+showtime(time() - regtime)+NOR;
	else
		return HIG+showtime(time() - regtime)+NOR;
}

varargs string show_estate(string owner, array myestates, string title, int enterprise)
{
	string level;
	string list;
	array loc;
	mapping estdata;
	mapping buildingtable = BUILDING_D->query_building_table();
	int floors, totalfloors, lands, totallands, flourish, totalflourish;
	int value, totalvalue;
	string *getdir;
	string dir;

	list = HIW"房地產種類  "HIG"建"NOR"/"HIR"回收"NOR"時間 位置座標    防禦 佔地 房間 總繁榮 總價值\n";
	
	if( !enterprise )
		list += NOR WHT"─────────────────────────────────────\n"NOR;
	
	foreach(estdata in sort_array(myestates, (: strcmp($1["type"], $2["type"]) || ($1["regtime"] - $2["regtime"]) :)))
	{
		lands = 0;
		floors = 0;
		flourish = 0;
		value = 0;
		
		loc = restore_variable(estdata["walltable"][0]);
		
		if( CITY_D->is_city_location(loc) )
		{
			value = ESTATE_D->query_estate_value(loc);

			if( dir != CITY_NUM_ROOM(loc[CITY], loc[NUM]) )
			{
				dir = CITY_NUM_ROOM(loc[CITY], loc[NUM]);
				getdir = get_dir(dir);
			}
	
			flourish = ESTATE_D->query_estate_flourish(estdata, getdir);
		}
		else
		{
			value = 0;
			flourish = 0;
		}

		lands = sizeof(estdata["walltable"]) + sizeof(estdata["roomtable"]);
		
		if( !sizeof(estdata["roomtable"]) )
			floors = 0;
		else
			floors = ESTATE_D->query_estate_floors(loc);
		
		switch(estdata["type"])
		{
			case "farm":
				level = NOR GRN"(Lv"+HIG+CITY_D->query_coor_data(loc, "growth_level")+NOR GRN")"NOR;
				break;
			case "pasture":
				level = NOR YEL"(Lv"+HIY+CITY_D->query_coor_data(loc, "growth_level")+NOR YEL")"NOR;
				break;
			case "fishfarm":
				level = NOR BLU"(Lv"+HIB+CITY_D->query_coor_data(loc, "growth_level")+NOR BLU")"NOR;
				break;
			default:
				level = ""; break;
		}
		list += sprintf("%-12s%11s %-12s%3d%% "NOR YEL"%-5d"NOR CYN"%-5d"HIC"%-7d"HIY"%s\n"NOR,
				buildingtable[estdata["type"]][ROOMMODULE_SHORT] + level,
				time_description(estdata["type"], estdata["regtime"]),
				loc_short(loc, 1),
				estdata["maintain"],
				lands,
				floors,
				flourish,
				NUMBER_D->number_symbol(value),
			    );
			    
		totalfloors += floors;
		totallands += lands;
		totalflourish += flourish;
		totalvalue += value;
	}

	if( !enterprise )
	{
		list += NOR WHT"─────────────────────────────────────\n"NOR;
		list += sprintf("%-41s"NOR YEL"%-5d"NOR CYN"%-5d"HIC"%-7d"HIY"%s\n"NOR, CITY_D->query_city_idname(loc[CITY]), totallands, totalfloors, totalflourish, NUMBER_D->number_symbol(totalvalue));
		list += NOR WHT"─────────────────────────────────────\n"NOR;
		list += HIW+title+"目前共有 "+sizeof(myestates)+" 筆房地產\n"NOR;
	}
	
	return list;
}

private void do_command(object me, string arg)
{
	object ob;
	string list, title;
	array myestates;
	string city;
	
	city = query("city", me);
	
	if( !city ) 
		return tell(me, pnoun(2, me)+"必須先加入某個城市之後才能開始擁有房地產。\n");
	
	if( find_player("clode") )
		tell(find_player("clode"), me->query_idname()+"show_estate\n");
	
	if( arg == "enhance" )
	{
		int maintain;
		string value;
		object env = environment(me);
		array loc = query_temp("location", me);
		string money_unit;
		string zone;

		zone = env->query_city() || env->query_area() ;
		
		if( !arrayp(loc) || !zone )
			return tell(me, pnoun(2, me)+"無法在這裡強化建築物的防禦工事。\n");

		//if( MAP_D->query_coor_data(loc, "owner") != me->query_id(1) )
		//	return tell(me, "這裡不是"+pnoun(2, me)+"的土地。\n");

		maintain = 100 - ESTATE_D->query_loc_estate_data(loc, "maintain");
		
		if( maintain <= 0 )
			return tell(me, "這裡的防禦不需要加強。\n");

		value = ESTATE_D->query_estate_value(loc);
		
		value = count(count(value, "*", maintain), "/", 10000);

		if( CITY_D->is_city_location(loc) )
			money_unit = MONEY_D->city_to_money_unit(zone);
		else if( AREA_D->is_area_location(loc) )
			money_unit = MONEY_D->query_default_money_unit();

		if( !me->spend_money(money_unit, value) )
			return tell(me, pnoun(2, me)+"身上的錢不足 "HIY+money(money_unit, value)+NOR"，無法強化建築物防禦工事。\n");
			
		ESTATE_D->set_loc_estate_data(loc, "maintain", 100);
		
		msg("$ME花費 "HIY+money(money_unit, value)+NOR" 將此建築物的防禦工事強化至 "HIG"100"NOR GRN"%"NOR"。\n", me, 0, 1);
		
		return;
		
	}
	else if( arg && wizardp(me) )
	{
		ob = find_player(arg);
		
		if( objectp(ob) )
			title = ob->query_idname();
		else
			title = capitalize(arg)+" ";
		
		myestates = ESTATE_D->query_player_estate(arg);
	}
	else if( arg && member_array(arg, CITY_D->query_citizens(city)) != -1 && (CITY_D->is_officer(city, me) ||  CITY_D->is_mayor(city, me)) )
	{
		myestates = ESTATE_D->query_player_estate(arg);
		title = "市民 "+capitalize(arg)+" ";
	}
	else
	{
		arg = me->query_id(1);
		myestates = ESTATE_D->query_player_estate(arg);
		title = pnoun(2, me);
	}

	if( !sizeof(myestates) )
		return tell(me, title+"目前沒有任何房地產。\n");

	list = show_estate(arg, myestates, title);

	me->more(list+"\n");
}