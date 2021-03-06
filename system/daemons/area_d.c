/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : area_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-18
 * Note   : 區域精靈, 處理所有區域資料庫
 * Update :
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <map.h>
#include <areadata.h>
#include <lock.h>
#include <location.h>
#include <daemon.h>
#include <object.h>
#include <terrain.h>
#include <gender.h>
#include <message.h>
#include <mudlib.h>
#include <nature.h>
#include <nature_data.h>
#include <roommodule.h>
#include <time.h>
#include <envvar.h>
#include <equipment.h>

#define LOG			"daemon/area"

#define SAVE_INFO		(1<<0)
#define SAVE_DATA		(1<<1)
#define SAVE_MAP		(1<<2)
#define SAVE_ESTATE		(1<<3)
#define SAVE_ALL		(SAVE_INFO|SAVE_DATA|SAVE_MAP|SAVE_ESTATE)

#define RANDOM_STRING		({ "花椰菜", "保齡球", "充氣娃娃", "棒棒糖", "榴槤", "按摩棒", "胖女人", "土地公", "高壓電箱", "碎玻璃", "疑似爆裂物", "黑道老大", "羊肉爐", "發情的動物", "核彈發射器", "捕鳥器" })

private int number;
private int save_all_time;

private mapping areas;
private mapping sort_save_list;

varargs int area_exist(string area, int num)
{
	if( undefinedp(num) )
		return mapp(areas[area]);
	else
		return mapp(areas[area]) && mapp(areas[area][num]);
}

varargs int map_exists(string map, int section)
{
	if( undefinedp(section) )
		return mapp(areas[map]);
	else
		return mapp(areas[map]) && mapp(areas[map][section]);
}

int is_area_location(mixed map)
{
	if( arrayp(map) )
		return area_exist(map[AREA], map[NUM]);
	else if( stringp(map) )
		return area_exist(map);

	return 0;
}

varargs string query_area_id(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["color_id"];
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["color_id"] + " "+(num+1);
	}
}

varargs string query_area_name(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["name"];
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["name"] + (areas[area][INFO]["section"][num]["name"] || "第"+CHINESE_D->chinese_number(num+1)+"區");
	}
}

varargs string query_area_idname(string area, int num)
{
	if( undefinedp(num) )
	{
		if( !area_exist(area) ) return 0;
		return areas[area][INFO]["name"]+"<"+areas[area][INFO]["color_id"]+">";
	}
	else
	{
		if( !area_exist(area, num) ) return 0;
		return areas[area][INFO]["name"]+(areas[area][INFO]["section"][num]["name"] || "第"+CHINESE_D->chinese_number(num+1)+"區")+"<"+areas[area][INFO]["color_id"]+" "+(num+1)+">";
	}
}

int valid_coordinate(mixed x, int y, string area, int num)
{
	if( intp(x) )
		return x>=0 && x<MAP_WIDTH && y>=0 && y<MAP_LENGTH && area_exist(area, num);
	else if( arrayp(x) )
		return x[X]>=0 && x[X]<MAP_WIDTH && x[Y]>=0 && x[Y]<MAP_LENGTH && area_exist(x[AREA], x[NUM]);
	else
		return 0;
}

/* 對所有區域編號 */
void assign_areas_num()
{
	number = 0;

	sort_save_list = allocate_mapping(0);

	foreach( string area in get_dir("/area/") )
	{
		if( file_size("/area/"+area) != -2 ) continue;

		sort_save_list[number++] = area;
	}
}

