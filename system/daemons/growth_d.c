/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : growth_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-04
 * Note   : 成長精靈 - 農田 / 牧場 / 養殖場
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <location.h>
#include <message.h>
#include <nature.h>
#include <nature_data.h>

int tick;
mapping growth;

varargs void set_status(array loc, mapping data)
{
	object ob;
	string status="";
	int growth_level = CITY_D->query_coor_data(loc, "growth_level");
	int maintain_time = CITY_D->query_coor_data(loc, "maintain_time");

	if( undefinedp(data) )
	{
		switch(CITY_D->query_coor_data(loc, TYPE))
		{
			case FARM:
				status =
					HIG"  種植"NOR GRN"作物："NOR+"無\n"
					HIG"  土地"NOR GRN"等級："HIG+repeat_string("*", growth_level)+NOR GRN"/"+maintain_time+"\n"NOR;
				break;
			case PASTURE:
				status =
					HIY"  畜養"NOR YEL"動物："NOR+"無\n"
					HIY"  土地"NOR YEL"等級："HIY+repeat_string("*", growth_level)+NOR YEL"/"+maintain_time+"\n"NOR;
				break;
			case FISHFARM:
				status =
					HIB"  養殖"NOR BLU"水產："NOR+"無\n"
					HIB"  土地"NOR BLU"等級："HIB+repeat_string("*", growth_level)+NOR BLU"/"+maintain_time+"\n"NOR;
				break;
		}
		status += "\n";
		CITY_D->set_coor_data(loc, "status", status);
		return;
	}

	switch(CITY_D->query_coor_data(loc, TYPE))
	{
		case FARM:
			status =
				HIG"  種植"NOR GRN"作物："NOR+data["idname"]+"\n"
				HIG"  種植"NOR GRN"數量："NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIG"  收成"NOR GRN"比例："NOR+data["harvest_percent"]+" %\n"
				HIG"  土地"NOR GRN"等級："HIG+repeat_string("*", growth_level)+NOR GRN"/"+maintain_time+"\n"NOR
				HIG"  種植"NOR GRN"時間："NOR+(data["tick"]/60)+" 分 "+(data["tick"]%60)+" 秒 "+(data["wait_for_harvesting"] ? HIG"(已可收成)"NOR : "" )+"\n"
				HIG"  種植"NOR GRN"狀態："NOR+(data["current_growing"]||"尚未灌溉完成\n");
			
			if( data["material"] )
				status += HIG"  需要"NOR GRN"材料："NOR;
			break;
		case PASTURE:
			status =
				HIY"  畜養"NOR YEL"動物："NOR+data["idname"]+"\n"
				HIY"  畜養"NOR YEL"數量："NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIY"  收成"NOR YEL"比例："NOR+data["harvest_percent"]+" %\n"
				HIY"  土地"NOR YEL"等級："HIY+repeat_string("*", growth_level)+NOR YEL"/"+maintain_time+"\n"NOR
				HIY"  畜養"NOR YEL"時間："NOR+(data["tick"]/60)+" 分 "+(data["tick"]%60)+" 秒 "+(data["wait_for_harvesting"] ? HIY"(已可收成)"NOR : "" )+"\n"
				HIY"  畜養"NOR YEL"狀態："NOR+(data["current_growing"]||"尚未餵食完成\n");
			
			if( data["material"] )
				status += HIY"  需要"NOR YEL"材料："NOR;
			break;
		case FISHFARM:
			status =
				HIB"  養殖"NOR BLU"水產："NOR+data["idname"]+"\n"
				HIB"  養殖"NOR BLU"數量："NOR+data["input_amount"]+"/"+(10+growth_level)+"\n"
				HIB"  收成"NOR BLU"比例："NOR+data["harvest_percent"]+" %\n"
				HIB"  土地"NOR BLU"等級："HIB+repeat_string("*", growth_level)+NOR BLU"/"+maintain_time+"\n"NOR
				HIB"  養殖"NOR BLU"時間："NOR+(data["tick"]/60)+" 分 "+(data["tick"]%60)+" 秒 "+(data["wait_for_harvesting"] ? HIB"(已可收成)"NOR : "" )+"\n"
				HIB"  養殖"NOR BLU"狀態："NOR+(data["current_growing"]||"尚未餵食完成\n");

			if( data["material"] )
				status += HIB"  需要"NOR BLU"材料："NOR;
			break;
	}	

	if( mapp(data["material"]) )
	{
		foreach(string file, int num in data["material"] )
		{
			ob = load_object(file);
			
			if( !objectp(ob) ) continue;
			
			status += num + " "+(query("unit", ob)||"單位") + ob->query_idname()+", ";
		}
		
		status = status[0..<3]+"\n";
	}
		
	status += "\n";
	CITY_D->set_coor_data(loc, "status", status);
}

