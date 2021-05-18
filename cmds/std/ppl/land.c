/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : land.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-08
 * Note   : land 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <location.h>

inherit COMMAND;

#define LEVEL_UP_RESOURCE_COST		1

string help = @HELP
1. 土地精華吸納，可用於轉移土地等級
2. 城市文明升級

land			- 查詢目前已經吸納的土地精華
land absorb		- 吸納土地精華(需要特殊物品)
land release '數字'	- 將土地精華列表中第 N 個吸收的土地精華釋放到目前所在的土地
land '數量' '資源名稱'	- 將自然資源鋪設在城市土地上，提升城市文明等級
land info		- 查詢城市升級貢獻度

HELP;

varargs void query_resource_amount(mapping ref result, object ob, float percent, int need)
{
	object temp_ob;
	mapping product_info = ob->query_product_info();
	
	if( !mapp(product_info) ) return;

	if( !percent )
		percent = 1.1;
	
	if( !need )
		need = 1;
	


	foreach(string material, int count in product_info["material"])
	{
		if( objectp(temp_ob = load_object("/std/module/product/"+material+"_module") ) )
			query_resource_amount(ref result, temp_ob, percent*1.1, count * need);

		else switch(material)
		{
			case "/obj/materials/metal":	result["metal"] += to_int(need * count * percent);	break;
			case "/obj/materials/stone":	result["stone"] += to_int(need * count * percent);	break;
			case "/obj/materials/water":	result["water"] += to_int(need * count * percent);	break;
			case "/obj/materials/wood":	result["wood"] += to_int(need * count * percent);	break;	
			case "/obj/materials/fuel":	result["fuel"] += to_int(need * count * percent);	break;
		}
	}
}

