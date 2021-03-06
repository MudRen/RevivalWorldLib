/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : city_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : 城市精靈, 處理所有城市資料庫
 * Update :
 *  o 2002-09-17 Clode 重新設計城市資料結構與儲存資料程序
 *  o 2002-09-17 Clode 修改部份 code 提高容錯能力
 *
 -----------------------------------------
 */


#include <ansi.h>
#include <map.h>
#include <lock.h>
#include <location.h>
#include <daemon.h>
#include <object.h>
#include <citydata.h>
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
#include <citycondition.h>

#define LOG			"daemon/city"
#define DEFAULT_COOR_DATA	([ ])

#define SAVE_INFO		(1<<0)
#define SAVE_DATA		(1<<1)
#define SAVE_MAP		(1<<2)
#define SAVE_ESTATE		(1<<3)
#define SAVE_ALL		(SAVE_INFO|SAVE_DATA|SAVE_MAP|SAVE_ESTATE)

#define OCCUPY_MONEY	100000000

#define RANDOM_STRING		({ "保齡球", "充氣娃娃", "棒棒糖", "榴槤", "按摩棒", "燃燒中的主機", "圓河豚", "高壓電箱", "碎玻璃", "疑似爆裂物", "黑道老大", "羊肉爐", "發情的動物", "核彈發射器", "捕鳥器" })
#define CITY_SCALE		({HIY"營"NOR YEL"地"NOR, HIY"部"NOR YEL"落"NOR, HIY"村"NOR YEL"落"NOR, HIG"市"NOR GRN"集"NOR, HIG"鄉"NOR GRN"村"NOR, HIG"小"NOR GRN"鎮"NOR, HIW"縣"NOR WHT"城"NOR, HIW"城"NOR WHT"鎮"NOR, HIW"城"NOR WHT"市"NOR, HIC"都"NOR CYN"市"NOR, HIC"都"NOR CYN"會"NOR, HIC"大"NOR CYN"都"HIC"會"NOR, HIR"都"NOR RED"心"NOR, HIR"大"NOR RED"都心"NOR, HIR"首"NOR RED"都"NOR})

private int number;
private int save_all_time;

private mapping cities;
private mapping sort_save_list;

varargs int city_exist(string city, int num)
{
	if( undefinedp(num) )
		return mapp(cities[city]);
	else
		return mapp(cities[city]) && mapp(cities[city][num]);
}

varargs int map_exists(string map, int section)
{
	if( undefinedp(section) )
		return mapp(cities[map]);
	else
		return mapp(cities[map]) && mapp(cities[map][section]);
}

int query_city_nums(string city)
{
	if( undefinedp(cities[city]) )
		return 0;

    return sizeof(filter_array(keys(cities[city]), (: intp($1) :)));
}

int query_city_num(string city)
{
	return query_city_nums(city);
}

int is_city_location(mixed map)
{
	if( arrayp(map) )
		return city_exist(map[CITY], map[NUM]);
	else if( stringp(map) )
		return city_exist(map);

	return 0;
}

varargs string query_city_id(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) ) return 0;
		return cities[city][INFO]["color_id"];
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["color_id"] + " "+(num+1);
	}
}

varargs string query_city_name(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) || !cities[city] || !cities[city][INFO] ) return 0;
		return cities[city][INFO]["name"];
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["name"] + (cities[city][INFO]["section"][num]["name"] || "第"+CHINESE_D->chinese_number(num+1)+"衛星都市");
	}
}

varargs string query_city_idname(string city, int num)
{
	if( undefinedp(num) )
	{
		if( !city_exist(city) ) return 0;
		if( !cities[city] || !cities[city][INFO] ) return 0;
		return cities[city][INFO]["name"]+"<"+cities[city][INFO]["color_id"]+">";
	}
	else
	{
		if( !city_exist(city, num) ) return 0;
		return cities[city][INFO]["name"]+(cities[city][INFO]["section"][num]["name"] || "第"+CHINESE_D->chinese_number(num+1)+"衛星都市")+"<"+cities[city][INFO]["color_id"]+" "+(num+1)+">";
	}
}

mapping query_public_facility(string city)
{
	int num, x, y;
	mapping all_coor_data;
	mapping facility = allocate_mapping(0);

	if( !CITY_D->city_exist(city) ) return 0;

	while(CITY_D->city_exist(city, num))
	{
		all_coor_data = cities[city][num][DATA];

		for(x=0;x<100;x++)
			for(y=0;y<100;y++)
			{
				if( all_coor_data[y][x][ROOM] )
					switch( all_coor_data[y][x][ROOM] )
					{
						case "cityhall":
						case "trading_post":
						case "recycle":
						case "labor":
						case "postoffice":
						case "bank":
						case "auction":
						case "jail":
						case "trainstation":
						case "prodigy":
						case "futures":
						case "church":
						case "hospital":
							facility["公共建築物"]++;
							break;
						default:
							break;
					}
				else
					switch( all_coor_data[y][x][TYPE] )
					{
						case MOUNTAIN:
							facility["山脈"]++;
							break;
						case TREE:
						case GRASS:
						case PAVILION:
						case FOREST:
							facility["森林"]++;
							break;
						case POOL:
						case RIVER:
							facility["河流"]++;
							break;
						case ROAD:
							facility["道路"]++;
							break;
						case BRIDGE:
							facility["橋樑"]++;
							break;
						case STATUE:
						case LIGHT:
						case 0:
							facility["空地"]++;
							break;	
					}
			}

		num++;
	}

	return facility;
}

int valid_coordinate(mixed x, int y, string city, int num)
{
	if( intp(x) )
		return x>=0 && x<MAP_WIDTH && y>=0 && y<MAP_LENGTH && city_exist(city, num);
	else if( arrayp(x) )
		return x[X]>=0 && x[X]<MAP_WIDTH && x[Y]>=0 && x[Y]<MAP_LENGTH && city_exist(x[CITY], x[NUM]);
	else
		return 0;
}

/* 對所有城市編號 */
void assign_cities_num()
{
	number = 0;

	sort_save_list = allocate_mapping(0);

	foreach( string city in get_dir("/city/") )
	{
		if( file_size("/city/"+city) != -2 ) continue;

		sort_save_list[number++] = city;
	}
}

void save_options(string city, int num, int bit)
{
	// 轉換地圖至 HTML
	CITY_D->translate_map_to_html(city, num);

	// 寫入基本資訊檔
	if( bit & SAVE_INFO )
	{

		if( !city_exist(city) || undefinedp(cities[city][INFO]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+city+" 基本資訊");
			log_file(LOG, "喪失 "+city+" 基本資訊");
		}
		else if( !write_file(CITY_INFO(city), save_variable( copy(cities[city][INFO]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+city+" 基本資訊檔");
			log_file(LOG, "無法寫入 "+city+" 基本資訊檔");
		}
	}

	// 寫入座標資料檔
	if( bit & SAVE_DATA )
	{
		if( !city_exist(city) || undefinedp(cities[city][num][DATA]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+city+" 座標資料");
			log_file(LOG, "喪失 "+city+" 座標資料");
		}

		else if( !write_file(CITY_NUM_DATA(city, num), save_variable( copy(cities[city][num][DATA]) ), 1) )
		{
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+city+" "+num+" 座標資料檔");
			log_file(LOG, "無法寫入 "+city+" "+num+" 座標資料檔");
		}
	}

	// 寫入地圖資料檔
	if( bit & SAVE_MAP )
	{
		if( !city_exist(city) || undefinedp(cities[city][num][MAP]) )
		{
			CHANNEL_D->channel_broadcast("sys", "喪失 "+city+" 地圖資料");
			log_file(LOG, "喪失 "+city+" 地圖資料");
		}

		else if( !write_file(CITY_NUM_MAP(city, num), save_variable( copy(cities[city][num][MAP]) ), 1) )
		{	
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+city+" "+num+" 地圖資料檔");
			log_file(LOG, "無法寫入 "+city+" "+num+" 地圖資料檔");
		}
	}

	// 寫入房地產資料檔
	if( bit & SAVE_ESTATE )
	{
		if( !write_file(CITY_NUM_ESTATE(city, num), save_variable(ESTATE_D->query_estate(city, num)), 1) )
		{	
			CHANNEL_D->channel_broadcast("sys", "無法寫入 "+city+" "+num+" 房地產資料檔");
			log_file(LOG, "無法寫入 "+city+" "+num+" 房地產資料檔");
		}
	}
}

varargs void save_city(string city, int num, int bit)
{
	int time_exp;

	if( !undefinedp(num) )
	{
		string options = "";

		if( !city_exist(city, num) ) return;

		time_exp = time_expression { save_options(city, num, undefinedp(bit) ? SAVE_ALL : bit); };

		if( bit & SAVE_INFO ) options += "基本資訊 ";
		if( bit & SAVE_DATA ) options += "座標資料 ";
		if( bit & SAVE_MAP  ) options += "地圖資料 ";
		if( bit & SAVE_ESTATE) options += "房地產資料 ";

		if( options != "" )
			CHANNEL_D->channel_broadcast("nch",sprintf("城市：儲存城市「%s」%s(%.6f sec)。",query_city_idname(city, num), options, time_exp/1000000.));
			
		call_out((:ESTATE_D->handle_freeland($(city), $(num)):), (num+1)*5);
	}
	else if( city_exist(city) )
	{
		for(num=0;city_exist(city, num);num++)
		{
			time_exp += time_expression { save_options(city, num, SAVE_ALL); };
			call_out((:ESTATE_D->handle_freeland($(city), $(num)):), (num+1)*5);
		}

		CHANNEL_D->channel_broadcast("nch",sprintf("城市：儲存城市「%s」所有資料(%.6f sec)。",query_city_idname(city), time_exp/1000000.));
	}
}

