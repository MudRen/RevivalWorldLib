/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-20
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */


#include <object.h>
#include <areadata.h>
#include <location.h>

/* 讀入 area_room  */
object load_arearoom(string area, int num)
{
	object arearoom;
	
	if( !stringp(area) || !intp(num) ) return 0;
		
	foreach( object child in children(AREAROOM) )
		if( query("area", child) == area && query("num", child) == num )
			return child;

	arearoom = new(AREAROOM);
	
	set("area", area, arearoom);
	set("num", num, arearoom);
	
        return arearoom;
}

array arrange_area_location(int x, int y, string area, int num)
{
	return ({ x, y, 0, area, num, 0, x+"/"+y+"/0/"+area+"/"+num+"/0" });
}

array arrange_defaultmap_location(int x, int y, string map, int num)
{
	return ({ x, y, 0, map, num, 0, x+"/"+y+"/0/"+map+"/"+num+"/0" });
}

// 提供 getdir 以大幅提升效能
varargs string *area_roomfiles(array loc, string* getdir)
{
	int header;
	string dir, file;

	dir = AREA_NUM_ROOM(loc[AREA], loc[NUM]);
	
	file = loc[X]+"_"+loc[Y]+"_%*s";
	header = file[0];

	if( undefinedp(getdir) )
		return map(filter_array(get_dir(dir), (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
	else
		return map(filter_array(getdir, (: sscanf($1, $(file)) == 1 :)), (: $(dir)+$1 :));
}