void save_options(string area, int num, int bit)
{
	// 轉換地圖至 HTML
	AREA_D->translate_map_to_html(area, num);

	// 寫入基本資訊檔
	if( bit & SAVE_INFO )
	{

		if( !area_exist(area) || undefinedp(areas[area][INFO]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+area+" 基本資訊");
			log_file(LOG, "喪失 "+area+" 基本資訊");
		}
		else if( !write_file(AREA_INFO(area), save_variable( copy(areas[area][INFO]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+area+" 基本資訊檔");
			log_file(LOG, "無法寫入 "+area+" 基本資訊檔");
		}
	}

	// 寫入座標資料檔
	if( bit & SAVE_DATA )
	{
		if( !area_exist(area) || undefinedp(areas[area][num][DATA]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+area+" 座標資料");
			log_file(LOG, "喪失 "+area+" 座標資料");
		}

		else if( !write_file(AREA_NUM_DATA(area, num), save_variable( copy(areas[area][num][DATA]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+area+" "+num+" 座標資料檔");
			log_file(LOG, "無法寫入 "+area+" "+num+" 座標資料檔");
		}
	}

	// 寫入地圖資料檔
	if( bit & SAVE_MAP )
	{
		if( !area_exist(area) || undefinedp(areas[area][num][MAP]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+area+" 地圖資料");
			log_file(LOG, "喪失 "+area+" 地圖資料");
		}

		else if( !write_file(AREA_NUM_MAP(area, num), save_variable( copy(areas[area][num][MAP]) ), 1) )
		{	
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+area+" "+num+" 地圖資料檔");
			log_file(LOG, "無法寫入 "+area+" "+num+" 地圖資料檔");
		}
	}
	
	// 寫入房地產資料檔
	if( bit & SAVE_ESTATE )
	{
		if( !write_file(AREA_NUM_ESTATE(area, num), save_variable(ESTATE_D->query_estate(area, num)), 1) )
		{	
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+area+" "+num+" 房地產資料檔");
			log_file(LOG, "無法寫入 "+area+" "+num+" 房地產資料檔");
		}
	}
}

varargs void save_area(string area, int num, int bit)
{
	int time_exp;

	if( !undefinedp(num) )
	{
		string options = "";

		if( !area_exist(area, num) ) return;

		time_exp = time_expression { save_options(area, num, undefinedp(bit) ? SAVE_ALL : bit); };

		if( bit & SAVE_INFO ) options += "基本資訊 ";
		if( bit & SAVE_DATA ) options += "座標資料 ";
		if( bit & SAVE_MAP  ) options += "地圖資料 ";
		if( bit & SAVE_ESTATE) options += "房地產資料 ";

		if( options != "" )
			CHANNEL_D->channel_broadcast("nch",sprintf("區域：儲存區域「%s」%s(%.6f sec)。",query_area_idname(area, num), options, time_exp/1000000.));
	}
	else if( area_exist(area) )
	{
		foreach( num, mapping data in areas[area] )
		if( intp(num) )
			time_exp += time_expression { save_options(area, num, SAVE_ALL); };

		CHANNEL_D->channel_broadcast("nch",sprintf("區域：儲存區域「%s」所有資料(%.6f sec)。",query_area_idname(area), time_exp/1000000.));
	}
}

/* 由 TIME_D 定期呼叫, 區域資料輪流儲存 */
void time_distributed_save()
{
	string area;
	int num;

	if( sizeof(areas) < 1 ) return;

	area = sort_save_list[++number] || sort_save_list[(number=0)];
	
	for(num=0;area_exist(area, num);num++)
		call_out( (: save_area :), (num+1)*10, area, num);
}

/* 儲存所有 area 資訊 */
int save_all()
{
	int time_exp;

	// 為避免在 shutdown 時, 造成重覆 save_all 的情形, 故設置 save_all_time 作檢查
	if( time() - save_all_time < 30 ) return 0;

	save_all_time = time();

	time_exp = time_expression {

		foreach( string area in get_dir(AREA_ROOT) )
		if( file_size(AREA_PATH(area)) == -2 )
			save_area(area);
	};// end of time_expression

	CHANNEL_D->channel_broadcast("nch",sprintf("區域：分散儲存所有區域資料完畢(共費 %.6f sec)。", time_exp/1000000.));
	return 1;
}

void initialize_area_data(string area)
{
	if( !stringp(area) ) return;

	areas[area] 			= allocate_mapping(0);
	areas[area][INFO] 		= allocate_mapping(0);
	areas[area][INFO]["section"]	= allocate_mapping(0);
}

void initialize_section_data(string area, int num)
{
	if( !stringp(area) || !intp(num) ) return;

	areas[area][num] = allocate_mapping(5);

	areas[area][num][DATA] = allocate_mapping(0);
	areas[area][num][MAP]  = allocate_mapping(0);
	areas[area][num][RMAP] = allocate_mapping(0);
	areas[area][num][AROOM]= load_arearoom(area, num);
}

/* 建立區域新分區, 啟始地圖資料庫與建立區域地圖 */
int create_section(string area, int maptype)
{
	array newmap;
	mapping newdata;

	int x, y, num=0, all_pop;

	// 自動搜尋編號
	while( file_size(AREA_NUM(area, num)) == -2 ) num++;

	mkdir(AREA_NUM(area, num));		// 建立子區域目錄
	mkdir(AREA_NUM(area, num)+"npc");
	mkdir(AREA_NUM(area, num)+"obj");
	mkdir(AREA_NUM_ROOM(area, num));	// 子區域房間目錄

	initialize_section_data(area, num);
	
	areas[area][INFO]["section"][num] = allocate_mapping(0);
	areas[area][INFO]["section"][num]["area_section_influence"] = allocate_mapping(0);

	newdata = allocate_mapping(100);

	// 建立預設座標資料
	for(y=0;y<MAP_LENGTH;y++)
	{
		newdata[y] = allocate_mapping(1);

		for(x=0;x<MAP_WIDTH;x++)
			newdata[y][x] = allocate_mapping(0);
	}

	// 建立區域標準地圖
	newmap = copy(allocate(MAP_LENGTH, copy(allocate(MAP_WIDTH, GRN"．"NOR))));

	// 建立特殊地形
	TERRAIN_D->create_area_map_terrain(ref newmap, ref newdata, maptype);
	
	areas[area][num][MAP] = copy(newmap);
	areas[area][num][RMAP]= copy(newmap);
	areas[area][num][DATA]= copy(newdata);

	// 啟始分區天氣
	NATURE_D->activate_nature(areas[area][num][AROOM]);
	
	// 建立房地產資料
	ESTATE_D->create_estate(area, num);

	log_file(LOG, area+" 區域建立新分區 "+num);

	CHANNEL_D->channel_broadcast("news", HIY"在世界的某處出現了全新的區域『"+NOR+query_area_idname(area, num)+HIY"』。\n"NOR);

	save_area(area, num, SAVE_ALL);

	return all_pop;
}

void rebuild_section_terrain(string area, int num, int maptype)
{
	array newmap;
	mapping newdata;
	int x, y;

	newdata = allocate_mapping(100);

	// 建立預設座標資料
	for(y=0;y<MAP_LENGTH;y++)
	{
		newdata[y] = allocate_mapping(1);

		for(x=0;x<MAP_WIDTH;x++)
			newdata[y][x] = allocate_mapping(0);
	}

	// 建立區域標準地圖
	newmap = copy(allocate(MAP_LENGTH, copy(allocate(MAP_WIDTH, GRN"．"NOR))));

	// 建立特殊地形
	TERRAIN_D->create_area_map_terrain(ref newmap, ref newdata, maptype);
	
	areas[area][num][MAP] = copy(newmap);
	areas[area][num][RMAP]= copy(newmap);
	areas[area][num][DATA]= copy(newdata);
	
	save_area(area, num, SAVE_ALL);
}

/* 建立全新區域, 啟始區域資料庫 */
int create_area(object me, string color_area_id, string color_area_name, int maptype)
{
	int all_pop, len;
	string area_id;

	color_area_id = kill_repeat_ansi(lower_case(color_area_id)+NOR);
	area_id = remove_ansi(color_area_id);

	// 將 color_area_id 第一字母 upper_case
	color_area_id = capitalize(color_area_id);

	// 已經存在 area id
	if( area_exist(area_id) || CITY_D->city_exist(area_id) ) return -1;

	// 非純英文之 area id
	foreach( int a in area_id )
	if( (int) a<'a' || (int) a>'z' ) 
		return -2;

	// area_name 字元數錯誤
	len = noansi_strlen(color_area_name);
	if(  len > 10 || len % 2 )
		return -3;

	// 非純中文之 area name	
	foreach( int a in remove_ansi(color_area_name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return -4;

	color_area_name = kill_repeat_ansi(color_area_name+NOR);

	// 重覆的區域中文名稱
	//foreach( string tmp, mapping data in areas )
	//if( mapp(data[INFO]) && remove_ansi(data[INFO]["name"]) == remove_ansi(color_area_name) ) return -6;

	// 建立需要的目錄
	mkdir(AREA_PATH(area_id));		// 區域主目錄

	// 初始化區域資料
	initialize_area_data(area_id);

	areas[area_id][INFO]["id"] 		= area_id;
	areas[area_id][INFO]["color_id"]	= color_area_id;
	areas[area_id][INFO]["name"] 		= color_area_name;
	areas[area_id][INFO]["maptype"]		= maptype;

	// 建立第一區域分區
	all_pop = create_section(area_id, maptype);

	assign_areas_num();

	log_file(LOG, me->query_id()+"建立全新區域 "+area_id+"。\n");

	return 1;
}

/* 刪除子區域分區 */
void delete_section(string area, int num)
{
	string filename;
	object room, inv;
	
	if( !areas[area][num] ) return;

	// 移除區域內部所有房間內之所有物件
	foreach( filename in get_dir(AREA_NUM_ROOM(area, 0)) )
	{
		room = find_object(AREA_NUM_ROOM(area, 0)+filename[0..<3]);

		if( !objectp(room) ) continue;

		foreach( inv in all_inventory(room) )
		{
			if( userp(inv) )
			{
				inv->move(WIZ_HALL);
				msg("$ME所在的區域消失了，只好流浪至巫師大廳。\n",inv,0,1);
			}
			else
				destruct(inv);
		}
		destruct(room);
	}

	// 移除所有在區域物件中的物件
	foreach( inv in all_inventory(areas[area][num][AROOM]) )
	{
		if( userp(inv) )
		{
			reset_screen(inv);
			inv->move(WIZ_HALL);
			msg("$ME所在的區域滅亡了，只好流浪至巫師大廳。\n",inv,0,1);
		}
		else
			destruct(inv);
	}

	// 終止分區天氣
	NATURE_D->deactivate_nature(areas[area][num][AROOM]);

	// 摧毀子區域物件
	destruct( areas[area][num][AROOM] );

	// 刪除子區域目錄
	rmtree(AREA_NUM(area, num));

	// 刪除子區域所有資訊
	map_delete(areas[area], num);

	// 移除子城市房地產資訊
	ESTATE_D->remove_zone_estate(area, num);

	log_file(LOG, "刪除 "+area+" 區域分區 "+num);
	CHANNEL_D->channel_broadcast("sys", "刪除 "+area+" 區域分區 "+num);
}

/* 刪除整個區域 */
void delete_area(string area)
{
	int num;

	if( !area_exist(area) ) return;

	num = sizeof(areas[area]);

	// 刪除所有分區
	while(num--)
		delete_section(area, num);

	log_file(LOG,"刪除 "+area+" 區域。\n");
	CHANNEL_D->channel_broadcast("news", query_area_idname(area)+"從此自世界地圖上消失。\n");

	// 清除區域資料
	map_delete(areas, area);

	if( !sizeof(areas) )
		areas = allocate_mapping(0);

	// 刪除區域目錄
	rmtree(AREA_PATH(area));

	// 重新編排區域排序
	assign_areas_num();
}

/* 設定某一區域地圖上某一座標之其中一項變數值 */
mixed set_coor_data(array loc, mixed prop, mixed data)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(areas[area][num][DATA][y][x]) )
		return 0;

	if( prop == FLAGS )
	{
		if( !intp(data) ) return 0;
		return (areas[area][num][DATA][y][x][prop] |= data);
	}
	else
		return (areas[area][num][DATA][y][x][prop] = data);
}

/* 對一座標之變數值做數值加減處理 */
mixed add_coor_data(array loc, mixed prop, int data)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(areas[area][num][DATA][y][x]) )
		return 0;

	return (areas[area][num][DATA][y][x][prop] += data);
}