/* 由 TIME_D 定期呼叫, 城市資料輪流儲存 */
void time_distributed_save()
{
	string city;
	int num;

	if( sizeof(cities) < 1 ) return;

	city = sort_save_list[++number] || sort_save_list[(number=0)];

	for(num=0;city_exist(city, num);num++)
	{
		broadcast(cities[city][num][CROOM], "\n"+query_city_idname(city)+"的地心深處突然傳來「轟」的一聲，隨著的是大地一陣搖晃。\n");
		call_out( (: save_city :), (num+1)*20, city, num);
	}

	CITY_D->rehash_flourish(city);

	if( CITY_D->query_city_info(city, "crashtime") > 0 )
	{
		if( CITY_D->query_city_info(city, "crashtime") - time() < 0 )
			CITY_D->city_collapse(city);
	}
}

/* 儲存所有 city 資訊 */
int save_all()
{
	int time_exp;

	// 為避免在 shutdown 時, 造成重覆 save_all 的情形, 故設置 save_all_time 作檢查
	if( time() - save_all_time < 30 ) return 0;

	save_all_time = time();

	time_exp = time_expression {
		foreach( string city in get_dir(CITY_ROOT) )
		if( file_size(CITY_PATH(city)) == -2 )
			save_city(city);
	};// end of time_expression

	CHANNEL_D->channel_broadcast("nch",sprintf("城市：分散儲存所有城市資料完畢(共費 %.6f sec)。", time_exp/1000000.));
	return 1;
}

// 計算城市升級所需資源
void calculate_level_up_resource(string city, int age)
{
	int num = query_city_nums(city);
	int amount = (25000000 + 20000000*(num-1))*to_int(pow(age, 2));

	if( !mapp(cities[city][INFO]["level_up_resource"]) )
		cities[city][INFO]["level_up_resource"] = allocate_mapping(0);

	cities[city][INFO]["level_up_resource"]["metal_need"] = amount;
	cities[city][INFO]["level_up_resource"]["stone_need"] = amount;
	cities[city][INFO]["level_up_resource"]["water_need"] = amount;
	cities[city][INFO]["level_up_resource"]["wood_need"] = amount;
	cities[city][INFO]["level_up_resource"]["fuel_need"] = amount;
}

// 已存在城市的重設資料
void reset_city_data(string city)
{
	int x, y;

	// 必須是 fallen city 才能夠重設資料
	if( !cities[city] 
	    || !cities[city][INFO]
	    || !cities[city][INFO]["fallen"] ) return;


	// 重設市民資料
	cities[city][INFO]["citizens"] 			= allocate(0);
	cities[city][INFO]["citizens_jointime"] 	= allocate_mapping(0);

	// 重設市長
	cities[city][INFO]["government/mayor"] 		= 0;

	// 重設官員
	cities[city][INFO]["government/officer"] 	= allocate_mapping(0);

	// 重設時代
	cities[city][INFO]["age"]			= 0;

	// 重設經費資料
	cities[city][INFO]["fund"]			= allocate_mapping(0);

	// 重設市府資產資料
	cities[city][INFO]["assets"]			= "0";

	cities[city][INFO]["crashtime"]			= 0;	
	// 重設分區資料
	cities[city][INFO]["section"] 			= allocate_mapping(0);
	cities[city][INFO]["section"][0] 		= allocate_mapping(0);

	// 居民人口
	cities[city][INFO]["section"][0]["flourish"]	= 0;
	cities[city][INFO]["section"][0]["resident"] 	= 0;

	// 總財產
	cities[city][INFO]["GNP"]			= "0";
	cities[city][INFO]["NNP"]			= "0";

	// 升級資源
	cities[city][INFO]["level_up_resource"]		= allocate_mapping(0);

	for(y=0;y<MAP_LENGTH;y++)
		for(x=0;x<MAP_WIDTH;x++)
		{
			switch(cities[city][0][DATA][y][x][TYPE])
			{
			case RIVER:
			case FOREST:
			case MOUNTAIN:

				break;
			default:
				cities[city][0][DATA][y][x] = DEFAULT_COOR_DATA;
				cities[city][0][RMAP][y][x] = "．";
				cities[city][0][MAP][y][x] = "．";	
				break;
			}
		}
}

// 創造城市初始化資料
void initialize_city_data(string city)
{
	if( !stringp(city) ) return;

	cities[city] 					= allocate_mapping(0);
	cities[city][INFO] 				= allocate_mapping(0);
	cities[city][INFO]["citizens"] 			= allocate(0);
	cities[city][INFO]["citizens_jointime"] 	= allocate_mapping(0);
	cities[city][INFO]["government/mayor"] 		= 0;
	cities[city][INFO]["government/officer"] 	= allocate_mapping(0);
	cities[city][INFO]["fund"]			= allocate_mapping(0);
	cities[city][INFO]["name"] 			= NOR WHT"廢棄都市"NOR;
	cities[city][INFO]["age"]			= 0;
	cities[city][INFO]["emblem"]			= ({ "          ", " 廢棄都市 ", "          " });
	cities[city][INFO]["assets"]			= "0";

	cities[city][INFO]["section"]   		= allocate_mapping(0);
	cities[city][INFO]["section"][0] 		= allocate_mapping(0);
	cities[city][INFO]["section"][0]["resident"] 	= 0;
	cities[city][INFO]["section"][0]["flourish"] 	= 0;

	cities[city][INFO]["fallen"]			= 1;

	cities[city][0] 				= allocate_mapping(5);
	cities[city][0][DATA] 				= allocate_mapping(0);
	cities[city][0][MAP]  				= allocate(0);
	cities[city][0][RMAP] 				= allocate(0);
	cities[city][0][CROOM]			= load_cityroom(city, 0);

	calculate_level_up_resource(city, 0);
}

// 佔領廢棄城市成為新都市
void occupy_new_city(object me, string cityid, string cityname, string moneyunit)
{
	array loc = query_temp("location", me);
	string city, newcity, defaultmoneyunit = MONEY_D->query_default_money_unit();
	int *worldmaploc, strlen;

	if( !arrayp(loc) || !cityid ) return;

	city = loc[CITY];

	if( !cities[city][INFO]["fallen"] )
		return tell(me, "這座城市並不是廢棄城市，無法佔領。\n");

	strlen = noansi_strlen(cityname);
	if( strlen < 2 || strlen > 10 )
		return tell(me, "城市的中文名稱字數必須介於 1 - 5 個中文字。\n");

	strlen = noansi_strlen(cityid);
	if( strlen < 3 || strlen > 12 )
		return tell(me, "城市的英文名稱字數必須介於 3 - 12 個英文字。\n");

	if( !is_chinese(cityname) )
		return tell(me, "城市的中文名稱只能使用中文。\n");

	if( !is_english(cityid) )
		return tell(me, "城市的英文名稱只能使用英文字母。\n");

	moneyunit = trim(upper_case(remove_ansi(moneyunit)));

	if( !is_english(moneyunit) )
		return tell(me, "貨幣單位必須是英文。\n");

	if( strlen(moneyunit) != 2 )
		return tell(me, "貨幣單位必須是 2 個英文字母。\n");

	cityid = lower_case(cityid);
	cityid = kill_repeat_ansi(replace_string(cityid, "\n", "")+NOR);
	newcity = remove_ansi(cityid);
	cityid = capitalize(cityid);

	cityname = kill_repeat_ansi(replace_string(cityname, "\n", "")+NOR);

	if( newcity != city && (city_exist(newcity) || AREA_D->area_exist(newcity)) )
		return tell(me, "區域 "+newcity+" 已經存在，請使用其他名稱。\n");

	if( MONEY_D->money_unit_to_city(moneyunit) )
		return tell(me, "幣制單位 $"+moneyunit+" 已經被使用。\n");

	if( !me->spend_money(defaultmoneyunit, OCCUPY_MONEY) )
		return tell(me, pnoun(2, me)+"必須擁有 $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" 的現金來整頓這個廢棄城市後才能佔領。\n");

	tell(me, pnoun(2, me)+"花費了 $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" 元整頓並佔領了這座廢棄都市。\n");
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"花費了 $"+defaultmoneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" 整頓並佔領了"+query_city_idname(city)+"，並取名為「"+cityname+"<"+cityid+">」，貨幣單位為「"+moneyunit+"」。");
	HISTORY_D->add_history("city/"+newcity, me->query_idname()+"佔領了"+query_city_idname(city)+"，建立全新城市「"+cityname+"<"+cityid+">」，貨幣單位為「"+moneyunit+"」。");

	if( newcity != city )
	{	
		// 切換 cityroom
		set("city", newcity, cities[city][0][CROOM]);
		set("num", 0, cities[city][0][CROOM]);

		// 切換城市資料
		cities[newcity] = copy(cities[city]);

		foreach(object ob in all_inventory(cities[city][0][CROOM]))
		{
			loc = query_temp("location", ob);

			if( !arrayp(loc) ) continue;

			ob->move(arrange_city_location(loc[X], loc[Y], newcity, 0));
		}
		rename("/city/"+city+"/", "/city/"+newcity+"/");



		// 刪除舊有資料
		map_delete(cities, city);
	}

	cities[newcity][INFO]["government/mayor"] = me->query_id(1);
	cities[newcity][INFO]["id"] = newcity;
	cities[newcity][INFO]["color_id"] = cityid;
	cities[newcity][INFO]["name"] = cityname;
	cities[newcity][INFO]["age"] = 1;
	cities[newcity][INFO]["time"] = time();
	cities[newcity][INFO]["emblem"]	= ({HIR"★"HIY"★"HIG"★"HIB"★"HIM"★"NOR, sprintf("%|10s", cityname), HIM"★"HIB"★"HIG"★"HIY"★"HIR"★"NOR});

	// 計算升級所需資源
	calculate_level_up_resource(newcity, cities[newcity][INFO]["age"]);

	map_delete(cities[newcity][INFO], "fallen");

	CITY_D->register_citizen(me->query_id(1), newcity);
	set("city", newcity, me);

	// 建立金融機制
	MONEY_D->create_new_finance_system(newcity, moneyunit);

	// 建立房地產資料
	ESTATE_D->create_estate(newcity, 0);

	// 重整 SHOPPING_D 資料
	SHOPPING_D->refresh_shopping();

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	if( arrayp(worldmaploc) )
	{
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
		MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], newcity, 0);
	}

	save_city(newcity);

	// 重新排序城市資訊
	assign_cities_num();
}

