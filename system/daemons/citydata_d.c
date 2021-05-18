/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : citydata_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : 城市精靈主體, 處理所有城市資料庫
 * Update :
 *  o 2002-09-17 Clode 重新設計城市資料結構與儲存資料程序
 *  o 2002-09-17 Clode 修改部份 code 提高容錯能力
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <citydata.h>

private mapping cities;

mapping query_data()
{
	return cities;
}

/* 重置所有資料 */
void restore_all_data()
{
	string city, roomfile;
	mixed num;

	cities = allocate_mapping(0);

	/* 初始化讀取所有城市資訊 */
	foreach( city in get_dir(CITY_ROOT) )
	{
		if( file_size(CITY_PATH(city)) != -2 ) continue;

		if(!read_file(CITY_INFO(city))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s 經讀取後字串值為 0 ... 略過。", CITY_INFO(city)));
			continue;
		}

		cities[city] = allocate_mapping(0);

		if(!(cities[city][INFO] = restore_variable(read_file(CITY_INFO(city))))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s 所存資料以 restore_variable 讀出結果為 0 ... 略過。", CITY_INFO(city)));
		}
	
		
		// 讀取所有子城市資料
		foreach( num in get_dir(CITY_PATH(city)) )
		{
			if( !sscanf(num, "%d", num) ) continue;

			CHANNEL_D->channel_broadcast("sys", "城市：初始化城市 "+city+" 之第 "+num+" 分區地圖資料庫");

			cities[city][num] 	= allocate_mapping(0);

			if(!read_file(CITY_NUM_DATA(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "DATA 經讀取後字串值為 0 ... 略過。");
			}
			else if(!(cities[city][num][DATA] = restore_variable(read_file(CITY_NUM_DATA(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "DATA 所存資料以 restore_variable 讀出結果為 0 ... 略過。");
			}

			if(!read_file(CITY_NUM_MAP(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "MAP 經讀取後字串值為 0 ... 略過。");
			}
			else if(!(cities[city][num][MAP] = restore_variable(read_file(CITY_NUM_MAP(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "MAP 所存資料以 restore_variable 讀出結果為 0 ... 略過。");
			}
			else
				cities[city][num][RMAP] = copy(cities[city][num][MAP]);

			cities[city][num][CROOM] = load_cityroom(city, num);

			// 載入所有 Module Room
			foreach( roomfile in get_dir(CITY_NUM_ROOM(city, num)) )
			{
				reset_eval_cost();

				catch(load_object(CITY_NUM_ROOM(city, num)+roomfile[0..<3]));
			}
		}
	}

	CITY_D->link_data();
}

int remove()
{
	return CITY_D->save_all();
}

void create()
{
	if( clonep() )
		destruct(this_object());

	restore_all_data();
}

string query_name()
{
	return "城市資料庫(CITYDATA_D)";
}