/* 刪除一座標之資料 */
varargs void delete_coor_data(array loc, mixed prop, int bit)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !mapp(areas[area][num][DATA][y][x]) ) return;

	// 若無 prop 則重新初始化房間資訊, 人口維持原狀
	if( !prop ) 
		areas[area][num][DATA][y][x] = allocate_mapping(0);

	else if( prop == FLAGS )
	{
		if( !undefinedp(bit) )
			areas[area][num][DATA][y][x][prop] ^= bit;
		else
			map_delete(areas[area][num][DATA][y][x], prop);
	}
	else
	{
		map_delete(areas[area][num][DATA][y][x], prop);

		// 群組刪除功能
		foreach(mixed ori_prop, mixed data in areas[area][num][DATA][y][x])
		{
			if( stringp(ori_prop) && stringp(prop) && sscanf(ori_prop, prop+"/%*s") == 1 )
				map_delete(areas[area][num][DATA][y][x], ori_prop);
		}
	}
}


/* 傳回某一區域地圖上某一座標之其中一項變數值 */
varargs mixed query_coor_data(array loc, mixed prop)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) )
		return areas[area][num][DATA][y][x];
	else
		return areas[area][num][DATA][y][x][prop];
}

/* 範圍性查尋, 傳回資料陣列 */
/* 圓形半徑資料群搜尋法, 搜尋時間與 radius 的平方呈正比 */
mapping query_coor_range(array loc, string prop, int radius)
{
	string area;
	int num, x, y;
	int i, j, xf, xt, yf, yt;
	mapping coor, data = allocate_mapping(0);

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	coor = areas[area][num][DATA];

	if( radius < 1 ) return ([ save_variable(loc):coor[y][x][prop] ]);

	xf = x-radius; if( xf < 0 ) 	xf = 0;
	xt = x+radius; if( xt > 99 ) 	xt = 99;
	yf = y-radius; if( yf < 0 )	yf = 0;
	yt = y+radius; if( yt > 99 )	yt = 99;
	
	radius *= radius;
	
	for(i=xf;i<=xt;i++)
		for(j=yf;j<=yt;j++)
			if( (i-x)*(i-x) + (j-y)*(j-y) <= radius )
				data[save_variable(arrange_area_location(i,j,area,num))] = coor[j][i][prop];
	return data;
}
/* 設定座標地圖圖形 */
void set_coor_icon(array loc, string icon)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, area, num) )
		areas[area][num][MAP][y][x] = areas[area][num][RMAP][y][x] = icon;
}

/* 設定座標地圖圖形 */
void set_real_time_coor_icon(array loc, string icon)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, area, num) )
		areas[area][num][RMAP][y][x] = icon;
}

/* 傳回座標地圖圖形 */
varargs string query_coor_icon(array loc, int raw)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	return !undefinedp(raw) ? remove_ansi(areas[area][num][MAP][y][x]) : areas[area][num][MAP][y][x];
}

/* 重設座標地圖圖形 */
void reset_coor_icon(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	areas[area][num][RMAP][y][x] = areas[area][num][MAP][y][x];
}

// 刪除座標地圖圖形
void delete_coor_icon(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	areas[area][num][RMAP][y][x] = GRN"．"NOR;
	areas[area][num][MAP][y][x] = GRN"．"NOR;
}

/* 設定區域基本資訊 */
mixed set_area_info(string area, string prop, mixed info)
{
	if( !stringp(area) || !stringp(prop) || undefinedp(info) || !area_exist(area) ) return 0;

	return areas[area][INFO][prop] = info;
}

/* 回傳區域基本資訊 */
varargs mixed query_area_info(string area, string prop)
{
	if( !stringp(area) || !area_exist(area) ) return 0;

	if( !undefinedp(prop) )
		return copy(areas[area][INFO][prop]);
	else
		return copy(areas[area][INFO]);
}

/* 刪除區域基本資訊 */
int delete_area_info(string area, string prop)
{
	if( !stringp(area) || !stringp(prop) || !area_exist(area) ) return 0;

	map_delete(areas[area][INFO], prop);

	return 1;
}
/* 設定區域分區基本資訊 */
mixed set_section_info(string area, int num, string prop, mixed info)
{
	if( !stringp(area) || !stringp(prop) || undefinedp(info) || !area_exist(area, num) ) return 0;

	return areas[area][INFO]["section"][num][prop] = info;
}

/* 回傳區域分區基本資訊 */
varargs mixed query_section_info(string area, int num, string prop)
{
	if( !stringp(area) || !area_exist(area, num) ) return 0;

	if( !undefinedp(prop) )
		return copy(areas[area][INFO]["section"][num][prop]);
	else
		return copy(areas[area][INFO]["section"][num]);
}

/* 刪除區域分區基本資訊 */
int delete_section_info(string area, int num, string prop)
{
	if( !stringp(area) || !stringp(prop) || !area_exist(area, num) ) return 0;

	map_delete(areas[area][INFO]["section"][num], prop);

	return 1;
}