varargs int has_building(string buildingtype, string city, int num)
{
	string filename;
	
	if( undefinedp(num) )
	{	
		for(num=0;num<query_city_nums(city);++num)
		{
			foreach( filename in get_dir(CITY_NUM_ROOM(city, num)) )
			{
				if( strsrch(filename, buildingtype) != -1 )
					return 1;	
			}
		}	
	}
	else
	{
		foreach( filename in get_dir(CITY_NUM_ROOM(city, num)) )
		{
			if( strsrch(filename, buildingtype) != -1 )
				return 1;	
		}
	}

	return 0;
}

void occupy_section_city(object me)
{
	int num;
	array loc = query_temp("location", me);
	string city, mycity, moneyunit;
	int *worldmaploc;

	mycity = query("city", me);

	if( !city_exist(mycity) || !arrayp(loc) ) return;

	city = loc[CITY];

	while(city_exist(mycity, num)) num++;

	if( !cities[city][INFO]["fallen"] )
		return tell(me, "這座城市並不是廢棄城市，無法佔領。\n");

	moneyunit = MONEY_D->city_to_money_unit(mycity);

	if( count(cities[mycity][INFO]["assets"], "<", 1000000000 ) )
		return tell(me, CITY_D->query_city_idname(mycity)+"的市府資產至少需擁有 $"+moneyunit+" "+NUMBER_D->number_symbol(1000000000)+" 才能再佔領新的衛星都市。\n");

	CITY_D->change_assets(mycity, "-"+OCCUPY_MONEY);

	// 切換 cityroom
	set("city", mycity, cities[city][0][CROOM]);
	set("num", num, cities[city][0][CROOM]);

	// 切換城市資料
	cities[mycity][num] = copy(cities[city][0]);
	cities[mycity][INFO]["section"][num] = allocate_mapping(0);

	foreach(object ob in all_inventory(cities[city][0][CROOM]))
	{
		loc = query_temp("location", ob);

		if( !arrayp(loc) ) continue;

		ob->move(arrange_city_location(loc[X], loc[Y], mycity, num));
	}

	mkdir(CITY_NUM(mycity, num));		// 子城市目錄
	mkdir(CITY_NUM_ROOM(mycity, num));	// 子城市房間目錄

	tell(me, pnoun(2, me)+"花費了市政府資產 $"+moneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" 元整頓並佔領了這座廢棄都市。\n");
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"花費了市政府資產 $"+moneyunit+" "+NUMBER_D->number_symbol(OCCUPY_MONEY)+" 整頓並佔領了"+query_city_idname(city)+"，成為「"+query_city_idname(mycity, num)+"」。");
	HISTORY_D->add_history("city/"+mycity, me->query_idname()+"佔領了"+query_city_idname(city)+"，成為「"+query_city_idname(mycity, num)+"」。");

	// 刪除舊有資料
	map_delete(cities, city);
	rmtree(CITY_PATH(city));

	// 建立房地產資料
	ESTATE_D->create_estate(mycity, num);

	// 重整 SHOPPING_D 資料
	SHOPPING_D->refresh_shopping();

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	if( arrayp(worldmaploc) )
	{
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
		MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], mycity, num);
	}

	save_city(mycity);

	// 重新排序城市資訊
	assign_cities_num();
	
	// 計算升級所需資源
	calculate_level_up_resource(mycity, cities[mycity][INFO]["age"]);
}

// 建立全新地形與重設資料
void make_new_terrain(string city)
{
	int x, y;
	array newmap;
	mapping newdata;

	newdata = allocate_mapping(100);

	// 建立預設座標資料
	for(y=0;y<MAP_LENGTH;y++)
	{
		newdata[y] = allocate_mapping(1);

		for(x=0;x<MAP_WIDTH;x++)
			newdata[y][x] = copy(DEFAULT_COOR_DATA);
	}

	// 建立城市標準地圖
	newmap = copy(allocate(MAP_LENGTH, copy(allocate(MAP_WIDTH, "．"))));

	// 建立特殊地形
	TERRAIN_D->create_city_map_terrain(ref newmap, ref newdata);

	cities[city][0][MAP] = copy(newmap);
	cities[city][0][RMAP]= copy(newmap);
	cities[city][0][DATA]= copy(newdata);

	log_file(LOG, "建立新城市地圖 "+city);

	save_city(city, 0, SAVE_ALL);
}

// 建立新城市地圖
void create_new_city()
{
	int i;
	string city;

	while(cities[(city = "fallencity"+(++i))]) ;

	// 建立城市檔案目錄
	mkdir(CITY_PATH(city));

	// 初始化城市資料
	initialize_city_data(city);

	cities[city][INFO]["id"] 		= city;
	cities[city][INFO]["color_id"]		= NOR WHT+capitalize(city)+NOR;

	mkdir(CITY_NUM(city, 0));		// 子城市目錄
	mkdir(CITY_NUM_ROOM(city, 0));		// 子城市房間目錄

	make_new_terrain(city);

	// 啟始分區天氣
	NATURE_D->activate_nature(cities[city][0][CROOM]);

	// 重整 SHOPPING_D 資料
	SHOPPING_D->refresh_shopping();

	log_file(LOG, "建立新城市地圖 "+city);

	CHANNEL_D->channel_broadcast("news", HIG"世界地圖上的某處出現了新的區域："NOR+query_city_idname(city));

	// 重新編號城市儲存順序
	assign_cities_num();

	save_city(city, 0, SAVE_ALL);
}

// 放棄分區
void section_collapse(string city, int num, string debtassets)
{
	object inv;
	object room, cityroom;
	string filename, newcity;
	int *worldmaploc;
	array loc;

	cityroom = cities[city][num][CROOM];

	// 移除城市內部所有房間內之所有物件
	foreach( filename in get_dir(CITY_NUM_ROOM(city, num)) )
	{
		reset_eval_cost();

		room = find_object(CITY_NUM_ROOM(city, num)+filename[0..<3]);

		if( !objectp(room) ) continue;
		
		foreach( inv in all_inventory(room) )
		{
			reset_eval_cost();

			if( userp(inv) )
			{
				inv->move(WIZ_HALL);
				msg("$ME所在的城市政府瓦解了，建築物遭到破壞，只好流浪至巫師大廳。\n",inv,0,1);
			}
			else
				destruct(inv);
		}
		destruct(room);
	}


	// 移除子城市房地產資訊
	ESTATE_D->remove_zone_estate(city, num);

	// 將衛星都市資料轉移成新城市
	if( num > 0 )
	{
		newcity = city+num;

		mkdir(CITY_PATH(newcity));

		// 轉換資料
		initialize_city_data(newcity);
		cities[newcity][0] = copy(cities[city][num]);
		cities[newcity][INFO] = copy(cities[city][INFO]);
		cities[newcity][INFO]["id"] = newcity;
		cities[newcity][INFO]["color_id"] = cities[city][INFO]["color_id"]+num;

		cities[newcity][INFO]["fallen"] = 1;
		reset_city_data(newcity);
		cities[newcity][INFO]["assets"] = debtassets || "0";
		map_delete(cities[city], num);

		// 移動子城市目錄
		rename(CITY_NUM(city, num), CITY_NUM(newcity, 0));

		// 切換 cityroom
		set("city", newcity, cityroom);
		set("num", 0, cityroom);

		cities[newcity][0][CROOM] = cityroom;

		foreach(object ob in all_inventory(cityroom))
		{
			loc = query_temp("location", ob);

			if( !arrayp(loc) ) continue;

			ob->move(arrange_city_location(loc[X], loc[Y], newcity, 0));
		}

		worldmaploc = MAP_D->query_world_map_location(city, num);

		if( arrayp(worldmaploc) )
		{
			MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);
			MAP_D->set_world_map(worldmaploc[0], worldmaploc[1], newcity, 0);
		}
		
		save_city(newcity, 0, SAVE_ALL);
	}

	// 重新編號城市儲存順序
	assign_cities_num();
}