private void do_command(object me, string arg)
{
	int i;
	string city;
	int num;

	if( !arg )
	{
		string list = "";
		array landdata = query("landdata", me);
		
		if( arrayp(landdata) && sizeof(landdata) )
		{
			list += HIY"土地"NOR YEL"精華"NOR"\n";
			foreach(array elem in landdata)
			{
				i++;
				switch(elem[0])
				{
					case FARM: 
						list += sprintf("%-3d%-20s "NOR GRN"土地等級 "HIG"%s"NOR GRN"/%d\n"NOR, i, HIG"農田"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case PASTURE: 
						list += sprintf("%-3d%-20s "NOR YEL"土地等級 "HIY"%s"NOR YEL"/%d\n"NOR, i, NOR YEL"牧場"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case FISHFARM: 
						list += sprintf("%-3d%-20s "NOR BLU"土地等級 "HIB"%s"NOR BLU"/%d\n"NOR, i, HIB"養殖場"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
				}
			}
			return tell(me, list+"\n");
		}
		else
			return tell(me, "目前沒有任何土地吸納精華。\n");
	}
	else if( (wizardp(me) && sscanf(arg, "info %s", city) == 1) || arg == "info" )
	{
		string str;
		string id;
		mapping land_record;
		mapping total_record = allocate_mapping(0);
		mapping data;

		if( !city )
			city = query("city", me);
		
		if( !city )
			return tell(me, pnoun(2, me)+"不屬於任何一座城市。\n");

		if( !CITY_D->city_exist(city) )
			return tell(me, "沒有 "+city+" 這座城市。\n");
		
		str = CITY_D->query_city_idname(city)+"資源貢獻數量如下：\n";
		str += WHT"─────────────────────────────────────\n\n"NOR;
		
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			land_record = copy(CITY_D->query_section_info(city, num, "material_land_record"));
			
			if( sizeof(land_record) )
			foreach(id, data in land_record)
			{
				if( undefinedp(total_record[id]) )
					total_record[id] = allocate_mapping(0);

				total_record[id]["metal"] += data["metal"];
				total_record[id]["stone"] += data["stone"];
				total_record[id]["water"] += data["water"];
				total_record[id]["wood"] += data["wood"];
				total_record[id]["fuel"] += data["fuel"];
			}
		}
		
		foreach(id, mapping data2 in total_record)
		{
			str += sprintf("  %-15s%-10s%-10s%-10s%-10s%-10s\n",
				capitalize(id),
				HIW+data2["metal"]+NOR,
				WHT+data2["stone"]+NOR,
				HIC+data2["water"]+NOR,
				YEL+data2["wood"]+NOR,
				HIM+data2["fuel"]+NOR);
		}
		
		str += WHT"\n─────────────────────────────────────\n"NOR;
		str += HIW"金屬 metal"NOR"、"WHT"原石 stone"NOR"、"HIC"清水 water"NOR"、"YEL"原木 wood"NOR"、"HIM"燃料 fuel"NOR"\n\n";
		return me->more(str);
	}
	else if( arg == "absorb" )
	{
		int type;
		int growth_level;
		int maintain_time;
		
		object media;
		object env = environment(me);
		array loc = query_temp("location", me);
		array landdata = query("landdata", me) || allocate(0);
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "無法吸納這塊土地的精華。\n");
		
		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "這塊土地並不是"+pnoun(2, me)+"的。\n");
			
		foreach(object ob in all_inventory(me))
		{
			if( query("land_absorb", ob) )	
			{
				media = ob;
				break;
			}
		}
		
		if( !objectp(media) )
			return tell(me, pnoun(2, me)+"身上缺乏吸納土地精華的媒介物品。\n");
		
		type = CITY_D->query_coor_data(loc, TYPE);
		growth_level = CITY_D->query_coor_data(loc, "growth_level");
		maintain_time = CITY_D->query_coor_data(loc, "maintain_time");
	
		if( type != FARM && type != FISHFARM && type != PASTURE )
			return tell(me, "無法吸納這種土地的精華。\n");
	
		if( maintain_time < 1 )
			return tell(me, "這塊土地沒有任何精華可以吸收。\n");

		msg("$ME將$YOU放置在土地正中央，只見$YOU發出刺眼的"HIW"白光"NOR"將這塊土地的精華完全吸納起來。\n", me, media, 1);
		msg("隨著白光逐漸散去，$YOU也跟著消失了。\n", me, media, 1);
		destruct(media, 1);

		CITY_D->delete_coor_data(loc, "growth_level");
		CITY_D->delete_coor_data(loc, "maintain_time");
		CITY_D->delete_coor_data(loc, "growth");
		CITY_D->delete_coor_data(loc, "status");
		TOP_D->delete_top("maintain", save_variable(loc));

		landdata += ({ ({ type, growth_level, maintain_time }) });
		set("landdata", landdata, me);
		me->save();
		CITY_D->save_city(loc[CITY], loc[NUM]);
	}
	else if( sscanf(arg, "release %d", i) == 1 )
	{
		object env = environment(me);
		array landdata = query("landdata", me);
		array loc = query_temp("location", me);
		array elem;
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "無法將土地精華釋放到這塊土地上。\n");

		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "這塊土地並不是"+pnoun(2, me)+"的。\n");
			
		if( !arrayp(landdata) )
			return tell(me, "目前沒有任何的土地精華。\n");

		if( i < 1 || i > sizeof(landdata) )
			return tell(me, "請輸入正確的數字。\n");

		elem = landdata[i-1];
		
		if( CITY_D->query_coor_data(loc, TYPE) != elem[0] )
			return tell(me, "無法將這種類型的土地精華釋放到這塊土地上。\n");
			
		if( CITY_D->query_coor_data(loc, "maintain_time") )
			return tell(me, "這塊土地已經有現存的土地精華，無法再將土地精華釋放到這塊土地上。\n");

		CITY_D->set_coor_data(loc, "growth_level", elem[1]);
		CITY_D->set_coor_data(loc, "maintain_time", elem[2]);
		GROWTH_D->set_status(loc);
		
		msg("$ME將大量的土地精華釋放到這塊土地上，只見土地上泛起一陣"HIW"白晢光芒"NOR"。\n", me, 0, 1);
		TOP_D->update_top("maintain", save_variable(loc), elem[1], me->query_idname(), elem[0]);

		landdata = landdata[0..i-2] + landdata[i..<1];
		
		if( !sizeof(landdata) )
			delete("landdata", me);
		else
			set("landdata", landdata, me);
		
		me->save();
	}
	else if( sscanf(arg, "%d %s", i, arg) == 2 )
	{
		object ob = present(arg);
		object env = environment(me);
		array loc = query_temp("location", me);
		mapping level_up_resource;
		mapping ob_resources;
		string str = "";
		int age, last_resource_needed;

		if( !objectp(ob) )
			return tell(me, "沒有 "+arg+" 這個物品。\n");

		//忙碌中不能下指令
		if( me->is_delaying() )
		{
			tell(me, me->query_delay_msg());
			return me->show_prompt();
		}

		if( i <= 0 || i > 10000 )
			return tell(me, "請輸入正確的數量(1~10000)。\n");

		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留狀態。\n");

		ob_resources = query("material", ob) || allocate_mapping(0);

		query_resource_amount(ref ob_resources, ob);

		if( !sizeof(ob_resources) )
			return tell(me, ob->query_idname()+"無法用來進行城市文明升級。\n");

		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "無法將資源鋪設在這塊土地上。\n");

		if( i > ob->query_amount() )
			return tell(me, "這裡沒有這麼多的"+ob->query_idname()+"。\n");

		if( CITY_D->query_city_info(loc[CITY], "age") >= 3 )
			return tell(me, "目前遊戲只開放到「"+CITY_D->query_age_name(3)+"」。\n");

		if( !me->cost_stamina(LEVEL_UP_RESOURCE_COST*i) )
			return tell(me, pnoun(2, me)+"的體力不夠了。\n");

		level_up_resource = copy(CITY_D->query_city_info(loc[CITY], "level_up_resource"));
		
		str = "$ME耗費 "+(LEVEL_UP_RESOURCE_COST*i)+" 點體力將"+QUANTITY_D->obj(ob, i)+"嵌到城市的地表下("HIG"完成度 "NOR GRN;

		age = CITY_D->query_city_info(loc[CITY], "age");

		if( age > 0 )
			last_resource_needed = (25000000 + 20000000*(CITY_D->query_city_nums(loc[CITY])-1))*to_int(pow(age-1, 2));
		else
			last_resource_needed = 0;

		foreach(string resource_name, int count in ob_resources)
		{
			if( level_up_resource[resource_name+"_curr"] + count * i > level_up_resource[resource_name+"_need"] )
			{
				tell(me, capitalize(resource_name) +" 資源數量無法再往上提升了。\n");
				level_up_resource[resource_name+"_curr"] = level_up_resource[resource_name+"_need"];
			}
			else
				level_up_resource[resource_name+"_curr"] += count * i;
			
			str += capitalize(resource_name) +" "+sprintf("%.5f%%", (level_up_resource[resource_name+"_curr"]-last_resource_needed)*100./(level_up_resource[resource_name+"_need"]-last_resource_needed))+" ";

			// 紀錄數量資訊
			CITY_D->add_land_record(loc[CITY], loc[NUM], me->query_id(1), resource_name, count * i);
		}

		str += NOR")。\n";
		
		msg(str, me, 0, 1);
		
		destruct(ob, i);

		CITY_D->set_city_info(loc[CITY], "level_up_resource", level_up_resource);
		CITY_D->city_level_up(loc[CITY]);
	}

	else return tell(me, "請輸入正確的指令參數(help land)。\n");
}