// 修改區域總名稱
void change_area_name(string area, string name)
{
	int len;

	if( !area_exist(area) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// 非純中文之 area name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(trim(name)+NOR);

	CHANNEL_D->channel_broadcast("news", query_area_idname(area)+"正式更名為「"+name+"」");

	set_area_info(area, "name", name);
}

// 修改子區域名稱 
void change_section_name(string area, int num, string name)
{
	int len;
	string original_idname;

	if( !area_exist(area, num) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// 非純中文之 area name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(trim(name)+NOR);

	original_idname = query_area_idname(area, num);

	set_section_info(area, num, "name", name);

	CHANNEL_D->channel_broadcast("news", original_idname+"正式更名為「"+query_area_idname(area, num)+"」");
}		



/* 傳回所有區域名稱 */
string *query_areas()
{
	return filter_array(keys(areas), (: stringp($1) :));
}

int query_sizeof_areas()
{
	return sizeof(query_areas());
}


object query_maproom(array loc)
{
	string area = loc[AREA];
	int num = loc[NUM];

	if( !area_exist(area, num) ) return 0;

	return areas[area][num][AROOM];
}

object *query_all_maproom()
{
	object *all_maproom = allocate(0);

	foreach( string area, mapping m1 in areas )
	foreach( int num, mapping m2 in m1 )
	{
		if( intp(num) )
			all_maproom += ({ m2[AROOM] });
	}
	return all_maproom;
}

varargs array query_map(array loc, int realtime)
{
	string area = loc[AREA];
	int num = loc[NUM];

	if( !area_exist(area, num) ) return 0;

	return realtime ? areas[area][num][RMAP] : areas[area][num][MAP];
}

/* 傳回在某區域中的所有玩家 */
varargs object *query_area_players(string area, int num)
{
	if( undefinedp(num) )
	{
		if( area_exist(area) )
			return filter_array(users(), (: query_temp("in_area/id",$1) == $(area) :));

		return 0;
	}

	if( area_exist(area, num) )
		return filter_array(users(), (: query_temp("in_area/id",$1) == $(area) && query_temp("in_area/num",$1) == $(num) :));

	return 0;
}

/* 傳回在某區域中的所有戶外玩家 */
varargs object *query_outdoor_players(string area, int num)
{
	if( undefinedp(num) )
	{
		if( area_exist(area) )
		{
			object *outdoor_players = allocate(0);

			foreach( num in keys(areas[area]) )
			if( intp(num) )
				outdoor_players += filter_array(all_inventory(areas[area][num][AROOM]), (: userp($1) :));

			return outdoor_players;
		}
		return 0;
	}

	if( area_exist(area, num) )
		return filter_array(all_inventory(areas[area][num][AROOM]), (: userp($1) :));

	return 0;
}

int query_sections(string area)
{
	return sizeof(filter_array(keys(areas[area]), (: intp($1) :)));
}

mapping query_action(array loc)
{
	string area;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	area = loc[AREA];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !area_exist(area, num) || !valid_coordinate(x, y, area, num) || undefinedp(areas[area][num][DATA]) ) return 0;

	switch( areas[area][num][DATA][y][x][TYPE] )
	{
		case FARM:	return 0;
		case PAVILION:
		case TREE:
		case GRASS:
			return ([
			    "sit":		([ 0:COMMAND_D->find_sub_command("area", "sit") ]),
			]);
		default: return 0;
	}
}

string position(int x, int y)
{
	switch(x)
	{
		case 0..32:
			switch(y)
			{
			case 0..32:	return "西北邊";
			case 33..65:	return "西邊";
			case 66..99:	return "西南邊";
			default:break;
			}
			break;
		case 33..65:
			switch(y)
			{
			case 0..32:	return "北邊";
			case 33..65:	return "中間";
			case 66..99:	return "南邊";
			default:break;
			}
			break;
		case 66..99:
			switch(y)
			{
			case 0..32:	return "東北邊";
			case 33..65:	return "東邊";
			case 66..99:	return "東南邊";
			default:break;
			}
			break;
		default:break;
	}

	return 0;
}

//   1  2   4
//   8      16
//   32 64  128
int direction_check(int x, int y, string area, int num)
{
	int flag;
	mapping coor, data = areas[area][num][DATA];

	coor = data[y];

	if( x 			&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 8;
	if( x!=99 		&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 16;

	coor = data[y-1];

	if( x && y 		&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 1;
	if( y && x!=99  	&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 4;
	if( y 			&& !(coor[x][FLAGS] & NO_MOVE)) 	flag |= 2;

	coor = data[y+1];
	if( x && y!=99 		&& !(coor[x-1][FLAGS] & NO_MOVE)) 	flag |= 32;
	if( y!=99 		&& !(coor[x][FLAGS] & NO_MOVE)) 	flag |= 64;
	if( y!=99 && x!=99 	&& !(coor[x+1][FLAGS] & NO_MOVE)) 	flag |= 128;

	return flag;
}

// 回傳區域座標說明
string query_coor_help(array loc, string prop, int radius)
{
	string area = loc[AREA];
	int num = loc[NUM], y = loc[Y], x = loc[X];

	switch( areas[area][num][DATA][y][x][TYPE] )
	{
	case FARM:
		return @FARM_HELP
    在這塊田地上可以插秧(播種)及灌溉與收割，指令如下
    
    sow 秧苗或種子名 	身上若有秧苗或是種子，就可以用此指令來插秧或播種
    irrigate water  	身上若是有水的話，就可以用此指令來灌溉
    reap		種成之後便可以執行此指令來收割
    
    做以上這些工作都是要花費體力的，並且要去找尋秧苗或種子與水源，秧苗品
質的好壞與水質都對收割結果有很重要的影響，另外開始灌溉後就必須要持續灌溉
至長成為止，否則田地上的農作物將會枯萎荒廢。

FARM_HELP;

	case ROAD:
		return @ROAD_HELP
    這是一條經由市政府規劃好的道路，可供行人與車輛行走，若是開車的話最好
盡量走在平坦的馬路上，若是任意開到未開發的土地上的話，很可能會把車的底盤
撞壞喔！

ROAD_HELP;
	default: return 0;
	}
}

string coor_short_name(array loc)
{
	string shortname = AREA_D->query_coor_data(loc, "short");

	if( shortname )
		return shortname;

	switch( AREA_D->query_coor_data(loc, TYPE) )
	{
	case 0:
		return NOR YEL"荒野"NOR;
		break;
	case ROAD:
		return HIW"道路";
		break;
	case BRIDGE:
		return HIR"橋樑";
		break;
	case FARM:
		return HIG"農田";
		break;
	case PASTURE:
		return NOR YEL"牧場";
		break;
	case DOOR:
		return HIY "門口";
		break;
	case WALL:
		return NOR WHT"牆壁";
		break;
	case SEA:
		return HIB "海洋";
		break;
	case RIVER:
		{
			switch( AREA_D->query_coor_data(loc, FLAGS) )
			{
				case FLOW_NORTH:
					return HBBLU + "流向往北的河流";
					break;
				case FLOW_SOUTH:
					return HBBLU + "流向往南的河流";
					break;
				case FLOW_EAST:
					return HBBLU + "流向往東的河流";
					break;
				case FLOW_WEST:
					return HBBLU + "流向往西的河流";
					break;
				default:
					return HBBLU + "河  流";
					break;
			}
			break;
		}

	case FOREST:
		return NOR HBGRN"樹林";
		break;
	case MOUNTAIN:
		return NOR HBYEL"山脈";
		break;

	case LIGHT:
		return NOR HBYEL"路燈";
	case POOL:
		return NOR HBCYN"水池";
	case PAVILION:
		return NOR HBGRN"涼亭";
	case TREE:
		return NOR HBGRN"大樹";
	case GRASS:
		return NOR HBGRN"草地";
	case STATUE:
		return NOR HBWHT"雕像";

	default:
		return repeat_string(" ", 16);
		break;
	}
}

// x_start , y_start 為可視範圍中 x,y 座標的起始值
// x_center 跟 y_center 是用來計算從哪開始人的位置不在中間可自動伸縮
// x 是橫座標， y 是垂直座標，area 是區域名，num 則是第幾個子市，不傳則是主市[0]
// Code 需要再整理

/*
          │          
    0     │    3     
─────┼─────
    1     │    2     
          │          
*/
string show_map(array loc)
{
	object me = this_player();
	object* equipments;

	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start, 
	x_center, y_center;

	string area, retmap;
	array map2, map3;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	area	=loc[AREA];
	num	=loc[NUM];

	x_center = (AREA_VISION_WIDTH+1)/2;
	y_center = (AREA_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, area, num) ) return "錯誤的地圖區塊！";

	// 給 wiz 方便用
	if( wizardp(me) && query("env/no_weather", me) )
		range = AREA_VISION_WIDTH/3;

	// 裝備手電筒
	else if(sizeof(equipments = me->query_equipping_object(EQ_HAND[PART_ID])) && sizeof(filter_array(equipments, (: query("flashlight", $1) :))))
		range = AREA_VISION_WIDTH/3;

	else
	{
		range = AREA_VISION_WIDTH/3;
		switch((TIME_D->query_gametime_array())[HOUR])
		{
		case 0..4	: range -= 2;break;
		case 20..23	: range -= 2;break;
		}
	
		switch( NATURE_D->query_nature(MAP_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID] )
		{
			case SUNNY:		break;
			case CLOUDY:		break;
			case RAINY:		range -= 1; break;
			case SNOWY:		range -= 3; break;
			case SHOWER:		range -= 2; break;
			case TYPHOON:		range -= 4; break;
			case FOEHN:		break;
		}
	}

	range*=range;

	// 圖上面的橫線
	retmap = NOR WHT"\e[s\e[1;1H\e[2K┌"+sprintf(HIG" %|3d "NOR+WHT"－"+HIG" %|3d "NOR+WHT+"─",x+1,y+1)+NOR WHT"────────────── "HIW+TIME_D->gametime_digital_clock()+NOR WHT" ─ "HIW+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+NOR WHT" ┐";

	dir = direction_check(x, y, area, num);

	// 顯示區塊計算公式
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);

	map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
	map3 = allocate(AREA_VISION_LENGTH, 0);

	if( range == (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3) )
	{	
		map3 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, 1));
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map3[j] = allocate(AREA_VISION_WIDTH);

		for(j=0;j<AREA_VISION_LENGTH;j++)
		{
			map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];

			for(i=0;i<AREA_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"Γ"NOR )
				{
					for(k=j-4;k<=j+4&&k<AREA_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<AREA_VISION_WIDTH;l++)
						{
							if( k>=0 && l>=0 && (l-i)*(l-i) + (k-j)*(k-j) <= 16 )
								map3[k][l] = 1;
						}
				}
				else if( !map3[j][i] && (i-x+x_start)*(i-x+x_start)*0.5 + (j-y+y_start)*(j-y+y_start) <= range )
					map3[j][i] = 1;
			}
		}		
	}

	for(j=0;j<AREA_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H\e[2K"NOR WHT"│"NOR;

		for(i=0;i<AREA_VISION_WIDTH;i++)
		{
			if( map3[j][i] )
			{
				if( j == y-y_start && i == x-x_start )
					retmap += ansi_part(map2[j][i])+HIC"╳"NOR;
				else if( random(40) )
					retmap += map2[j][i];
				else
					retmap += map2[j][i];
			}

			else
				retmap += WHT"◆"NOR;
		}
		retmap += NOR WHT"│\n"NOR;
	}

	// 圖下的橫線
	retmap += "\e["+(j+2)+";1H\e[2K"NOR WHT"└["NOR+
	sprintf("%|18s", coor_short_name(loc)+NOR)+
	NOR WHT"]───"+(BATTLEFIELD_D->is_battlefield(loc)?sprintf("%4d:%-3d─", BATTLEFIELD_D->query_battle_time()/60, BATTLEFIELD_D->query_battle_time()%60):"─────")+
	sprintf(NOR"%|14s"NOR WHT"─"NOR"%|10s"NOR WHT, AREA_D->query_area_name(area)+" "HIW+(num+1)+NOR, AREA_D->query_section_info(area, num, "name")||("第"+CHINESE_D->chinese_number(num+1)+"區"))+
	"┘\e["+(j+3)+";1H\e[2K"NOR GRN"_________________________________________________________郊區地圖_\e[u"NOR;
	
	return retmap+NOR;
}

