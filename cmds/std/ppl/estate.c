/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : estate.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-17
 * Note   : estate ���O
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
    �Цa���d�߫��O�C

estate			- �d�ߦۤv���Цa�����
estate '���aID'		- �d�ߨ�L�������Цa�����(�����P�x�����O)

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
		return GRN+"(�j��)"+HIG+showtime(time() - regtime)+NOR;
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

	list = HIW"�Цa������  "HIG"��"NOR"/"HIR"�^��"NOR"�ɶ� ��m�y��    ���m ���a �ж� �`�c�a �`����\n";
	
	if( !enterprise )
		list += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
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
		list += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		list += sprintf("%-41s"NOR YEL"%-5d"NOR CYN"%-5d"HIC"%-7d"HIY"%s\n"NOR, CITY_D->query_city_idname(loc[CITY]), totallands, totalfloors, totalflourish, NUMBER_D->number_symbol(totalvalue));
		list += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		list += HIW+title+"�ثe�@�� "+sizeof(myestates)+" ���Цa��\n"NOR;
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
		return tell(me, pnoun(2, me)+"�������[�J�Y�ӫ�������~��}�l�֦��Цa���C\n");
	
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
			return tell(me, pnoun(2, me)+"�L�k�b�o�̱j�ƫؿv�������m�u�ơC\n");

		//if( MAP_D->query_coor_data(loc, "owner") != me->query_id(1) )
		//	return tell(me, "�o�̤��O"+pnoun(2, me)+"���g�a�C\n");

		maintain = 100 - ESTATE_D->query_loc_estate_data(loc, "maintain");
		
		if( maintain <= 0 )
			return tell(me, "�o�̪����m���ݭn�[�j�C\n");

		value = ESTATE_D->query_estate_value(loc);
		
		value = count(count(value, "*", maintain), "/", 10000);

		if( CITY_D->is_city_location(loc) )
			money_unit = MONEY_D->city_to_money_unit(zone);
		else if( AREA_D->is_area_location(loc) )
			money_unit = MONEY_D->query_default_money_unit();

		if( !me->spend_money(money_unit, value) )
			return tell(me, pnoun(2, me)+"���W�������� "HIY+money(money_unit, value)+NOR"�A�L�k�j�ƫؿv�����m�u�ơC\n");
			
		ESTATE_D->set_loc_estate_data(loc, "maintain", 100);
		
		msg("$ME��O "HIY+money(money_unit, value)+NOR" �N���ؿv�������m�u�Ʊj�Ʀ� "HIG"100"NOR GRN"%"NOR"�C\n", me, 0, 1);
		
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
		title = "���� "+capitalize(arg)+" ";
	}
	else
	{
		arg = me->query_id(1);
		myestates = ESTATE_D->query_player_estate(arg);
		title = pnoun(2, me);
	}

	if( !sizeof(myestates) )
		return tell(me, title+"�ثe�S������Цa���C\n");

	list = show_estate(arg, myestates, title);

	me->more(list+"\n");
}