// 城市毀滅(重設城市)
void city_collapse(string city)
{
	int num;
	object citizen;
	string id;
	string debtassets;

	if( !city_exist(city) ) return;

	if( count(cities[city][INFO]["assets"], ">=", 0) ) return;

	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+HIR"負債超過三天導致政府完全瓦解，城市發生嚴重暴動，居民撤離，全部建設破壞消失。"NOR);
	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+HIR"政府崩潰了，從此消失在這個世界中，僅剩下眾人的追思與龐大的負債。"NOR);
	HISTORY_D->add_history("city/"+city, query_city_idname(city)+HIR"政府崩潰了，從此消失在這個世界中。"NOR);

	debtassets = count(
	    EXCHANGE_D->convert(to_int(cities[city][INFO]["assets"]), MONEY_D->city_to_money_unit(city), MONEY_D->query_default_money_unit())
	    , "/", CITY_D->query_sections(city));

	// 處理市民資料
	foreach( id in cities[city][INFO]["citizens"] )
	{
		reset_eval_cost();

		citizen = load_user(id);

		if( !objectp(citizen) ) continue;

		delete("city", citizen);

		tell(citizen, pnoun(2, citizen)+"的城市崩潰了，"+pnoun(2, citizen)+"成為無市籍的遊民。\n");

		citizen->save();

		if( !userp(citizen) )
			destruct(citizen);
	}

	while(city_exist(city, num))
	{
		reset_eval_cost();
		
		section_collapse(city, num, debtassets);

		num++;
	}

	cities[city][INFO]["fallen"] = 1;
	reset_city_data(city);
	cities[city][INFO]["assets"] = debtassets;

	// 刪除城市金融系統資訊
	MONEY_D->remove_finance_system(city);

	// 重整 SHOPPING_D 資料
	SHOPPING_D->refresh_shopping();

	save_city(city, 0, SAVE_ALL);

	// 重新編號城市儲存順序
	assign_cities_num();
	
	// 重整種植系統
	GROWTH_D->refresh_growth();
}


// 刪除整個城市的所有資料
void delete_city(string city)
{
	object room, inv;
	object citizen;
	string id, filename;
	int *worldmaploc;

	// 只能刪除廢棄城市
	if( !city_exist(city) || !cities[city][INFO]["fallen"] ) return;

	worldmaploc = MAP_D->query_world_map_location(city, 0);

	// 移除城市內部所有房間內之所有物件
	foreach( filename in get_dir(CITY_NUM_ROOM(city, 0)) )
	{
		room = find_object(CITY_NUM_ROOM(city, 0)+filename[0..<3]);

		if( !objectp(room) ) continue;

		foreach( inv in all_inventory(room) )
		{
			if( userp(inv) )
			{
				inv->move(WIZ_HALL);
				msg("$ME所在的城市滅亡了，只好流浪至巫師大廳。\n",inv,0,1);
			}
			else
				destruct(inv);
		}
		destruct(room);
	}

	// 移除所有在城市物件中的物件
	foreach( inv in all_inventory(cities[city][0][CROOM]) )
	{
		if( userp(inv) )
		{
			reset_screen(inv);
			inv->move(WIZ_HALL);
			msg("$ME所在的城市滅亡了，只好流浪至巫師大廳。\n",inv,0,1);
		}
		else
			destruct(inv);
	}

	// 終止分區天氣
	NATURE_D->deactivate_nature(cities[city][0][CROOM]);

	// 摧毀子城市物件
	destruct( cities[city][0][CROOM] );

	// 刪除子城市目錄
	rmtree(CITY_NUM(city, 0));

	// 刪除子城市所有資訊
	map_delete(cities[city], 0);

	log_file(LOG, "刪除 "+city+" 城市分區 "+0);
	CHANNEL_D->channel_broadcast("sys", "刪除 "+city+" 城市分區 "+0);


	// 修改所有市民資料
	foreach(id in cities[city][INFO]["citizens"])
	{
		if( objectp(citizen = find_player(id)) )
		{
			delete("city", citizen);
			citizen->save();
		}
		else if( objectp(citizen = load_user(id)) )
		{
			delete("city", citizen);
			citizen->save();
			destruct(citizen);
		}
	}

	log_file(LOG,"刪除 "+city+" 城市。\n");
	CHANNEL_D->channel_broadcast("news", HIG+query_city_idname(city)+HIG"在一陣天搖地動後沉入海底，永遠從世界地圖上消失。\n"NOR);

	// 清除城市資料
	map_delete(cities, city);

	// 移除子城市房地產資訊
	ESTATE_D->remove_zone_estate(city, 0);

	// 重置消費系統
	SHOPPING_D->refresh_shopping();

	// 刪除城市金融系統資訊
	MONEY_D->remove_finance_system(city);

	// 若有世界座標, 則刪除
	if( arrayp(worldmaploc) )
		MAP_D->delete_world_map(worldmaploc[0], worldmaploc[1]);

	// 刪除城市目錄
	rmtree(CITY_PATH(city));

	// 重新編排城市排序
	assign_cities_num();
}

/* 設定某一城市地圖上某一座標之其中一項變數值 */
mixed set_coor_data(array loc, mixed prop, mixed data)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(cities[city][num][DATA][y][x]) )
		return 0;

	if( prop == FLAGS )
	{
		if( !intp(data) ) return 0;
		return (cities[city][num][DATA][y][x][prop] |= data);
	}
	else
		return (cities[city][num][DATA][y][x][prop] = data);
}

/* 對一座標之變數值做數值加減處理 */
mixed add_coor_data(array loc, mixed prop, int data)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) || !mapp(cities[city][num][DATA][y][x]) )
		return 0;

	return (cities[city][num][DATA][y][x][prop] += data);
}

/* 刪除一座標之資料 */
varargs void delete_coor_data(array loc, mixed prop, int bit)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !mapp(cities[city][num][DATA][y][x]) ) return;

	// 若無 prop 則重新初始化房間資訊, 人口維持原狀
	if( !prop ) 
	{
		if(this_object()->query_coor_data(loc, TYPE) == BRIDGE)
		{ 	
			this_object()->set_coor_data(loc, TYPE, RIVER); 
			this_object()->set_coor_icon(loc, BBLU HIB"∼"NOR);
			this_object()->delete_coor_data(loc, "owner");
		}
		else
		{
			int origin_region = CITY_D->query_coor_data(loc, "region");
	
			cities[city][num][DATA][y][x] = DEFAULT_COOR_DATA;
	
			switch( origin_region  )
			{
			case AGRICULTURE_REGION:
				this_object()->set_coor_data(loc, 6, "100000");
				this_object()->set_coor_data(loc, "region", AGRICULTURE_REGION);
				break;
			case INDUSTRY_REGION:
				this_object()->set_coor_data(loc, 6, "200000");
				this_object()->set_coor_data(loc, "region", INDUSTRY_REGION);
				break;
			case COMMERCE_REGION:
				this_object()->set_coor_data(loc, 6, "300000");
				this_object()->set_coor_data(loc, "region", COMMERCE_REGION);
				break;
			}
		}
	}

	else if( prop == FLAGS )
	{
		if( !undefinedp(bit) )
			cities[city][num][DATA][y][x][prop] ^= bit;
		else
			map_delete(cities[city][num][DATA][y][x], prop);
	}
	else
	{
		map_delete(cities[city][num][DATA][y][x], prop);

		// 群組刪除功能
		foreach(mixed ori_prop, mixed data in cities[city][num][DATA][y][x])
		{
			if( stringp(ori_prop) && stringp(prop) && sscanf(ori_prop, prop+"/%*s") == 1 )
				map_delete(cities[city][num][DATA][y][x], ori_prop);
		}
	}
}


/* 傳回某一城市地圖上某一座標之其中一項變數值 */
varargs mixed query_coor_data(array loc, mixed prop)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( undefinedp(prop) )
		return cities[city][num][DATA][y][x];
	else
		return cities[city][num][DATA][y][x][prop];
}

array query_all_coor_data(string city, int num)
{
	if( !city_exist(city, num) )
		return 0;

	return cities[city][num][DATA];
}

/* 範圍性查尋, 傳回資料陣列 */
/* 圓形半徑資料群搜尋法, 搜尋時間與 radius 的平方呈正比 */
mapping query_coor_range(array loc, string prop, int radius)
{
	string city;
	int num, x, y;
	int i, j, xf, xt, yf, yt;
	mapping coor, data = allocate_mapping(0);

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	coor = cities[city][num][DATA];

	if( radius < 1 ) return ([ save_variable(loc):coor[y][x][prop] ]);

xf = x-radius; if( xf < 0 ) 	xf = 0;
xt = x+radius; if( xt > 99 ) 	xt = 99;
yf = y-radius; if( yf < 0 )	yf = 0;
yt = y+radius; if( yt > 99 )	yt = 99;

radius *= radius;

for(i=xf;i<=xt;i++)
	for(j=yf;j<=yt;j++)
		if( (i-x)*(i-x) + (j-y)*(j-y) <= radius )
			data[save_variable(arrange_city_location(i,j,city,num))] = coor[j][i][prop];
return data;
}
/* 設定座標地圖圖形 */
void set_coor_icon(array loc, string icon)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, city, num) )
		cities[city][num][MAP][y][x] = cities[city][num][RMAP][y][x] = icon;
}

/* 設定座標地圖圖形 */
void set_real_time_coor_icon(array loc, string icon)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) || noansi_strlen(icon) != 2 ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( valid_coordinate(x, y, city, num) )
		cities[city][num][RMAP][y][x] = icon;
}

/* 傳回座標地圖圖形 */
varargs string query_coor_icon(array loc, int raw)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	return !undefinedp(raw) ? remove_ansi(cities[city][num][MAP][y][x]) : cities[city][num][MAP][y][x];
}

/* 重設座標地圖圖形 */
void reset_coor_icon(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	cities[city][num][RMAP][y][x] = cities[city][num][MAP][y][x];
}