int query_area_nums(string area)
{
	if( undefinedp(areas[area]) )
		return 0;

        return sizeof(filter_array(keys(areas[area]), (: intp($1) :)));
}


string show_map2(array loc)
{
	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start, 
	x_center, y_center;

	string area, retmap;
	array map2, map3,link;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	area	=loc[AREA];
	num	=loc[NUM];

	x_center = (AREA_VISION_WIDTH+1)/2;
	y_center = (AREA_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, area, num) ) return "錯誤的地圖區塊！";

	

	range = AREA_VISION_WIDTH/3;

	switch((TIME_D->query_gametime_array())[HOUR])
	{
	case 0..4	: range -= 2;break;
	case 20..23	: range -= 2;break;
	}

	switch( NATURE_D->query_nature(MAP_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID] )
	{
		case SUNNY:		break;
		case CLOUDY:		break;
		case RAINY:		range -= 1; break;
		case SNOWY:		range -= 3; break;
		case SHOWER:		range -= 2; break;
		case TYPHOON:		range -= 4; break;
		case FOEHN:		break;
	}

	range*=range;


	// 給 wiz 方便用
	if( wizardp(this_player()) && query("env/no_weather",this_player()) )
		range = (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3);

	// 圖上面的橫線
	retmap = NOR WHT"\e[s\e[1;1H┌"+sprintf(HIG" %|3d "NOR+WHT"－"+HIG" %|3d "NOR+WHT+"─",x+1,y+1)+NOR WHT"────────────── "HIW+TIME_D->gametime_digital_clock()+NOR WHT" ─ "HIW+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+NOR WHT" ┐";

	dir = direction_check(x, y, area, num);

	
	x_start = x+1-x_center;
	y_start = y+1-y_center;
	
	map2 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, "　"));

	//map2=allocate(AREA_VISION_LENGTH);
	//for(j=0;j<AREA_VISION_LENGTH;j++)
	//	map2[j]=allocate(AREA_VISION_WIDTH, "幹");
	//foreach(array aa in map2)
	//tell(find_player("cookys"),implode(map2[0],"")+"\n");

	if( x_start < 0 )
	{
		int abs_x=abs(x_start);
		//人在右
		if( y_start < 0 )
		{
			int abs_y=abs(y_start);
			// 人在右下角
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				
				if( j >= abs_y )
				{
					// 右下
					map2[j][abs_x+1..<1]=copy(areas[area][num][RMAP][y_start+j][0..AREA_VISION_WIDTH-abs_x]);
					// 左下
					if( link=AREA_D->query_section_info(area, num, "maplink_west") )
					{
						map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][<abs_x..<1]);
					}
				} else {
					// 右上
					if( link=AREA_D->query_section_info(area, num, "maplink_north") )
					{
						map2[j][abs_x+1..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..AREA_VISION_WIDTH-abs_x]);
						// 左上
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_west") )
							map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][<abs_x..<1]);
					}	
				}
			}
		
		} else if( y_start+AREA_VISION_LENGTH-1 > MAP_LENGTH )
		{
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			// 人在右上角
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				if(  j >= AREA_VISION_LENGTH-abs_y)
				{
					// 右下
					if( link=AREA_D->query_section_info(area, num, "maplink_south") )
					{
						map2[j][abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][0..AREA_VISION_WIDTH-abs_x]);
						// 左下
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_west") )
							map2[j][0..abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][<abs_x..<1]);
					}
					
				} else {
					//MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j
					// 左上
					if( link=AREA_D->query_section_info(area, num, "maplink_west") )
						map2[j][0..abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][<abs_x..<1]);
					//右上
					//
					map2[j][abs_x..<1]=copy(areas[area][num][RMAP][MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][0..AREA_VISION_WIDTH-abs_x]);
					
				}
			}
		} else {
			// 人在右半
			if( link=AREA_D->query_section_info(area, num, "maplink_west") )
			{
				for(j=0;j<AREA_VISION_LENGTH;j++)
				{
					map2[j][0..abs_x]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][<abs_x..<1]);
					map2[j][abs_x..<1]=copy(areas[area][num][RMAP][y_start+j][0..AREA_VISION_WIDTH-abs_x]);
				}
			}
		}
	} else if( x_start+AREA_VISION_WIDTH - 1 > MAP_WIDTH )
	{
		int abs_x= x_start + AREA_VISION_WIDTH -MAP_WIDTH;
		//人在左
		if( y_start < 0 )
		{
			// 人在左下角
			int abs_y=abs(y_start);
					
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				//依序填入
				//左下
				if( j >= abs_y )
				{
					map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(areas[area][num][RMAP][y_start+j][<AREA_VISION_WIDTH-abs_x..<1]);
					// 右下
					if( link=AREA_D->query_section_info(area, num, "maplink_east") )
						map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-abs_y][0..abs_x-1]);
				} else {
					//左上
					
					if( link=AREA_D->query_section_info(area, num, "maplink_north") )
					{
						map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][<AREA_VISION_WIDTH-abs_x..<1]);
						if( link=AREA_D->query_section_info(link[0], link[1], "maplink_east") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..abs_x-1]);
					} else if( link=AREA_D->query_section_info(area, num, "maplink_east") )	{
						if( link=AREA_D->query_section_info(link[0],link[1], "maplink_north") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-abs_y+j][0..abs_x-1]);
					}
				}
			}
			
		} else if( y_start+AREA_VISION_LENGTH-1 > MAP_LENGTH )
		{
			// 人在左上角
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			
			for( j=0;j<AREA_VISION_LENGTH;j++)
			{
				if( j >= AREA_VISION_LENGTH-abs_y )
				{
					// 左下
					if( link=AREA_D->query_section_info(area, num, "maplink_south") )
					{
						map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][<AREA_VISION_WIDTH-abs_x..<1]);
						//右下
						if( link=AREA_D->query_section_info(link[0],link[1], "maplink_east") )
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[j-(AREA_VISION_LENGTH-abs_y)][0..abs_x-1]);
					}
				} else {
					// 左上
					map2[j][0..AREA_VISION_WIDTH-abs_x-1]=copy(areas[area][num][RMAP][MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][<AREA_VISION_WIDTH-abs_x..<1]);
					//右上
					if( link=AREA_D->query_section_info(area, num, "maplink_east") )
					{
							map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[MAP_LENGTH-(AREA_VISION_LENGTH-abs_y)+j][0..abs_x-1]);
					}
				}
			}
		} else {
			// 人在左半
			if( link=AREA_D->query_section_info(area, num, "maplink_east") )
			{
				for(j=0;j<AREA_VISION_LENGTH;j++)
				{
					map2[j][0..AREA_VISION_WIDTH-abs_x]=copy(areas[area][num][RMAP][y_start+j][<AREA_VISION_WIDTH-abs_x..<1]);
					map2[j][AREA_VISION_WIDTH-abs_x..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[y_start+j][0..abs_x-1]);
					
				}
			}
		}
	} else {
		// X 置中
		
		if( y_start < 0 )
		{
			// 人在下
			int abs_y=abs(y_start);
			if( link=AREA_D->query_section_info(area, num, "maplink_north") )
			{
				map2[0..abs_y]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[<abs_y..<1]);
				map2[abs_y..<1]=copy(areas[area][num][RMAP][0..AREA_VISION_LENGTH-abs_y]);
			}
			
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		
		} else if( y_start+AREA_VISION_LENGTH+1 > MAP_LENGTH )
		{
			int abs_y=(y_start+AREA_VISION_LENGTH)-MAP_LENGTH;
			// 人在上
			if( link=AREA_D->query_section_info(area, num, "maplink_south") )
			{
				map2[0..<abs_y]=copy(areas[area][num][RMAP][y_start..<1]);
				map2[<abs_y-1..<1]=copy(MAP_D->query_map(arrange_area_location(0,0,link[0],link[1]))[0..abs_y]);
				
			}
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		} else {
			//區域內
			map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
			for(j=0;j<AREA_VISION_LENGTH;j++)
				map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
		}
	}


