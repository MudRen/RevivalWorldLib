/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <object.h>
#include <areadata.h>
#include <citydata.h>
#include <daemon.h>
#include <location.h>

/* 讀入 area_room  */
object load_zoneroom(string zone, int num, int z)
{
	object zoneroom;
	
	if( !stringp(zone) || !stringp(zone) || !intp(z) ) return 0;
		
	foreach( object child in children(ZONEROOM) )
		if( query("zone", child) == zone  && query("z", child) == z &&  query("num", child) == num   )
			return child;

	zoneroom = new(ZONEROOM);
	
	set("zone", zone, zoneroom);
	set("num", num, zoneroom);
	set("z", z, zoneroom);
	
        return zoneroom;
}


// 提供 getdir 以大幅提升效能
varargs string *zone_roomfiles(array loc, string* getdir)
{
	int header;
	string dir, file;

	if( CITY_D->is_city_location(loc) )
		dir = CITY_NUM_ROOM(loc[CITY], loc[NUM]);
	else if( AREA_D->is_area_location(loc) )
		dir = AREA_NUM_ROOM(loc[AREA], loc[NUM]);
	
	file = loc[X]+"_"+loc[Y]+"_%*s";
	header = file[0];

	if( undefinedp(getdir) )
		return map(filter_array(get_dir(dir), (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
	else
		return map(filter_array(getdir, (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
}
