/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <citydata.h>
#include <object.h>
#include <location.h>
#include <daemon.h>

/* 讀入 city_room  */
object load_cityroom(string city, int num)
{
	object cityroom;
	
	if( !stringp(city) || !intp(num) ) return 0;
		
	foreach( object child in children(CITYROOM) )
		if( query("city", child) == city && query("num", child) == num )
			return child;

	cityroom = new(CITYROOM);
	
	set("city", city, cityroom);
	set("num", num, cityroom);
	
        return cityroom;
}

array arrange_city_location(int x, int y, string city, int num)
{
	return ({ x, y, 0, city, num, 0, x+"/"+y+"/0/"+city+"/"+num+"/0" });
}

// 提供 getdir 以大幅提升效能
varargs string *city_roomfiles(array loc, string* getdir)
{
	int header;
	string dir, file;

	dir = CITY_NUM_ROOM(loc[CITY], loc[NUM]);
	
	file = loc[X]+"_"+loc[Y]+"_%*s";
	header = file[0];

	if( undefinedp(getdir) )
		return map(filter_array(get_dir(dir), (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
	else
		return map(filter_array(getdir, (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
}

varargs string city_coor_short(int x, int y, string city, int num)
{
	string str = "<";
	
	if( undefinedp(x) || undefinedp(y) )
		return 0;
	
	if( !undefinedp(city) )
		str += city + " ";
	
	if( !undefinedp(num) )
		str += (num+1) + " ";
		
	str += (x+1)+","+(y+1)+">";
	
	return str;
}

varargs string loc_short(mixed city, int num, int x, int y)
{
	if( arrayp(city) )
	{
		if( CITY_D->city_exist(city[CITY]) )
			return NOR WHT"<"NOR+(num?"":CITY_D->query_city_id(city[CITY])+" ")+HIW+(city[NUM]+1)+NOR" "HIW+(city[X]+1)+NOR WHT","HIW+(city[Y]+1)+NOR WHT">"NOR;
		else if( AREA_D->area_exist(city[CITY]) )
			return NOR WHT"<"NOR+(num?"":AREA_D->query_area_id(city[CITY])+" ")+HIW+(city[NUM]+1)+NOR" "HIW+(city[X]+1)+NOR WHT","HIW+(city[Y]+1)+NOR WHT">"NOR;
	}
	else if( stringp(city) )
	{
		if( CITY_D->city_exist(city) )
			return NOR WHT"<"NOR+CITY_D->query_city_id(city)+" "HIW+(num+1)+NOR" "HIW+(x+1)+NOR WHT","HIW+(y+1)+NOR WHT">"NOR;
		else if( AREA_D->area_exist(city) )
			return NOR WHT"<"NOR+AREA_D->query_area_id(city)+" "HIW+(num+1)+NOR" "HIW+(x+1)+NOR WHT","HIW+(y+1)+NOR WHT">"NOR;
	}

	return 0;
}

int belong_to_government(string owner)
{
	return stringp(owner) && owner[0..9] == "GOVERNMENT";
}

int belong_to_enterprise(string owner)
{
	return stringp(owner) && owner[0..9] == "ENTERPRISE";
}

int belong_to_individual(string owner)
{
	return stringp(owner) && owner[0..9] != "GOVERNMENT" && owner[0..9] != "ENTERPRISE";
}
