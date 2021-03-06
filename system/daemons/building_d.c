/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : building_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-24
 * Note   : 建築物精靈
 * Update :
 *  o 2002-09-26 Clode 重寫建築物牆壁邏輯檢查函式 valid_building_logic()
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <location.h>
#include <roommodule.h>
#include <areadata.h>
#include <citydata.h>
#include <message.h>
#include <building.h>

#define WALL_TABLE	0
#define ROOM_TABLE	1

#define XX	0
#define YY	1

mapping building_table;
mapping floor_total_value;
mapping floor_value;

mapping default_building_table = ([

"land"	:	({ "荒地"			, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"farm"	: 	({ HIG"農田"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION										, -1, 1, 0, 1, 0, 1 }),
"pasture":	({ NOR YEL"牧場"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION										, -1, 1, 0, 1, 0, 1 }),
"fishfarm":	({ HIB"養殖場"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION										, -1, 1, 0, 1, 0, 1 }),
"light"	:	({ HIY"路燈"NOR 	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"pool"	:	({ HIC"水池"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"pavilion":	({ HIG"涼亭"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"tree":		({ HIG"大樹"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"grass":	({ HIG"草皮"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"statue":	({ HIM"雕像"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
"fence":	({ HIM"圍牆"NOR		, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 0, 0, 0, 0, 1 }),
"wizhall":	({ HIW"巫師大廳"NOR	, 1, 0, INDIVIDUAL, AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION	, -1, 1, 0, 1, 0, 1 }),
]);

varargs mapping analyze_building_logic(array loc, int no_owner_check);

int has_building(string type)
{
	return arrayp(building_table[type]);
}

mapping query_building_table()
{
	return copy(building_table);
}

mapping query_building(string building)
{
	return building_table[building];
}	
	
mapping exits_dir(array loc)
{
	string city = loc[CITY];
	int num = loc[NUM];
	int x = loc[X];
	int y = loc[Y];

	return ([ 
		arrange_city_location(x-1, y, city, num) : "west",
		arrange_city_location(x+1, y, city, num) : "east",
		arrange_city_location(x, y-1, city, num) : "north",
		arrange_city_location(x, y+1, city, num) : "south",
	]);
}


// 物質化城市建築
void materialize_city_building(object me, string owner, mapping table, string city, int num, string btype)
{
	array loc, temploc;
	string file, sloc;
	mapping exits;
	object room;

	// 處理牆壁座標資料
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);
		
		switch(MAP_D->query_coor_data(loc, TYPE))
		{
			case WALL:
				MAP_D->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("牆壁資料程式錯誤，請通知巫師處理。\n");
		}
	}
	
	// 處理房間座標資料
	foreach( sloc in table["roomtable"] )
	{
		loc = restore_variable(sloc);
		
		// 設定此座標為 ROOM 標記
		MAP_D->set_coor_data(loc, ROOM, btype);
		
		// 設定此座標之地圖圖示為"  "
		MAP_D->set_coor_icon(loc, "  ");
		
		file = CITY_ROOM_MODULE(city, num, loc[X], loc[Y], btype);
		
		// 摧毀原本在此地的房間物件
		if( find_object(file) ) destruct(file);
		// 刪除原本在此地的房間資料
		if( file_exist(file+".o") ) rm(file+".o");
			
		// 載入虛擬房間物件
		room = load_object(file);
		
		exits = allocate_mapping(1);
		
		// 建立路口連接
		foreach( temploc, string dir in exits_dir(loc) )
		{	
			if( temploc[X] < 0 || temploc[X] > 99 || temploc[Y] < 0 || temploc[Y] > 99 ) continue;
			
			if( member_array( save_variable(temploc), table["roomtable"] ) != -1 ) 
				exits[dir] = CITY_ROOM_MODULE(city, num, temploc[X], temploc[Y], btype);
			else if( MAP_D->query_coor_data(temploc, TYPE) == DOOR ) 
				exits[dir] = copy(temploc);
		}
		
		set("exits", exits, room);
		set("owner", owner, room);
		room->save();
	}

	if( find_object(MAP_D->query_map_system(loc)) == find_object(CITY_D) )
		ESTATE_D->set_estate(owner, table, btype, city, num);

	(MAP_D->query_map_system(loc))->translate_map_to_html(city, num);
	

}

// 物質化區域建築, 用來建立郊區區域的獨立建築
void materialize_area_building(object me, string owner, mapping table, string area, int num, string btype)
{
	array loc, temploc;
	string file, sloc;
	mapping exits;
	object room;

	// 處理牆壁座標資料
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);
		
		switch(MAP_D->query_coor_data(loc, TYPE))
		{
			case WALL:
				MAP_D->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("牆壁資料程式錯誤，請通知巫師處理。\n");
		}
	}
	
	// 處理房間座標資料
	foreach( sloc in table["roomtable"] )
	{
		loc = restore_variable(sloc);
		
		// 設定此座標為 ROOM 標記
		MAP_D->set_coor_data(loc, ROOM, btype);
		
		// 設定此座標之地圖圖示為"  "
		MAP_D->set_coor_icon(loc, "  ");
		
		file = AREA_ROOM_MODULE(area, num, loc[X], loc[Y], btype);
		
		// 摧毀原本在此地的房間物件
		if( find_object(file) ) destruct(file);
		// 刪除原本在此地的房間資料
		if( file_exist(file+".o") ) rm(file+".o");
			
		// 載入虛擬房間物件
		room = load_object(file);

		exits = allocate_mapping(1);
		
		// 建立路口連接
		foreach( temploc, string dir in exits_dir(loc) )
		{	
			if( temploc[X] < 0 || temploc[X] > 99 || temploc[Y] < 0 || temploc[Y] > 99 ) continue;
			
			if( member_array( save_variable(temploc), table["roomtable"] ) != -1 ) 
				exits[dir] = AREA_ROOM_MODULE(area, num, temploc[X], temploc[Y], btype);
			else if( MAP_D->query_coor_data(temploc, TYPE) == DOOR ) 
				exits[dir] = copy(temploc);
		}
		
		set("exits", exits, room);
		set("owner", owner, room);
		room->save();
	}

	ESTATE_D->set_estate(owner, table, btype, area, num);

	(MAP_D->query_map_system(loc))->translate_map_to_html(area, num);
}

// 物質化戶外建築(無室內的建築類型)
void materialize_outdoor_building(object me, string owner, array loc, string building_icon, string building_name, int type)
{
	mapping table = allocate_mapping(0);
	
	CITY_D->set_coor_data(loc, TYPE, type);
	CITY_D->set_coor_icon(loc, building_icon);
	
	// 刪除 material 資訊
	CITY_D->delete_coor_data(loc, "material");
	
	table["walltable"] = ({ save_variable(loc) });
	table["roomtable"] = allocate(0);
	
	if( type != WALL && type != DOOR )
	{
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"在"+loc_short(loc)+NOR"建造了「"+building_name+" "+building_icon+"」", me);
		
		foreach(string building_english_name, array building_info in default_building_table)
		{
			if( remove_ansi(building_info[BUILDING_SHORT]) == building_name )
			{
				ESTATE_D->set_estate(owner, table, building_english_name, loc[CITY], loc[NUM]);
				break;
			}
		}
	}
	
	switch(type)
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			GROWTH_D->add_growth(loc);
			break;
	}
}


// 物質化圍牆建築
/*
void materialize_fence(object me, string owner, mapping table, string city, int num, string btype)
{
	array loc;
	string sloc;

	// 處理牆壁座標資料
	foreach( sloc in table["walltable"] )
	{
		loc = restore_variable(sloc);

		switch(CITY_D->query_coor_data(loc, TYPE))
		{
			case WALL:
				CITY_D->set_coor_data(loc, FLAGS, NO_MOVE);
				break;
			case DOOR:
				break;
				
			default:
				error("牆壁資料程式錯誤，請通知巫師處理。\n");
		}
	}

	table["roomtable"] = allocate(0);

	ESTATE_D->set_estate(owner, table, btype, city, num);
}
*/


// 處理建築物邏輯部份
// --------------------------------------------------------------------------------------------------
//  1
//2 + 4
//  8
#define N	(1<<0)	//NORTH
#define W	(1<<1)	//WEST
#define E	(1<<2)	//EAST
#define S	(1<<3)	//SOUTH

#define T 	8

#define MAX	0
#define MIN	1
#define MAX_BUILDING_RANGE      500
#define MAX_ROOM_TABLE		25
mapping picdir =
([
"∥":N|S,
"��":N|S,
"│":N|S,

"╯":N|W,
"��":N|W,
"��":N|W,
"┘":N|W,
"��":N|W,

"╮":W|S,
"��":W|S,
"��":W|S,
"��":W|S,
"┐":W|S,

"╰":N|E,
"��":N|E,
"��":N|E,
"└":N|E,
"��":N|E,

"╭":S|E,
"��":S|E,
"��":S|E,
"┌":S|E,
"��":S|E,

"＝":W|E,
"��":W|E,
"─":W|E,
]);

string coorname(int x, int y)
{
	return "("+(x+1)+","+(y+1)+")";
}

// 檢查建築物圖形邏輯
varargs mapping analyze_building_logic(array loc, int no_owner_check)
{
	int i, x, y, lx, ly, *ry, entry, exit, lentry, lexit, dir, ndir, evenwall, evenwallbit, num;

	object me = this_player();
	array map = MAP_D->query_map(loc);
	
	string owner, tempowner, city;
	string *inverse_roomtable = allocate(0);
	
	mapping boundary = allocate_mapping(0);
	mapping table = allocate_mapping(0);

	table["walltable"] = allocate(0);
	table["roomtable"] = allocate(0);

	city = loc[CITY];
	num = loc[NUM];
	x = loc[X];
	y = loc[Y];
	
	// 檢查牆壁連續與過度轉折之狀況
	while(++i)
	{

		if( i>MAX_BUILDING_RANGE )
		{
			tell(me, "以現在的技術無法建造這麼大的建築物。\n");
			return 0;
		}
	
		// 並非牆壁圖像
		if( undefinedp(map[y]) || undefinedp(map[y][x]) || undefinedp(dir = picdir[remove_ansi(map[y][x])]) )
		{
			tell(me, "牆壁必須要完整不能有破碎斷裂的地方喔！\n");
			return 0;
		}

		// 用於第一個牆壁上, 隨機找出一個目前的圖像出口
		if( undefinedp(lexit) ) 
		{
			for(lexit=1;lexit &~ dir;lexit*=2); 
			lexit = T / lexit;
		}
		
		// 此牆壁之入口方向沒有接續上個牆壁之出口方向
		if( T / lexit &~ dir )
		{
			tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(lx,ly)+"的兩座牆壁並不連續！\n");
			return 0;
		}

		entry = T / lexit;
		exit = dir ^ entry;
		
		if( !undefinedp(lentry) && lentry == exit )
		{
			tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(lx,ly)+"的兩座牆壁轉折過度！\n");
			return 0;
		}
		
		// 牆壁連續性檢查完畢
		if( i!=1 && x == loc[X] && y == loc[Y] ) break;
		
		// 建立建築物範圍資訊
		if( undefinedp(boundary[y]) ) boundary[y] = allocate(2, x);
		if( x < boundary[y][MIN] ) boundary[y][MIN] = x;
		if( x > boundary[y][MAX] ) boundary[y][MAX] = x;
		
		table["walltable"] += ({ save_variable( arrange_city_location(x, y, city, num) ) });
		
		lx = x;
		ly = y;
		lexit = exit;
		lentry = entry;
			
		switch(exit)
		{
			case N: y--;break;
			case S: y++;break;
			case W: x--;break;
			case E:	x++;break;
		}
		
	}
	
	ry = sort_array(keys(boundary),1);
	
	// 從整個建築物範圍掃描檢查細節之牆壁重疊狀況與內部狀態
	for(y=ry[0];y<=ry[<1];evenwall=evenwallbit=0, y++)
	for(x=boundary[y][MIN];x<=boundary[y][MAX];x++)
	{
		owner = MAP_D->query_coor_data(arrange_location(x,y,loc[Z],loc[PLACE],loc[NUM],loc[EXTRA]), "owner");
		
		if( !tempowner )
			tempowner = owner;
		
		if( !no_owner_check && owner != tempowner )
		{
			tell(me, "建築物中的座標"+coorname(x,y)+"的土地擁有人與此地不同。\n");
			return 0;
		}
		
		if( picdir[remove_ansi(map[y][x])] & (N|S) )
		{
			evenwall++;
			evenwallbit |= picdir[remove_ansi(map[y][x])];
		}

		// 發現有牆壁圖像, 開始作重疊檢查
		if( !undefinedp(dir = picdir[remove_ansi(map[y][x])]) )
		{
			// 若此牆壁並非之前所檢查之連續範圍. 則利用遞迴再檢查內部之建築邏輯
			// 由此可允許製作多層次建築物
			if( member_array(save_variable( arrange_city_location(x, y, city, num) ), table["walltable"] ) == -1 )
			{
				mapping inside_building_table;
				array inside_loc = arrange_location(x, y, loc[Z], loc[PLACE], loc[NUM], loc[EXTRA]);
				
				inside_building_table = analyze_building_logic(inside_loc);
				
				if( !arrayp(inside_building_table) )
				{
					tell(me, "多層次建築之內部建築物邏輯錯誤。\n");
					return 0;
				}
				
				table["walltable"] += inside_building_table["walltable"];
				// 反相之 roomtable
				inverse_roomtable += inside_building_table["roomtable"];
			}

			// Y 向牆壁重疊檢查
			if( y < ry[<1] )
			{
				ndir = picdir[remove_ansi(map[y+1][x])];
				if( ndir && S &~ dir && N &~ ndir )
				{
					tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(x, y+1)+"兩個牆壁不能這樣疊在一起。\n");
					return 0;
				}
			}
			
			// X 向牆壁重疊檢查
			if( x < boundary[y][MAX] )
			{
				ndir = picdir[remove_ansi(map[y][x+1])];
				if( ndir && E &~ dir && W &~ ndir )
				{
					tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(x+1, y)+"兩個牆壁不能這樣疊在一起。\n");
					return 0;
				}
			}

			// 斜向牆壁重疊檢查, 避免斜向封閉空間
			if( y < ry[<1] && x < boundary[y][MAX] && (dir == (N|W)) && (picdir[remove_ansi(map[y+1][x+1])] == (S|E)) )
			{
				tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(x+1, y+1)+"兩個牆壁不能這樣疊在一起。\n");
				return 0;
			}
			
			// 斜向牆壁重疊檢查, 避免斜向封閉空間
			if( y < ry[<1] && x > boundary[y][MIN] && (dir == (N|E)) && (picdir[remove_ansi(map[y+1][x-1])] == (S|W)) )
			{
				tell(me, "建築物中的座標"+coorname(x,y)+"與座標"+coorname(x-1, y+1)+"兩個牆壁不能這樣疊在一起。\n");
				return 0;
			}
		}
		else if( evenwall % 2 || evenwallbit & (N|S) == (N|S)) 
			table["roomtable"] += ({ save_variable( arrange_city_location(x, y, city, num) ) });
	}
	
	table["roomtable"] -= inverse_roomtable;

	if( sizeof(table["roomtable"]) > MAX_ROOM_TABLE )
	{
		tell(me, "目前建築物之房間佔地數量不得超過 "+MAX_ROOM_TABLE+"。\n");
		return 0;
	}
	return table;
}


varargs mixed query_floor_total_value(int floor)
{
	if( undefinedp(floor) )
		return floor_total_value;
	
	return floor_total_value[floor];
}
	
varargs mixed query_floor_value(int floor)
{
	if( undefinedp(floor) )
		return floor_value;
	
	return floor_value[floor];
}

// --------------------------------------------------------------------------------------------------


void create()
{
	int i;
	object module;
	building_table = allocate_mapping(0);
	floor_total_value = allocate_mapping(0);
	floor_value = allocate_mapping(0);

	// 初始化所有模組資訊
	foreach( string file in get_dir(ROOM_MODULE_PATH) )
	{
		if( file[<2..<1] == ".c" && !catch(module = load_object(ROOM_MODULE_PATH+file)) )
			building_table[file[0..<3]] = module->query_building_info();
	}
	
	building_table += copy(default_building_table);

	for(i=1;i<=5;i++)
		floor_value[i] = 100000 * i;
	for(i=6;i<=100;i++)
		floor_value[i] = 100000 * i * (i-4);
	for(i=101;i<=120;i++)
		floor_value[i] = 300000 * i * (i-4);
	for(i=121;i<=140;i++)
		floor_value[i] = 500000 * i * (i-4);
	for(i=141;i<=160;i++)
		floor_value[i] = 700000 * i * (i-4);
	for(i=161;i<=180;i++)
		floor_value[i] = 900000 * i * (i-4);
	for(i=181;i<=200;i++)
		floor_value[i] = 1100000 * i * (i-4);
	for(i=201;i<=1000;i++)
		floor_value[i] = 5000000 * i * (i-4);
		
	for(i=1;i<=1000;i++)
		floor_total_value[i] = floor_total_value[i-1] + floor_value[i];
}
	
string query_name()
{
	return "建築系統(BUILDING_D)";
}

int is_loc_closed(array loc)
{
	mixed type = MAP_D->query_coor_data(loc, TYPE);
	
	if( type == DOOR || type == WALL || MAP_D->query_coor_data(loc, ROOM) )
		return 1;
		
	return 0;
}

// 檢查建築物是否完全被封鎖
varargs int is_building_locked(array loc, mapping estdata)
{
	int x, y, num;
	string city;
	int locked;
	string sloc;
	
	if( !arrayp(loc) ) return 0;
	
	if( undefinedp(estdata) )
		estdata = ESTATE_D->query_loc_estate(loc);
	
	if( !mapp(estdata) ) return 0;
		
	locked = 1;
	
	foreach(sloc in estdata["walltable"])
	{
		loc = restore_variable(sloc);	
		
		if( CITY_D->query_coor_data(loc, TYPE) == WALL || CITY_D->query_coor_data(loc, "lock") )
			continue;

		x = loc[X];
		y = loc[Y];
		city = loc[CITY];
		num = loc[NUM];
		
		if( x+1 < MAP_WIDTH && !is_loc_closed(arrange_city_location(x+1, y, city, num)) )
		{
			locked = 0;
			break;
		}
		
		if( x-1 >= 0 && !is_loc_closed(arrange_city_location(x-1, y, city, num)) )
		{
			locked = 0;
			break;
		}
			
		if( y+1 < MAP_LENGTH && !is_loc_closed(arrange_city_location(x, y+1, city, num)) )
		{
			locked = 0;
			break;
		}
		
		if( y-1 >= 0 && !is_loc_closed(arrange_city_location(x, y-1, city, num)) )
		{
			locked = 0;
			break;
		}
	}
	
	return locked;
}

// 檢查某個房間對應之建築物是否完全被封鎖
int is_building_room_locked(object room)
{
	return is_building_locked(room->query_location());
}