//	foreach(array aa in map2)
//		tell(find_player("cookys"),implode(aa,"")+"\n");
	//map2 = copy(areas[area][num][RMAP][y_start..y_start+AREA_VISION_LENGTH-1]);
	map3 = allocate(AREA_VISION_LENGTH, 0);

// 顯示區塊計算公式
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);


	if( range == (AREA_VISION_WIDTH/3)*(AREA_VISION_WIDTH/3) )
	{	
		map3 = allocate(AREA_VISION_LENGTH, allocate(AREA_VISION_WIDTH, 1));
		//for(j=0;j<AREA_VISION_LENGTH;j++)
		//	map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<AREA_VISION_LENGTH;j++)
			map3[j] = allocate(AREA_VISION_WIDTH);

		for(j=0;j<AREA_VISION_LENGTH;j++)
		{
		//	map2[j] = map2[j][x_start..x_start+AREA_VISION_WIDTH-1];

			for(i=0;i<AREA_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"Γ"NOR )
				{
					for(k=j-4;k<=j+4&&k<AREA_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<AREA_VISION_WIDTH;l++)
						{
							if( k>=0 && l>=0 && (l-i)*(l-i) + (k-j)*(k-j) <= 16 )
								map3[k][l] = 1;
						}
				}
				else if( !map3[j][i] && (i-x+x_start)*(i-x+x_start)*0.5 + (j-y+y_start)*(j-y+y_start) <= range )
					map3[j][i] = 1;
			}
		}		
	}

	for(j=0;j<AREA_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H"NOR WHT"│"NOR;

		for(i=0;i<AREA_VISION_WIDTH;i++)
		{
			//tell(find_player("cookys"),sprintf("i:%d j:%d\n%O\n",i,j,map2));
			if( map3[j][i] )
			{
				//if( j == y-y_start && i == x-x_start )
				if( j==y_center-1 && i ==x_center-1 )
					retmap += ansi_part(map2[j][i])+HIC"╳"NOR;
				else 		
					retmap += map2[j][i];
			}

			else
				retmap += WHT"◆"NOR;
		}
		retmap += NOR WHT"│\n"NOR;
	}

	// 圖下的橫線
	retmap += "\e["+(j+2)+";1H"NOR WHT"└["NOR+
	sprintf("%|18s", coor_short_name(loc)+NOR)+
	NOR WHT"]──────────"+
	sprintf(NOR"%|10s"NOR WHT"─"NOR"%|10s"NOR WHT, AREA_D->query_area_name(area), AREA_D->query_section_info(area, num, "name")||("第"+CHINESE_D->chinese_number(num+1)+"區"))+
	"┘\e["+(j+3)+
	";1H"NOR MAG"__________________________________________________________________\e[u"NOR;

	return retmap;
}

mapping corresponding =
([
	"north"		:	"south",
	"south"		:	"north",
	"east"		:	"west",
	"west"		:	"east",
	"northwest"	:	"southeast",
	"northeast"	:	"southwest",
	"southwest"	:	"northeast",
	"southeast"	:	"northwest",
	"up"		:	"down",
	"down"		:	"up",
]);

mapping weather_go =
([
    SUNNY		:	"滿頭大汗地",
    CLOUDY		:	"伴隨著和風",
    RAINY		:	"腳底拖著雨水",
    WINDY		:	"風塵僕僕",
    SNOWY		:	"帶著斑斑白雪",
    SHOWER		:	"從頭到腳濕淋淋地",
    TYPHOON		:	"全身一塌糊塗地",
    FOEHN		:	"撲著熱風"
]);