// 刪除座標地圖圖形
void delete_coor_icon(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	switch( CITY_D->query_coor_data(loc, "region")  )
	{
	case AGRICULTURE_REGION:
		cities[city][num][RMAP][y][x] = BGRN"．"NOR;
		cities[city][num][MAP][y][x] = BGRN"．"NOR;
		break;
	case INDUSTRY_REGION:
		cities[city][num][RMAP][y][x] = BYEL"．"NOR;
		cities[city][num][MAP][y][x] = BYEL"．"NOR;
		break;
	case COMMERCE_REGION:
		cities[city][num][RMAP][y][x] = BCYN"．"NOR;
		cities[city][num][MAP][y][x] = BCYN"．"NOR;
		break;
	default:
		cities[city][num][RMAP][y][x] = "．";
		cities[city][num][MAP][y][x] = "．";
		break;
	}
}

/* 設定城市基本資訊 */
mixed set_city_info(string city, string prop, mixed info)
{
	if( !stringp(city) || !stringp(prop) || undefinedp(info) || !city_exist(city) ) return 0;

	return cities[city][INFO][prop] = info;
}

/* 回傳城市基本資訊 */
varargs mixed query_city_info(string city, string prop)
{
	if( !stringp(city) || !city_exist(city) || !cities[city] || !cities[city][INFO] ) return 0;

	if( !undefinedp(prop) )
		return copy(cities[city][INFO][prop]);
	else
		return copy(cities[city][INFO]); 
}

/* 刪除城市基本資訊 */
int delete_city_info(string city, string prop)
{
	if( !stringp(city) || !stringp(prop) || !city_exist(city) ) return 0;

	map_delete(cities[city][INFO], prop);

	return 1;
}
/* 設定城市分區基本資訊 */
mixed set_section_info(string city, int num, string prop, mixed info)
{
	if( !stringp(city) || !stringp(prop) || undefinedp(info) || !city_exist(city, num) ) return 0;

	return cities[city][INFO]["section"][num][prop] = info;
}

/* 回傳城市分區基本資訊 */
varargs mixed query_section_info(string city, int num, string prop)
{
	if( !stringp(city) || !city_exist(city, num) ) return 0;

	if( !undefinedp(prop) )
		return copy(cities[city][INFO]["section"][num][prop]);
	else
		return copy(cities[city][INFO]["section"][num]);
}

/* 刪除城市分區基本資訊 */
int delete_section_info(string city, int num, string prop)
{
	if( !stringp(city) || !stringp(prop) || !city_exist(city, num) ) return 0;

	map_delete(cities[city][INFO]["section"][num], prop);

	return 1;
}


/* 改變城市資產 */
string change_assets(string city, mixed money)
{
	string new_assets;

	if( !stringp(city) || !money || !city_exist(city) ) return 0;

	if( !(money = to_int(big_number_check(money))) )
		return copy(query_city_info(city, "assets"));

	new_assets = count(query_city_info(city, "assets"), "+", money);

	if( count(new_assets, "<", 0) )
	{
		int crashtime = query_city_info(city, "crashtime");

		if( !crashtime )
			crashtime = set_city_info(city, "crashtime", time()+72*3600);

		crashtime -= time();

		if( count(money, ">", 0) )
			set_city_info(city, "tmpassets", new_assets);
		else if( count(count(new_assets, "-", query_city_info(city, "tmpassets")), "<", -10000000) )
		{
			if( crashtime > 0 )
				CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"市政府負債 $"+MONEY_D->city_to_money_unit(city)+" "+NUMBER_D->number_symbol(new_assets)+"，面臨財政危機，倒數 "+(crashtime/86400)+" 天 "+(crashtime%86400/60/60)+" 時 "+(crashtime%3600/60)+" 分城市崩潰毀滅。");
			else
				CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"市政府負債 $"+MONEY_D->city_to_money_unit(city)+" "+NUMBER_D->number_symbol(new_assets)+"，面臨財政危機，城市即將崩潰毀滅。");

			set_city_info(city, "tmpassets", new_assets);
		}
	}
	else if( query_city_info(city, "crashtime") )
	{
		delete_city_info(city, "crashtime");
		delete_city_info(city, "tmpassets");
		CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"市政府償還負債，解除財政危機。");
	}


	return set_city_info(city, "assets", new_assets);
}

string query_assets(string city)
{
	return query_city_info(city, "assets");
}

// 修改城市總名稱
void change_city_name(string city, string name)
{
	int len;

	if( !city_exist(city) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// 非純中文之 city name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(trim(name)+NOR);

	CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"正式更名為「"+name+"」");

	set_city_info(city, "name", name);
}

// 修改子城市名稱 
void change_section_name(string city, int num, string name)
{
	int len;
	string original_idname;

	if( !city_exist(city, num) || !stringp(name) ) return 0;

	len = noansi_strlen(name);
	if(  len > 10 || len % 2 ) return 0;

	// 非純中文之 city name	
	foreach( int a in remove_ansi(name) )
	if(  a <= ' ' || --len%2 && (a < 160 || a > 255) )
		return 0;

	name = kill_repeat_ansi(trim(name)+NOR);

	original_idname = query_city_idname(city, num);

	set_section_info(city, num, "name", name);

	CHANNEL_D->channel_broadcast("news", original_idname+"正式更名為「"+query_city_idname(city, num)+"」");
}		



/* 傳回所有城市名稱 */
string *query_cities()
{
	return filter_array(keys(cities), (: stringp($1) :));
}

int query_sizeof_cities()
{
	return sizeof(query_cities());
}

/* 傳回某城市中, 所有公民的 ID */
string *query_citizens(string city)
{
	if( !city_exist(city) ) return 0;

	return cities[city][INFO]["citizens"];
}

object query_maproom(array loc)
{
	string city = loc[CITY];
	int num = loc[NUM];

	if( !city_exist(city, num) ) return 0;

	return cities[city][num][CROOM];
}

object *query_all_maproom()
{
	object *all_maproom = allocate(0);

	foreach( string city, mapping m1 in cities )
	foreach( int num, mapping m2 in m1 )
	{
		if( intp(num) )
			all_maproom += ({ m2[CROOM] });
	}
	return all_maproom;
}

varargs array query_map(array loc, int realtime)
{
	string city = loc[CITY];
	int num = loc[NUM];

	if( !city_exist(city, num) ) return 0;

	return realtime ? cities[city][num][RMAP] : cities[city][num][MAP];
}

/* 傳回在某城市中的所有玩家 */
varargs object *query_city_players(string city, int num)
{
	if( undefinedp(num) )
	{
		if( city_exist(city) )
			return filter_array(users(), (: query_temp("in_city/id",$1) == $(city) :));

		return 0;
	}

	if( city_exist(city, num) )
		return filter_array(users(), (: query_temp("in_city/id",$1) == $(city) && query_temp("in_city/num",$1) == $(num) :));

	return 0;
}

/* 傳回在某城市中的所有戶外玩家 */
varargs object *query_outdoor_players(string city, int num)
{
	if( undefinedp(num) )
	{
		if( city_exist(city) )
		{
			object *outdoor_players = allocate(0);

			foreach( num in keys(cities[city]) )
			if( intp(num) )
				outdoor_players += filter_array(all_inventory(cities[city][num][CROOM]), (: userp($1) :));

			return outdoor_players;
		}
		return 0;
	}

	if( city_exist(city, num) )
		return filter_array(all_inventory(cities[city][num][CROOM]), (: userp($1) :));

	return 0;
}

mapping query_cities_info()
{
	return copy(cities);	
}

int query_sections(string city)
{
	return sizeof(filter_array(keys(cities[city]), (: intp($1) :)));
}

int is_citizen(string id, string city)
{
	if( !id || !city_exist(city) ) return 0;

	return member_array(id, cities[city][INFO]["citizens"]) != -1;
}

// 註冊市民身份
int register_citizen(string id, string city)
{
	if( !id || !city_exist(city) ) return 0;

	cities[city][INFO]["citizens"] |= ({ id });
	cities[city][INFO]["citizens_jointime"][id] = time();

	return 1;
}

// 移除市民身份
int remove_citizen(string id)
{
	foreach( string city, mapping info in cities )
	{
		if( !cities[city] || !cities[city][INFO] || !cities[city][INFO]["citizens"]
		    || member_array(id, cities[city][INFO]["citizens"]) == -1 )
			continue;

		cities[city][INFO]["citizens"] -= ({ id });
		map_delete(cities[city][INFO]["citizens_jointime"], id);

		if( cities[city][INFO]["government/mayor"] == id )
			cities[city][INFO]["government/mayor"] = 0;

		map_delete(cities[city][INFO]["government/officer"], id);
	
		for(int num=0;num<query_city_nums(city);num++)
			if( query_section_info(city, num, "vicemayor") == id )
				delete_section_info(city, num, "vicemayor");

		return 1;
	}

	return 0;
}

string query_player_city(string id)
{
	foreach( string city, mapping info in cities )
	{
		if( !mapp(info) || undefinedp(info[INFO]) ) continue;

		if( member_array(id, info[INFO]["citizens"]) != -1 )
			return city;
	}
	return 0;
}

