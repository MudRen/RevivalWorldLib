/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : collect.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-19
 * Note   : collect 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define COLLECT_STR_COST	40

#define MATERIAL_PATH		"/obj/materials/"

#include <buff.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <material.h>
#include <location.h>
#include <citydata.h>

inherit COMMAND;

string help = @HELP

採集指令

這個指令可在地圖上採集各種原料，原料共分五種。

metal	金屬 - 於山區使用十字鎬類工具採集
stone	原石 - 於山區使用鑽頭類工具採集
water	清水 - 於河流使用水桶類工具採集
wood	原木 - 於森林使用木鋸類工具採集
fuel	燃料 - 於森林使用鏟子類工具採集

指令格式:
collect metal			- 採集一個金屬(採集身上必須有十字鎬類工具)
collect 10 metal		- 一次採集十個金屬
collect warehouse city N X,Y	- 設定資源採集倉庫地點(ex: collect warehouse wizard 1 50,50)
collect warehouse -d		- 取消資料倉庫設定
相關指令: info

HELP;

private int cost_stamina(object me, int amount, int skilllevel, int quality)
{
	int collect_buff = me->query_all_buff(BUFF_COLLECT_ABILITY);
	int stamina_cost = (amount - amount * collect_buff / 100)* (COLLECT_STR_COST - skilllevel/4);

	stamina_cost = to_int(stamina_cost * 100. / quality);

	return me->cost_stamina(stamina_cost);	
}