void grow_up(string sloc)
{
	int localtick;
	string weather;
	array loc = restore_variable(sloc);
	mapping coor_data;
	mapping data;
	
	if( !CITY_D->city_exist(loc[CITY]) )
		return;

	coor_data = CITY_D->query_coor_data(loc);
	data = coor_data["growth"];

	switch(coor_data[TYPE])
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			break;
		default:
			this_object()->remove_growth(loc);
			return;
			break;
	}

	if( !mapp(data) || !data["started"] || data["wait_for_harvesting"] ) return;

	weather = NATURE_D->query_nature(MAP_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID];

	switch( weather )
	{
		case SHOWER:
		case FOEHN:
		case SNOWY:
		case TYPHOON:
			if( random(4) ) break;
			
			if( random(10) && member_array(STATUE, values(CITY_D->query_coor_range(loc, TYPE, 3))) != -1 )
			{
				switch(weather)
				{
					case SHOWER:
						broadcast(loc, "附近莊嚴肅穆的雕像發出隱隱"HIG"綠"NOR GRN"光"NOR"，天上降下的滂沱大雨似乎減小了許多。\n");
						break;
					case FOEHN:
						broadcast(loc, "附近莊嚴肅穆的雕像發出隱隱"HIG"綠"NOR GRN"光"NOR"，剎時焚風似乎減弱了許多。\n");
						break;
					case SNOWY:
						broadcast(loc, "附近莊嚴肅穆的雕像發出隱隱"HIG"綠"NOR GRN"光"NOR"，積雪似乎慢慢地融化了。\n");
						break;
					case TYPHOON:
						broadcast(loc, "附近莊嚴肅穆的雕像發出隱隱"HIG"綠"NOR GRN"光"NOR"，風勢似乎逐漸變弱了。\n");
						break;
				}
			}
			else
			{
				broadcast(loc, HIR+data["weather"][weather]+NOR);
				data["harvest_percent"] -= random(5);
				
				if( data["harvest_percent"] < 0 )
					data["harvest_percent"] = 0;
			}	
				

			break;
	}
			
	data["tick"] += 5;
	localtick = data["tick"];

	if( data["growing"][localtick] )
	{
		// 廣播成長訊息
		broadcast(loc, data["growing"][localtick]);
		data["current_growing"] = copy(data["growing"][localtick]);
		map_delete(data["growing"], localtick);
				
		// 成長完畢
		if( !sizeof(data["growing"]) )
			data["wait_for_harvesting"] = 1;
	}
	
	set_status(loc, data);
	CITY_D->set_coor_data(loc, "growth", data);
}

void heart_beat()
{
	string sloc;

	++tick;
	
	// 分散處理 - 農田
	if( !((tick+1)%5) )
	{
		foreach(sloc in growth[FARM])
			grow_up(sloc);
	}
	// 分散處理 - 牧場
	else if( !((tick+3)%5) )
	{	
		foreach(sloc in growth[PASTURE])
			grow_up(sloc);
	}	
	// 分散處理 - 養殖場	
	else if( !((tick+4)%5) )
	{
		foreach(sloc in growth[FISHFARM])
			grow_up(sloc);
	}
}

void add_growth(array loc)
{
	string sloc;
	int type = CITY_D->query_coor_data(loc, TYPE);

	switch(type)
	{
		case FARM:
		case PASTURE:
		case FISHFARM:
			sloc = save_variable(loc);
			growth[type] |= ({ sloc });
			break;
		default:
			break;
	}
}

void remove_growth(array loc)
{
	string sloc = save_variable(loc);

	foreach(int type, string *slocs in growth)
		growth[type] -= ({ sloc });

	CITY_D->delete_coor_data(loc, "growth");
}

void refresh_growth()
{
	int num, x, y, type;
	mapping coordata;
	string sloc;

	growth = allocate_mapping(0);	
	growth[FARM] = allocate(0);
	growth[PASTURE] = allocate(0);
	growth[FISHFARM] = allocate(0);
	
	// 重新載入資料
	foreach(string city in CITY_D->query_cities())
	{
		for(num=0;CITY_D->city_exist(city, num);++num)
		{
			coordata = CITY_D->query_all_coor_data(city, num);

			for(y=0;y<100;y++)
			for(x=0;x<100;x++)
			{
				type = coordata[y][x][TYPE];
				
				switch(type)
				{
					case FARM:
					case PASTURE:
					case FISHFARM:
						sloc = save_variable(arrange_city_location(x, y, city, num));
						growth[type] |= ({ sloc });
						break;
					default:
						break;
				}
			}
		}
	}
}

void create()
{
	refresh_growth();

	set_heart_beat(1);
}

mapping query_growth_data()
{
	return growth;
}

string query_name()
{
	return "成長系統(GROWTH_D)";
}