mapping query_action(array loc)
{
	string city;
	int num, x, y;

	if( !arrayp(loc) ) return 0;

	city = loc[CITY];
	num = loc[NUM];
	y = loc[Y];
	x = loc[X];

	if( !city_exist(city, num) || !valid_coordinate(x, y, city, num) || undefinedp(cities[city][num][DATA])) return 0;

	switch( cities[city][num][DATA][y][x][TYPE] )
	{
	case PAVILION:
	case TREE:
	case GRASS:
		return ([
		    "sit":		([ 0:COMMAND_D->find_sub_command("city", "sit") ]),
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
int direction_check(int x, int y, string city, int num)
{
	int flag;
	mapping coor, data = cities[city][num][DATA];

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

// 回傳城市座標說明
string query_coor_help(array loc, string prop, int radius)
{
	string city = loc[CITY];
	int num = loc[NUM], y = loc[Y], x = loc[X];

	switch( cities[city][num][DATA][y][x][TYPE] )
	{
	case FARM:
	case PASTURE:
	case FISHFARM:
		return @GROWTH_HELP
    生產方式請參閱 help grow
GROWTH_HELP;

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
	string owner = CITY_D->query_coor_data(loc, "owner");
	string personal_short = CITY_D->query_coor_data(loc ,"short");
	
	if( owner )
	{
		if( belong_to_government(owner) )
			owner = query_city_name(owner[11..]);
		else if( belong_to_enterprise(owner) ) 
			owner = ENTERPRISE_D->query_enterprise_color_id(owner[11..]);
		else
			owner = capitalize(owner)+" ";
	}

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
	case 0:
		{
			if( owner )
			{
				return owner+"的"WHT"土地"NOR;
			}
			else
			{
				switch( CITY_D->query_coor_data(loc, "region") )
				{
				case AGRICULTURE_REGION:
					return GRN"農業規劃區"NOR;
				case INDUSTRY_REGION:
					return YEL"工業規劃區"NOR;
				case COMMERCE_REGION:
					return CYN"商業規劃區"NOR;
				default:
					return HIY"荒"NOR YEL"地"NOR;
				}
			}
			break;
		}
	case ROAD:
		return (CITY_D->query_coor_data(loc ,"roadname"))||(HIW"道"NOR WHT"路"NOR);
		break;
	case BRIDGE:
		return (CITY_D->query_coor_data(loc ,"bridgename"))||(HIR"橋"NOR RED"樑"NOR);
		break;
	case FARM:
		return personal_short||(owner + "的"HIG"農田"NOR);
		break;
	case PASTURE:
		return personal_short||(owner + "的"NOR YEL"牧場"NOR);
		break;
	case FISHFARM:
		return personal_short||(owner + "的"HIB"養殖場"NOR);
		break;
	case DOOR:
		{
			// 耗費效能過大先 mark 起來
			//mapping estdata = ESTATE_D->query_loc_estate(loc);
			//array building_info;

			//if( estdata )
			//	building_info = BUILDING_D->query_building(estdata["type"]);

			//if( !building_info )	
				return personal_short || (owner+"的建築物門口");
			//else
			//	return personal_short || (owner+"的"+building_info[0]);
			break;
		}

	case WALL:
		return HIM + owner + HIM"的建築工地";
		break;

	case RIVER:
		{
			switch( CITY_D->query_coor_data(loc, FLAGS) )
			{
			case FLOW_NORTH:
				return HBBLU"流向往北的河流"NOR;
				break;
			case FLOW_SOUTH:
				return HBBLU"流向往南的河流"NOR;
				break;
			case FLOW_EAST:
				return HBBLU"流向往東的河流"NOR;
				break;
			case FLOW_WEST:
				return HBBLU"流向往西的河流"NOR;
				break;
			default:
				return HBBLU"河流"NOR;
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
		return personal_short||(owner + NOR"的"HBYEL"路燈"NOR);
	case POOL:
		return personal_short||(owner + NOR"的"HBCYN"水池"NOR);
	case PAVILION:
		return personal_short||(owner + NOR"的"HBGRN"涼亭"NOR);
	case TREE:
		return personal_short||(owner + NOR"的"HBGRN"大樹"NOR);
	case GRASS:
		return personal_short||(owner + NOR"的"HBGRN"草地"NOR);
	case STATUE:
		return personal_short||(owner + NOR"的"HBWHT"雕像"NOR);
	default:
		return "未知的土地";
		break;
	}
}

mixed cookys_test()
{
	return cities;
}

// x_start , y_start 為可視範圍中 x,y 座標的起始值
// x_center 跟 y_center 是用來計算從哪開始人的位置不在中間可自動伸縮
// x 是橫座標， y 是垂直座標，city 是城市名，num 則是第幾個子市，不傳則是主市[0]
// Code 需要再整理
string show_map(array loc)
{
	object me = this_player();
	object* equipments;

	int i, j, k, l, dir, x, y, num, range,
	x_start, y_start, 
	x_center, y_center;

	string city, retmap, *emblem;
	string *column = allocate(CITY_VISION_LENGTH);
	array map2, map3;

	if( !loc ) return 0;

	x	=loc[X];
	y	=loc[Y];
	city	=loc[CITY];
	num	=loc[NUM];

	x_center = (CITY_VISION_WIDTH+1)/2;
	y_center = (CITY_VISION_LENGTH+1)/2;

	if( !valid_coordinate(x, y, city, num) ) return "錯誤的地圖區塊！";

	// 顯示區塊計算公式
	x_start = (x>=x_center)*(x+1-x_center) - ((x+x_center)>MAP_WIDTH)*(x_center+x-MAP_WIDTH);
	y_start = (y>=y_center)*(y+1-y_center) - ((y+y_center)>MAP_LENGTH)*(y_center+y-MAP_LENGTH);

	// 給 wiz 方便用
	if( wizardp(me) && query("env/no_weather", me) )
		range = CITY_VISION_WIDTH/3;

	// 裝備手電筒
	else if(sizeof(equipments = me->query_equipping_object(EQ_HAND[PART_ID])) && sizeof(filter_array(equipments, (: query("flashlight", $1) :))))
		range = CITY_VISION_WIDTH/3;

	else
	{
		range = CITY_VISION_WIDTH/3;
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
			case SNOWY:		range -= 2; break;
			case SHOWER:		range -= 1; break;
			case TYPHOON:		range -= 3; break;
			case FOEHN:		break;
		}
	}

	range*=range;

	// 圖上面的橫線
	retmap  = SAVEC;	// 儲存游標位置
	retmap += "\e[1;1H\e[2K";	// 游標移置畫面 1, 1
	retmap += NOR WHT"┌"+sprintf(HIG" %|3d "NOR+WHT"－"+HIG" %|3d "NOR+WHT+"─"HIG" %|2d "NOR WHT, x+1, y+1, num+1)+"────────────────┬─────┐";

	dir = direction_check(x, y, city, num);

	emblem = query_city_info(city, "emblem");

	column[0]	=NOR WHT"│"NOR+emblem[0]+NOR WHT"│";
	column[1]	=NOR WHT"│"NOR+emblem[1]+NOR WHT"│";
	column[2]	=NOR WHT"│"NOR+emblem[2]+NOR WHT"│";
	column[3]	=NOR WHT"│"HIW+TIME_D->gametime_digital_clock()+WHT"│";
	column[4]	=NOR WHT"│"HIW"天氣："+(NATURE_D->query_nature(MAP_D->query_maproom(loc)))[NATURE_WEATHER][WEATHER_CHINESE]+WHT+"│";
	column[5]	=NOR WHT"├─────┤";
	column[6] 	=NOR WHT"│  "+(dir&1?HIW"↖":NOR WHT"╲")	+(dir&2?HIW"↑":NOR WHT"│")	+(dir&4?HIW"↗":NOR WHT"╱")	+NOR WHT"  │";
	column[7]	=NOR WHT"│  "+(dir&8?HIW"←":NOR WHT"─")	+HIW+"○"+NOR WHT		+(dir&16?HIW"→":NOR WHT"─")	+NOR WHT"  │";
	column[8]	=NOR WHT"│  "+(dir&32?HIW"↙":NOR WHT"╱")	+(dir&64?HIW"↓":NOR WHT"│")	+(dir&128?HIW"↘":NOR WHT"╲")	+NOR WHT"  │";

	map2 = copy(cities[city][num][RMAP][y_start..y_start+CITY_VISION_LENGTH-1]);
	map3 = allocate(CITY_VISION_LENGTH, 0);

	if( range == (CITY_VISION_WIDTH/3)*(CITY_VISION_WIDTH/3) )
	{	
		map3 = allocate(CITY_VISION_LENGTH, allocate(CITY_VISION_WIDTH, 1));
		for(j=0;j<CITY_VISION_LENGTH;j++)
			map2[j] = map2[j][x_start..x_start+CITY_VISION_WIDTH-1];
	}
	else
	{
		for(j=0;j<CITY_VISION_LENGTH;j++)
			map3[j] = allocate(CITY_VISION_WIDTH);

		for(j=0;j<CITY_VISION_LENGTH;j++)
		{
			map2[j] = map2[j][x_start..x_start+CITY_VISION_WIDTH-1];

			for(i=0;i<CITY_VISION_WIDTH;i++)
			{
				if( map2[j][i] == HIY"Γ"NOR )
				{
					for(k=j-4;k<=j+4&&k<CITY_VISION_LENGTH;k++)
						for(l=i-4;l<=i+4&&l<CITY_VISION_WIDTH;l++)
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

	for(j=0;j<CITY_VISION_LENGTH;j++)
	{
		retmap += "\e["+(j+2)+";1H\e[2K"NOR WHT"│"NOR;	// 游標移至畫面 j+2, 1

		for(i=0;i<CITY_VISION_WIDTH;i++)
		{
			if( map3[j][i] )
			{
				if( j == y-y_start && i == x-x_start )
					retmap += ansi_part(map2[j][i])+HIC"╳"NOR;
				else
					retmap += map2[j][i];
			}

			else
				retmap += WHT"◆"NOR;
		}
		retmap += column[j]+"\n"NOR;
	}

	// 圖下的橫線
	retmap += "\e["+(j+2)+";1H\e[2K";	// 游標移至畫面 j+2, 1
	retmap += sprintf(NOR WHT"└["NOR"%|32s"NOR WHT"]───"NOR"%|10s"NOR WHT,
	    coor_short_name(loc),
	    CITY_D->query_section_info(city, num, "name") || (WHT"第"+CHINESE_D->chinese_number(num+1)+"都市")
	) + "┴─────┘";

	retmap += "\e["+(j+3)+";1H\e[2K";	// 游標移至畫面 j+3, 1
	retmap += NOR YEL"_________________________________________________________城區地圖_\e[u"NOR;
	retmap += REST; // 返回已儲存之游標位置

	return retmap+NOR;
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


varargs varargs int valid_move(object me, string direction, array loc)
{
	int x, y, num, *worldmaploc, maplink;

	mapping data;
	string city;
	array nextloc;

	if( !objectp(me) ) return 0;

	if( !loc )
		loc = query_temp("location", me);

	if( !arrayp(loc) )
		return 0;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	city = loc[CITY];
	num = loc[NUM];

	worldmaploc = MAP_D->query_world_map_location(city, num);

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
	nextloc = arrange_city_location(x, y, city, num);

	if( !valid_coordinate(x, y, city, num) )
		return 0;


	data = CITY_D->query_coor_data(nextloc);
		
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

varargs void move(object me, string direction, int automove)
{
	int x, y, num, cost_stamina, *worldmaploc, maplink;
	mapping data;
	string city, my_idname;
	array loc, nextloc;
	string random_thing = RANDOM_STRING[random(sizeof(RANDOM_STRING))];

	if( !objectp(me) || !arrayp(loc = query_temp("location",me)) ) return;

	x = copy(loc[X]);
	y = copy(loc[Y]);
	city = loc[CITY];
	num = loc[NUM];

	if( !city_exist(city) ) return;

	worldmaploc = MAP_D->query_world_map_location(city, num);

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

		if( sizeof(linked_map) == 2 )
		{
			nextloc = arrange_area_location(x, y, linked_map[0], linked_map[1]);

			broadcast(loc, my_idname+"朝"+CHINESE_D->to_chinese(direction)+"離開。\n", 0, me);
			me->move(nextloc);
			broadcast(nextloc, my_idname+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，自"+CHINESE_D->to_chinese(corresponding[direction])+"走了過來。\n", 0, me);
			
			me->follower_move(loc, nextloc);
			return;
		}
		return 0;
	}
	nextloc = arrange_city_location(x, y, city, num);

	data = CITY_D->query_coor_data(nextloc);

	delete_temp("go_resist", me);

	// 通往城市建築物內部
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
		else
		{
			msg("$ME推開了"+CHINESE_D->to_chinese(direction)+"的門進入了"+BUILDING_D->query_building(data[ROOM])[0]+"。\n",me, 0, 1);
			me->move(CITY_ROOM_MODULE(city, num, x, y, data[ROOM]));
			broadcast(CITY_ROOM_MODULE(city, num, x, y, data[ROOM]), my_idname+"推開了"+CHINESE_D->to_chinese(corresponding[direction])+"的門，"+weather_go[NATURE_D->query_nature(query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"，朝屋內走了進來。\n", 0, me);

			me->follower_move(loc, CITY_ROOM_MODULE(city, num, x, y, data[ROOM]));

			return;
		}
	}

	remove_call_out(query_temp("call_out_handle/flowing", me));

	// 檢查特殊土地形態的移動
	switch(data[TYPE])
	{
	case RIVER:
		if( !sizeof(me->query_equipping_object(EQ_BOAT[PART_ID])) )
		{
			cost_stamina = 100 - me->query_skill_level("sport")/2;
	
			switch( CITY_D->query_coor_data(nextloc, FLAGS) )
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

	if( is_city_location(nextloc) )
	{
		int coortype = query_coor_data(nextloc, TYPE);
		mapping fund = query_city_info(city, "fund");


		if( coortype == ROAD && fund["道路"] < 120 && !random((fund["道路"]+5)*(fund["道路"]+5)) )
		{
			msg("$ME突然一腳踩進年久失修的道路坑洞中，鼠蹊部狠狠地撞上路旁的"+HIY+random_thing+NOR+"。\n", me, 0, 1);
			me->faint();
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"突然一腳踩進年久失修的道路坑洞("+query_city_id(city, num)+" "+(nextloc[X]+1)+","+(nextloc[Y]+1)+")中，鼠蹊部狠狠地撞上路旁的"+HIY+random_thing+NOR+"。", me);
		}
		else if( coortype == BRIDGE && fund["橋樑"] < 120 && !random((fund["橋樑"]+5)*(fund["橋樑"]+5)) )
		{
			msg("$ME腳底下的橋樑突然斷裂，鼠蹊部狠狠地撞上橋墩上的"+HIY+random_thing+NOR+"。\n", me, 0, 1);
			me->faint();
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"腳底下的橋樑("+query_city_id(city, num)+" "+(nextloc[X]+1)+","+(nextloc[Y]+1)+")突然斷裂，鼠蹊部狠狠地撞上橋墩上的"+HIY+random_thing+NOR+"。", me);

		}
		else if( !coortype && !random(1000) )
		{
			msg("$ME走在高低不平的荒地上，突然間腳踝扭了一下，鼠蹊部狠狠地撞上旁邊的"+HIY+random_thing+NOR+"。\n", me, 0, 1);
			me->faint();
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"走在高低不平的荒地上，突然間腳踝扭了一下，鼠蹊部狠狠地撞上旁邊的"+HIY+random_thing+NOR+"。", me);
		}
	}

	me->follower_move(loc, nextloc);
}

void remove_living_icon(object me, array loc)
{
	int x, y, num;
	string city;
	object *others;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	city = loc[CITY];
	num = loc[NUM];

	if( !city_exist(city, num) || !valid_coordinate(x, y, city, num) || undefinedp(cities[city][num][RMAP]) || undefinedp(cities[city][num][MAP]) ) return;

	others = filter_array(present_objects(me)-({me}), (: userp($1) :));

	switch( sizeof(others) )
	{
	case 0:	cities[city][num][RMAP][y][x] = cities[city][num][MAP][y][x];break;
	case 1:	cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + (query("gender", others[0]) == MALE_GENDER ? HIG + "♂" : HIR + "♀") + NOR;break;
	default:cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + HIC"※"NOR;break;
	}
}

void set_living_icon(object me, array loc)
{
	int x, y, num;
	string city;

	if( !arrayp(loc) ) return;

	x = loc[X];
	y = loc[Y];
	city = loc[CITY];
	num = loc[NUM];

	if( !valid_coordinate(x, y, city, num) ) return;

	if( sizeof(filter_array(present_objects(me), (: userp($1) :))) > 1 )
		cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + HIC"※"NOR;
	else
		cities[city][num][RMAP][y][x] = ansi_part(cities[city][num][MAP][y][x]) + (query("gender", me) == MALE_GENDER ? HIG + "♂" : HIR + "♀") + NOR;
}


// 將地圖轉換為 HTML 格式
void translate_map_to_html(string city, int num)
{
	array arr;
	string map_graph = query_city_idname(city, num)+"\n";

	if( city_exist(city, num) )
	{
		foreach( arr in cities[city][num][RMAP] )
			map_graph += implode(arr, "")+"\n";

		write_file("/www/map/citymap_"+city+"_"+num+"_ansi", map_graph, 1);

		HTML_D->make_html(remove_ansi(map_graph), ([
			"bgcolor"	:"black",
			"filename"	:"/www/map/citymap_"+city+"_"+num+".html",
			"title"		:remove_ansi(MUD_FULL_NAME+" "+CITY_D->query_city_idname(city, num)),
			"refresh"	:300,
			"fontsize"	:12,
		    ]));
	}
}

int is_mayor(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) ) 
			return 0;
	}
	else
		return 0;

	return id == cities[city][INFO]["government/mayor"];
}

int is_officer(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) ) 
			return 0;
	}
	else
		return 0;

	return !undefinedp(cities[city][INFO]["government/officer"][id]);
}

int is_mayor_or_officer(string city, mixed ob)
{
	string id;

	if( !city_exist(city) ) return 0;

	if( stringp(ob) )
		id = ob;
	else if( objectp(ob) )
	{
		if( !stringp(id = ob->query_id(1)) ) 
			return 0;
	}
	else
		return 0;

	return id == cities[city][INFO]["government/mayor"] || !undefinedp(cities[city][INFO]["government/officer"][id]);
}

mapping query_city_fund(string city)
{
	int num;
	mapping fund, facilities, result;

	if( !city_exist(city) ) return 0;

	result = allocate_mapping(0);
	fund = query_city_info(city, "fund") || allocate_mapping(0);
	facilities = query_public_facility(city);

	num = query_city_nums(city);
	
	result["道路"] 		= fund["道路"] * facilities["道路"] * 20;
	result["橋樑"] 		= fund["橋樑"] * facilities["橋樑"] * 600;

	result["空地"] 		= facilities["空地"] * 20000;	
		
	result["公共建築物"]= facilities["公共建築物"] * 1000000;

	return result;
}

varargs string query_age_name(mixed city, int twoword)
{
	if( twoword )
	switch(intp(city) ? city : query_city_info(city, "age"))
	{
	case 0:
		return HIW"史"WHT"前"NOR;
		break;
	case 1:
		return HIG"農"NOR GRN"業"NOR;
		break;
	case 2:
		return HIY"工"NOR YEL"業"NOR;
		break;
	case 3:
		return HIC"知"NOR CYN"識"NOR;
		break;
	case 4:
		return HIB"宇"NOR BLU"宙"NOR;
		break;
	case 5:
                return HIR"未"NOR RED"來"NOR;
		break;
	default:
		return HIR"末"NOR RED"日"NOR;
		break;
	}
	else
	switch(intp(city) ? city : query_city_info(city, "age"))
	{
	case 0:
		return HIW"史前"NOR WHT"時代"NOR;
		break;
	case 1:
		return HIG"農業"NOR GRN"時代"NOR;
		break;
	case 2:
		return HIY"工業"NOR YEL"時代"NOR;
		break;
	case 3:
		return HIC"知識"NOR CYN"時代"NOR;
		break;
	case 4:
		return HIB"宇宙"NOR BLU"時代"NOR;
		break;
	case 5:
                return HIR"未來"NOR RED"時代"NOR;
		break;
	default:
		return HIR"世界"NOR RED"末日"NOR;
		break;
	}
}

// 城市升級檢查
void city_level_up(string city, int force_level_up)
{
	int age = query_city_info(city, "age");
	mapping level_up_resource = query_city_info(city, "level_up_resource");
	
	// 升級條件
	if( 
		force_level_up ||
		(
		level_up_resource["metal_curr"] >= level_up_resource["metal_need"] &&
		level_up_resource["stone_curr"] >= level_up_resource["stone_need"] &&
		level_up_resource["water_curr"] >= level_up_resource["water_need"] &&
		level_up_resource["wood_curr"] >= level_up_resource["wood_need"] &&
		level_up_resource["fuel_curr"] >= level_up_resource["fuel_need"]
		)
	)
	{
		set_city_info(city, "age", age+1);
	
		calculate_level_up_resource(city, age+1);
		
		CHANNEL_D->channel_broadcast("news", query_city_idname(city)+"市長驕傲的宣佈本城市的文明已演進為「"+query_age_name(city)+"」！！");
		HISTORY_D->add_history("city/"+city, query_city_idname(city)+"城市文明演進為「"+query_age_name(city)+"」。");
		
		// 儲存城市資料
		save_city(city);
	}
}

void build_level_up_resource_data()
{
	foreach( string city in query_cities() )
	{
		calculate_level_up_resource(city, query_city_info(city, "age"));
	}
}

void fund_handler()
{
	string fundname, city, citizen;
	mapping currentassets = allocate_mapping(0);
	int totalmoney, money;

	foreach( city in query_cities() )
		currentassets[city] = cities[city][INFO]["assets"];

	// 關閉系統心跳計算
	set_heartbeat_interval(0);

	// 處理徵稅資料
	TAX_D->game_month_process();

	foreach( city in query_cities() )
	{
		reset_eval_cost();

		totalmoney = 0;

		// 廢棄都市不計算經費
		if( cities[city][INFO]["fallen"] ) continue;

		// 一般經費計算
		foreach( fundname, money in query_city_fund(city) )
			totalmoney -= money;

		foreach( citizen in query_citizens(city) )
			totalmoney -= ESTATE_D->query_all_estate_value(citizen) / 15000;

		change_assets(city, totalmoney);

		cities[city][INFO]["last_total_gain"] = count(cities[city][INFO]["assets"], "-", currentassets[city]);
	}
	
	CHANNEL_D->channel_broadcast("news", "政府經費支出與稅收收入結算完畢");
	
	// 重新啟動系統心跳計算
	set_heartbeat_interval(1);
}

void start_fund_handler()
{
	CHANNEL_D->channel_broadcast("news", "開始進行本月政府經費支出與稅收收入結算");
	
	call_out( (: fund_handler :), 1);
}

void add_land_record(string city, int num, string id, string material, int amount)
{
	if( !city_exist(city, num) ) return;

	if( !mapp(cities[city][INFO]["section"][num]["material_land_record"]) )
		cities[city][INFO]["section"][num]["material_land_record"] = allocate_mapping(0);
		
	if( !mapp(cities[city][INFO]["section"][num]["material_land_record"][id]) )
		cities[city][INFO]["section"][num]["material_land_record"][id] = allocate_mapping(0);
		
	cities[city][INFO]["section"][num]["material_land_record"][id][material] += amount;
}

// 用於手動修改城市資料庫內容
/*
void change_data()
{
	foreach( string city, mapping map1 in cities )
	foreach( mixed num, mapping map2 in map1 )
	{
		if( !intp(num) ) continue;

		for(int y=0;y<100;y++)
		for(int x=0;x<100;x++)
		{

		}
	}
}
*/

string query_city_scale(string city, int num)
{
	int level;

	if( !city_exist(city, num) ) return 0;
		
	level = cities[city][INFO]["section"][num]["level"];

	return CITY_SCALE[level];
}

// 繁榮度與居民人口計算
varargs void rehash_flourish(string city, int spec_num)
{
	int i, level, flourish, num, cityuser;
	mapping data, estdata, cityestdata;
	string id;
	array myestates;
	int register_open;
	string *getdir;
	float flourish_bonus;

	if( !city_exist(city, num) ) return;

	cityuser = sizeof(filter_array(users(), (: objectp($1) && query("city", $1) == $(city) :)));

	if( undefinedp(spec_num) )
		cities[city][INFO]["totalflourish"] = 0;
	
	if( cities[city][INFO]["fallen"] ) return;

	// 檢查城市是否有開放註冊
	if( sizeof(objects((: query("function", $1) == "register" && $1->query_city() == $(city) && !BUILDING_D->is_building_room_locked($1) :))) )
		register_open = 1;

	cities[city][INFO]["register_open"] = register_open;

	for(num=0;city_exist(city, num);num++)
	{
		if( !undefinedp(spec_num) && num != spec_num ) 
			continue;

		flourish = 0;
		flourish_bonus = 1.;

		data = cities[city][num][DATA];

		cityestdata = ESTATE_D->query_estate(city, num);
		getdir = get_dir(CITY_NUM_ROOM(city, num));

		if( sizeof(cityestdata) )
		foreach(id, myestates in cityestdata)
		{
			if( belong_to_government(id) || belong_to_enterprise(id) || !objectp(find_player(id)) || query("prisoner", find_player(id)) )
				continue;

			foreach(estdata in myestates)
			{
				if( sizeof(estdata["roomtable"]) > 0 && BUILDING_D->is_building_locked(restore_variable(estdata["roomtable"][0]), estdata) )
					continue;

				flourish += ESTATE_D->query_estate_flourish(estdata, getdir);
			}
		}

		// 線上玩家貢獻繁榮度
		flourish += cityuser*300;

		// 隨機變動
		flourish += range_random(-100, 100);

		// 最低值
		if( flourish < 10 )
			flourish = random(10)+1;

		// 經費影響
		foreach(i in values(query_city_info(city, "fund")))
			flourish += i;

		if( cities[city][INFO]["section"][num][FLOURISH_BUFF] > time() )
			flourish_bonus += 0.2;
			
		if( cities[city][INFO][BATTLEFIELD_FLOURISH_BUFF] > time() )
			flourish_bonus += 0.2;
			
		if( has_building("prodigy", city, num) )
			flourish_bonus += 0.1;

		flourish = to_int(flourish * flourish_bonus);

		cities[city][INFO]["section"][num]["flourish"] = flourish;

		cities[city][INFO]["section"][num]["resident"] = to_int(pow(to_float(flourish)*1000, 0.8));
		
		if( undefinedp(spec_num) )
			cities[city][INFO]["totalflourish"] += flourish;

		level = cities[city][INFO]["section"][num]["level"];

		// 降級
		if( cities[city][INFO]["section"][num]["resident"] < level*300000 && level > 0 )
		{
			cities[city][INFO]["section"][num]["level"] = level-1;
			CHANNEL_D->channel_broadcast("city", query_city_idname(city, num)+"居民人口總數少於"HIW" "+NUMBER_D->number_symbol(level*300000)+" "NOR"人，規模降為「"+CITY_SCALE[level-1]+"」", city);	
		}
		// 升級
		else if( cities[city][INFO]["section"][num]["resident"] > (level+1)*300000 && level < 14 )
		{
			cities[city][INFO]["section"][num]["level"] = level+1;
			CHANNEL_D->channel_broadcast("city", query_city_idname(city, num)+"居民人口總數突破"HIW" "+NUMBER_D->number_symbol((level+1)*300000)+" "NOR"人，規模升為「"+CITY_SCALE[level+1]+"」", city);	
		}
		
		TOP_D->update_top("city", city+num, flourish, city, num);
	}
}


void rehash_all_flourish()
{
	foreach( string city in query_cities() )
	rehash_flourish(city);
}

mapping query_city_data(string city, int num)
{
	if( undefinedp(city) )
		return cities;
	else if( undefinedp(num) )
		return cities[city];
	else
		return cities[city][num];
}

// 聯結 data
void link_data()
{
	cities = CITYDATA_D->query_data();

	// 對所有城市進行編號排序
	assign_cities_num();
}

private void create()
{
	if( clonep() )
		destruct(this_object());

	link_data();
}

string query_name()
{
	return "城市系統(CITY_D)";
}