private void do_command(object me, string arg)
{
	array loc;
	object ob, env = environment(me);
	mapping coorrangetype;
	object *tools;
	int skilllevel, endurance, amount;
	string arg2;
	string area, city;
	int num;
	int quality;
	int x, y;
	string mycity = query("city", me);
	object warehouse;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要採集哪種資源？("HIW"金屬 metal"NOR"、"WHT"原石 stone"NOR"、"HIC"清水 water"NOR"、"YEL"原木 wood"NOR"、"HIM"燃料 fuel"NOR")\n");
	
	if( arg == "warehouse" )
	{
		if( !objectp(warehouse = load_module(query("collect_warehouse", me))) )
			return tell(me, pnoun(2, me)+"尚未設定資源倉庫。\n");
		else
			return tell(me, pnoun(2, me)+"目前的資源倉庫設定在"+warehouse->query_room_name()+"。\n");
	}
	else if( arg == "warehouse -d" )
	{
		if( !query("collect_warehouse", me) )
			return tell(me, pnoun(2, me)+"原本便未設定資源倉庫。\n");
			
		delete("collect_warehouse", me);
		
		return tell(me, pnoun(2, me)+"取消了資源倉庫的設定。\n");
	}
	else if( sscanf(arg, "warehouse '%s' %d %d,%d", city, num, x, y) == 4 || sscanf(arg, "warehouse %s %d %d,%d", city, num, x, y) == 4 )
	{
		string file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");
		
		if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
			return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並沒有倉庫。\n");

		if( query("owner", warehouse) != me->query_id(1) )
			return tell(me, warehouse->query_room_name()+"並不是"+pnoun(2, me)+"的倉庫。\n");

		if( warehouse != warehouse->query_master() )
		{
			file = base_name(warehouse->query_master());
			warehouse = warehouse->query_master();
		}
		
		set("collect_warehouse", file, me);
		
		msg("$ME設定"+warehouse->query_room_name()+"為資源採集的倉庫。\n", me, 0, 1);
		return;
	}

	if( !env || !env->is_maproom() )
		return tell(me, "無法在這裡採集資源。\n");
	
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}
	
	if( !mycity )
		return tell(me, pnoun(2, me)+"必須先成為某個城市的市民之後才可以開始採集資源。\n");

	loc = query_temp("location", me);
	
	if( (MAP_D->query_map_system(loc)) != AREA_D )
		return tell(me, "無法在城市內採集資源。\n");

	warehouse = load_module(query("collect_warehouse", me));
		
	if( objectp(warehouse) && query("owner", warehouse) != me->query_id(1) )
		return tell(me, warehouse->query_room_name()+"並不是"+pnoun(2, me)+"的倉庫。\n");
 
 	area = loc[AREA];
	num = loc[NUM];
	
	if( AREA_D->query_lead_city(area, num) != mycity )
		return tell(me, pnoun(2, me)+"的城市"+CITY_D->query_city_idname(mycity)+"尚未佔領這塊郊區或佔有數量不足 50000，"+pnoun(2, me)+"無法在這裡採集資源。\n");

 	if( me->is_fatigue_full() )
		return tell(me, pnoun(2, me)+"實在是太疲勞了，無法再採集資源。\n");

	coorrangetype = AREA_D->query_coor_range(loc, TYPE, 1);

	arg = lower_case(arg);

	if( sscanf(arg, "%d %s", amount, arg2) == 2 )
		arg = arg2;
	
	if( amount < 1 )
		amount = 1;
	
	if( amount > 1000 )
		return tell(me, "最多一次只能採集 1000 個資源。\n");

	switch(arg)
	{
		case "metal":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有山區，沒辦法開採金屬。\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採金屬的工具。\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME氣喘噓噓，已經沒力氣再開採金屬了...。\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採金屬了...。\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);

			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME舉起$YOU賣力地從礦石中鑿出了"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("metal") < 20 )
				me->add_skill_exp("metal", amount*(12+random(11)));
			else if( me->query_skill_level("metal") == 20 )
				tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

			me->add_social_exp(to_int(amount*(30+random(31))));

			if( !random(1000) )
				me->add_fatigue(1);

			break;	

		case "stone":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有山區，沒辦法開採石材。\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採石材的工具。\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME氣喘噓噓，已經沒力氣再開採石材了...。\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採石材了...。\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);

			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME舉起$YOU賣力地從岩層鑿出了"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("stone") < 20 )
				me->add_skill_exp("stone", amount*(12+random(11)));
			else if( me->query_skill_level("stone") == 20 )
				tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

			me->add_social_exp(to_int(amount*(30+random(31))));
			
			if( !random(1000) )
				me->add_fatigue(1);

			break;	

		case "water":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == POOL || $2 == RIVER :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有水源，沒辦法採集清水。\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來集水的工具。\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME氣喘噓噓，已經沒力氣再舀水了...。\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，底部裂了一個大洞，顯然是沒辦法再繼續用來裝水了...。\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}

				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);
			
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME提著$YOU用力地從河中舀起了"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("water") < 20 )
				me->add_skill_exp("water", amount*(12+random(11)));
			else if( me->query_skill_level("water") == 20 )
				tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

			me->add_social_exp(to_int(amount*(30+random(31))));
			
			if( !random(1000) )
				me->add_fatigue(1);

			break;
			
		case "wood":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有樹林，沒辦法伐木。\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來伐木的工具。\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME氣喘噓噓，已經沒力氣再伐木了...。\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來伐木了...。\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
	
			ob = new(MATERIAL_PATH+arg);
		
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME手握$YOU努力地從巨木上鋸下了"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("wood") < 20 )
				me->add_skill_exp("wood", amount*(12+random(11)));
			else if( me->query_skill_level("wood") == 20 )
				tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

			me->add_social_exp(to_int(amount*(30+random(31))));

			if( !random(1000) )
				me->add_fatigue(1);

			break;

		case "fuel":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "附近的區域中並沒有森林，沒辦法開採燃料。\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"身上沒有用來開採燃料的工具。\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME氣喘噓噓，已經沒力氣再開採燃料了...。\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// 工具損壞(損壞機率與工具耐久度和使用者技能相關)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME手上的"+tools[0]->query_idname()+"突然『啪』的一聲，手柄處裂了開來，顯然是沒辦法再繼續用來開採燃料了...。\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"從這個事件中獲得更多的社會經驗。\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);
			
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME舉起$YOU賣力地從土地裡挖出了"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME已經拿不動"+ob->short(1)+"了！只好先放在地上。\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("fuel") < 20 )
				me->add_skill_exp("fuel", amount*(12+random(11)));
			else if( me->query_skill_level("fuel") == 20 )
				tell(me, pnoun(2, me)+"的採集技能等級已經到達 20，無法再自我往上提升，更高深的學問必須開始到大學學習。\n");

			me->add_social_exp(amount*(30+random(31)));

			if( !random(1000) )
				me->add_fatigue(1);

			break;

		default:
			return tell(me, pnoun(2, me)+"想要收集哪種原料？("HIW"金屬 metal"NOR", "WHT"原石 stone"NOR", "HIC"清水 water"NOR", "YEL"原木 wood"NOR", "HIM"燃料 fuel"NOR")\n");
			break;
	}
	
	me->remove_status(query_temp("last_time_collect_status", me));
	me->add_status(set_temp("last_time_collect_status", HIM"採"NOR MAG"集"NOR, me), 30);
}
	