varargs int valid_move(object me, string direction, array loc)
{
	int x, y, num, *worldmaploc, maplink;

	mapping data;
	string area;
	array nextloc;

	if( !objectp(me) ) return 0;

	if( !loc )
		loc = query_temp("location", me);

	if( !arrayp(loc) )
		return 0;
		
	x = copy(loc[X]);
	y = copy(loc[Y]);
	area = loc[AREA];
	num = loc[NUM];

	worldmaploc = MAP_D->query_world_map_location(area, num);

	switch(direction)
	{
		case "north":		y--;		break;
		case "south":		y++;		break;
		case "west":		x--;		break;
		case "east":		x++;		break;
		case "northeast":	y--;x++; 	break;
		case "southwest":	y++;x--;  	break;
		case "northwest":	y--;x--; 	break;
		case "southeast":	y++;x++; 	break;
		case "maplink"	:	break;
		default: return 0;
	}

	if( y < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;
		
		y = MAP_LENGTH-1;
		worldmaploc[1]++;
		maplink = 1;
	}
	else if( y >= MAP_LENGTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = 0;
		worldmaploc[1]--;
		maplink = 1;
	}

	if( x < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;
		
		x = MAP_WIDTH-1;
		worldmaploc[0]--;
		maplink = 1;
	}
	else if( x >= MAP_WIDTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = 0;
		worldmaploc[0]++;
		maplink = 1;
	}

	if( maplink )
	{
		array linked_map = MAP_D->query_world_map(worldmaploc[0], worldmaploc[1]);
		
		if( linked_map )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);
			
			return MAP_D->valid_move(me, "maplink", nextloc);
		}
		return 0;
	}

	nextloc = arrange_area_location(x, y, area, num);

	if( !valid_coordinate(x, y, area, num) )
		return 0;

	data = AREA_D->query_coor_data(nextloc);

	if( data[FLAGS] & NO_MOVE )
	{
		if( data[TYPE] == WALL )
			return -2;
	}	

	switch(data[TYPE])
	{
	case DOOR:
		if( data["lock"] & LOCKED || (!wizardp(me) && data["lock"] & WIZLOCKED) )
			return -1;
		break;
	}

	if( data[ROOM] && ((direction == "northeast") || (direction == "southwest") || (direction == "northwest") || (direction == "southeast")) )
		return -3;

	return 1;
}

// Called by TIME_D
void downgrade_section_influence()
{
	string city;
	string area;
	int i, influence;
	
	foreach(area in query_areas())
	for(i=0;i<query_area_nums(area);i++)
	{
		if( !mapp(areas[area][INFO]["section"][i]["area_section_influence"]) )
			areas[area][INFO]["section"][i]["area_section_influence"] = allocate_mapping(0);

		foreach(city, influence in areas[area][INFO]["section"][i]["area_section_influence"])
		{
			if( to_int(influence * 0.8) < 1 )
				map_delete(areas[area][INFO]["section"][i]["area_section_influence"], city);
			else
			{
				if( CITY_D->has_building("prodigy", city) )
					areas[area][INFO]["section"][i]["area_section_influence"][city] = to_int(influence * 0.9);
				else
					areas[area][INFO]["section"][i]["area_section_influence"][city] = to_int(influence * 0.8);
			}
		}
	}
}

varargs void move(object me, string direction, int automove)
{
	int x, y, num, cost_stamina, *worldmaploc, maplink;
	mapping data;
	string area, my_idname;
	array loc, nextloc;
	string random_thing = RANDOM_STRING[random(sizeof(RANDOM_STRING))];

	if( !objectp(me) || !arrayp(loc = query_temp("location",me)) ) return;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	area = loc[AREA];
	num = loc[NUM];
	
	if( !area_exist(area) ) return;

	worldmaploc = MAP_D->query_world_map_location(area, num);

	my_idname = me->query_idname();

	switch(direction)
	{
		case "north":		y--;		break;
		case "south":		y++;		break;
		case "west":		x--;		break;
		case "east":		x++;		break;
		case "northeast":	y--;x++; 	break;
		case "southwest":	y++;x--;  	break;
		case "northwest":	y--;x--; 	break;
		case "southeast":	y++;x++; 	break;
		case "maplink"	:	break;
		default: return 0;
	}

	if( y < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;
		
		y = MAP_LENGTH-1;
		worldmaploc[1]++;
		maplink = 1;
	}
	else if( y >= MAP_LENGTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		y = 0;
		worldmaploc[1]--;
		maplink = 1;
	}

	if( x < 0 )
	{
		if( !arrayp(worldmaploc) ) return 0;
		
		x = MAP_WIDTH-1;
		worldmaploc[0]--;
		maplink = 1;
	}
	else if( x >= MAP_WIDTH )
	{
		if( !arrayp(worldmaploc) ) return 0;

		x = 0;
		worldmaploc[0]++;
		maplink = 1;
	}

	if( maplink )
	{
		array linked_map = MAP_D->query_world_map(worldmaploc[0], worldmaploc[1]);
		
		if( linked_map )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);
			
			broadcast(loc, my_idname+"朝"+CHINESE_D->to_chinese(direction)+"離開。\n", 0, me);
			me->move(nextloc);
			broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，自"+CHINESE_D->to_chinese(corresponding[direction])+"走了過來。\n", 0, me);
			
			me->follower_move(loc, nextloc);
			return;
		}
		error(area+" 通往 "+linked_map[0]+" 出口錯誤");
	}
	nextloc = arrange_area_location(x, y, area, num);

	data = AREA_D->query_coor_data(nextloc);

	delete_temp("go_resist", me);

	// 通往區域建築物內部
	if( stringp(data[ROOM]) )
	{
		// 特殊出口移動
		if( data[SPECIAL_EXIT] )
		{
			broadcast(loc, my_idname+"朝"+CHINESE_D->to_chinese(direction)+"離開。\n", 0, me);
			me->move(data[SPECIAL_EXIT]);
			broadcast(data[SPECIAL_EXIT], my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，自"+CHINESE_D->to_chinese(corresponding[direction])+"走了過來。\n", 0, me);

			me->follower_move(loc, data[SPECIAL_EXIT]);

			return;
		}
		else if( file_exists(AREA_ROOM_MODULE(area, num, x, y, data[ROOM])) )
		{
			msg("$ME推開了"+CHINESE_D->to_chinese(direction)+"的門進入了"+BUILDING_D->query_building(data[ROOM])[0]+"。\n",me, 0, 1);
			me->move(AREA_ROOM_MODULE(area, num, x, y, data[ROOM]));
			broadcast(AREA_ROOM_MODULE(area, num, x, y, data[ROOM]), my_idname+"推開了"+CHINESE_D->to_chinese(corresponding[direction])+"的門，"+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，朝屋內走了進來。\n", 0, me);

			me->follower_move(loc, AREA_ROOM_MODULE(area, num, x, y, data[ROOM]));

			return;
		}
		return tell(me, "此建築物內一塌糊塗，目前無法進入。\n");
	}

	remove_call_out(query_temp("call_out_handle/flowing", me));

	// 檢查特殊土地形態的移動
	switch(data[TYPE])
	{
	case RIVER:
		if( !sizeof(me->query_equipping_object(EQ_BOAT[PART_ID])) )
		{
			cost_stamina = 100 - me->query_skill_level("sport")/2;
	
			switch( AREA_D->query_coor_data(nextloc, FLAGS) )
			{
			case FLOW_NORTH	: direction = "north"; break;
			case FLOW_SOUTH	: direction = "south"; break;
			case FLOW_EAST	: direction = "east"; break;
			case FLOW_WEST	: direction = "west"; break;
			default: direction = 0; break;
			}
	
			if( automove )
			{
				if( direction )
					msg("$ME隨著河流慢慢地往"+CHINESE_D->to_chinese(direction)+"飄了過去。\n", me, 0, 1);
				else
					return msg("$ME在河水上載浮載沉。\n", me, 0, 1);
			}
			else if( !me->cost_stamina(cost_stamina) )
				return tell(me, pnoun(2, me)+"沒有體力前進了。\n");
			else
				msg("$ME奮力地往"+CHINESE_D->to_chinese(direction)+"游過去，花費 "+cost_stamina+" 點體力。\n",me,0,1);
	
			if( !random(100) )
			{
				msg("$ME游泳游到一半，突然間被漩渦捲了下去，鼠蹊部狠狠地撞上水底的"+HIY+random_thing+NOR+"。\n", me, 0, 1);
				me->faint();
			}
	
			set_temp("call_out_handle/flowing",call_out( (: move, me, direction, 1 :), 2), me);
		}
		else
			msg("$ME往"+CHINESE_D->to_chinese(direction)+"的河流航行前進。\n",me,0,1);

		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	case FOREST:
		if( sizeof(me->query_equipping_object(EQ_VEHICLE[PART_ID])) )
			msg("$ME往"+CHINESE_D->to_chinese(direction)+"的濃密森林驅車前進。\n",me,0,1);
		else
		{
			cost_stamina = 25 - me->query_skill_level("sport")/5;
	
			if( !me->cost_stamina(cost_stamina) )
				return tell(me, pnoun(2, me)+"沒有體力前進了。\n");
				
			msg("$ME小心地往"+CHINESE_D->to_chinese(direction)+"的濃密森林前進，花費 "+cost_stamina+" 點體力。\n",me,0,1);
		}
		
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	case MOUNTAIN:
		if( sizeof(me->query_equipping_object(EQ_VEHICLE[PART_ID])) )
			msg("$ME往"+CHINESE_D->to_chinese(direction)+"的山脈驅車前進。\n",me,0,1);
		else
		{
			cost_stamina = 90 - me->query_skill_level("sport")/2;
		
			if( !me->cost_stamina(cost_stamina) )
				return tell(me, pnoun(2, me)+"沒有體力前進了。\n");
				
			msg("$ME氣喘噓噓地往"+CHINESE_D->to_chinese(direction)+"的山脈前進，花費 "+cost_stamina+" 點體力。\n",me,0,1);
		}
		
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	case SEA:
		if( !sizeof(me->query_equipping_object(EQ_BOAT[PART_ID])) )
			return tell(me, pnoun(2, me)+"必須搭乘船隻才能夠出海。\n");
		
		msg("$ME往"+CHINESE_D->to_chinese(direction)+"的海洋航行前進。\n",me,0,1);
			
		me->move(nextloc);
		me->follower_move(loc, nextloc);
		return;
		break;
	}

	// 一般地圖系統移動
	broadcast(loc, my_idname+"朝"+CHINESE_D->to_chinese(direction)+"離開。\n", 0, me);
	me->move(nextloc);
	broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，自"+CHINESE_D->to_chinese(corresponding[direction])+"走了過來。\n", 0, me);

	me->follower_move(loc, nextloc);
}

void remove_living_icon(object me, array loc)
{
	int x, y, num;
	string area;
	object ob, *users = allocate(0);
	int user_count, boss_count, living_count, autokill_count;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	area = loc[AREA];
	num = loc[NUM];

	if( !area_exist(area, num) || !valid_coordinate(x, y, area, num) || undefinedp(areas[area][num][RMAP]) || undefinedp(areas[area][num][MAP]) ) return;

	foreach(ob in (present_objects(me)-({me})))
	{
		if( userp(ob) )
		{
			users += ({ ob });
			user_count++;
		}
		else if( !ob->is_dead() )
		{
			if( ob->is_boss() )
				boss_count++;
			else if( query("autokill", ob) )
				autokill_count++;
			else if( ob->is_living() && !ob->is_module_npc() )
				living_count++;
		}
	}

	if( user_count > 1 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + HIC"※"NOR;
	else if( user_count == 1 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + (query("gender", users[0]) == MALE_GENDER ? HIG + "♂" : HIR + "♀") + NOR;
	else if( boss_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIR+"♁"+NOR;
	else if( autokill_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIM+"♁"+NOR;
	else if( living_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIW+"♁"+NOR;
	else
		areas[area][num][RMAP][y][x] = areas[area][num][MAP][y][x];
}

void set_living_icon(object me, array loc)
{
	int x, y, num;
	string area;
	object ob, *users = allocate(0);
	int user_count, boss_count, living_count, autokill_count;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	area = loc[AREA];
	num = loc[NUM];

	if( !valid_coordinate(x, y, area, num) ) return;

	foreach(ob in present_objects(me))
	{
		if( userp(ob) )
		{
			users += ({ ob });
			user_count++;
		}
		else if( !ob->is_dead() )
		{
			if( ob->is_boss() )
				boss_count++;
			else if( query("autokill", ob) )
				autokill_count++;
			else if( ob->is_living() && !ob->is_module_npc() )
				living_count++;
		}
	}

	if( user_count > 1 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + HIC"※"NOR;
	else if( user_count == 1 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x]) + (query("gender", me) == MALE_GENDER ? HIG + "♂" : HIR + "♀") + NOR;
	else if( boss_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIR+"♁"+NOR;
	else if( autokill_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIM+"♁"+NOR;
	else if( living_count > 0 )
		areas[area][num][RMAP][y][x] = ansi_part(areas[area][num][MAP][y][x])+HIW+"♁"+NOR;
}


// 將地圖轉換為 HTML 格式
void translate_map_to_html(string area, int num)
{
	array arr;
	string map_graph = query_area_idname(area, num)+"\n";

	if( area_exist(area, num) )
	{
		foreach( arr in areas[area][num][RMAP] )
			map_graph += implode(arr, "")+"\n";

		write_file("/www/map/areamap_"+area+"_"+num+"_ansi", map_graph, 1);

		HTML_D->make_html(remove_ansi(map_graph), ([
			"bgcolor"	:"black",
			"filename"	:"/www/map/areamap_"+area+"_"+num+".html",
			"title"		:remove_ansi(MUD_FULL_NAME+" "+AREA_D->query_area_idname(area, num)),
			"refresh"	:300,
			"fontsize"	:12,
		    ]));
	}
}

// 回傳郊區所佔領城市
string query_lead_city(string area, int num)
{
	string *sort_city;
	mapping area_section_influence;

	if( !area_exist(area, num) ) return 0;
		
	area_section_influence = query_section_info(area, num, "area_section_influence");

	if( !mapp(area_section_influence) ) return 0;
		
	sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));
	
	if( !sizeof(sort_city) || area_section_influence[sort_city[0]] < 50000 )
		return 0;

	return sort_city[0];
}

// 檢查或修改座標資料
void modify_coor_data()
{
	foreach( string area, mapping map1 in areas )
	foreach( mixed num, mapping map2 in map1 )
	{
		if( !intp(num) ) continue;
			
		CHANNEL_D->channel_broadcast("sys", "checking "+area+"-"+num+" data");
			
		for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{
			if( !mapp(areas[area][num][DATA][y][x]) )
			{
				CHANNEL_D->channel_broadcast("sys", area+"-"+num+" "+y+","+x+" data is broken");
			}
		}
	}
}

// 聯結 data
void link_data()
{
	areas = AREADATA_D->query_data();

	// 對所有區域進行編號排序
	assign_areas_num();
}

private void create()
{
	if( clonep() )
		destruct(this_object());

	link_data();
}

string query_name()
{
	return "郊區系統(AREA_D)";
}
