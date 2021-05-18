/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shopping2_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-12
 * Note   : 購物精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <citydata.h>
#include <object.h>
#include <map.h>
#include <npc.h>
#include <citycondition.h>

mapping shopping;

#define DATA_PATH					"/data/daemon/shopping.o"
#define MONEY_PER_RESIDENT			500	// 每位居民提供 500 元的消費
#define DEMAND_DECAY_RATE			50

int refresh_time;
int time_tick;
string popproduct;
int worldmoney;
int lastworldmoney;

mapping query_shopping()
{
	return shopping;
}

void pop_product_choose()
{
	string city;
	int num;
	string product;
	object ob;
	string *all_products = allocate(0);

	foreach(city in CITY_D->query_cities() )
	{
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			foreach(product in shopping[city][num]["demand_rank"])
				all_products |= ({ product });
		}
	}

	if( sizeof(all_products) )
	for(int i=0;i<100;i++)
	{
		popproduct = all_products[random(sizeof(all_products))];
		
		if( objectp(ob = load_object(popproduct)) )
		{
			if( query("badsell", ob) )
				continue;

			CHANNEL_D->channel_broadcast("news", NOR GRN"流行特報：「"+ob->query_idname()+NOR GRN"」在消費市場上引發搶購熱潮。"NOR);
			break;
		}
		else if( objectp(ob = load_object("/std/module/product/"+popproduct+"_module.c")) )
		{
			mapping data = ob->query_product_info();
			CHANNEL_D->channel_broadcast("news", NOR GRN"流行特報：「"+data["name"]+"("+data["id"]+")"+NOR GRN"類工業產品」在消費市場上引發搶購熱潮。"NOR);
			break;
		}
	}
}

string set_pop_product(string arg)
{
	popproduct = arg;
}

string query_pop_product()
{
	return popproduct;
}

mixed query_shopping_info(string city, int num, string prop)
{
	if( !shopping[city] || !shopping[city][num] )
		return 0;

	return shopping[city][num][prop];
}

int shopping_cost_ratio(string city, int num)
{
	return shopping[city][num]["ratio"];
}
	
void shopping_cost_money(string city, int num, int cost)
{
	if( worldmoney > cost )
		worldmoney -= cost;
	else
		shopping[city][num]["money"] -= cost;
	
	shopping[city][num]["costmoney"] += cost;

	if( shopping[city][num]["moneypersec"] == 0 )
		return;
}

void count_demand(string city, int num, object product, int cost)
{
	if( product->is_module_product() )
		shopping[city][num]["demand"][product->query_module()] += cost;
	else
		shopping[city][num]["demand"][base_name(product)] += cost;
}

// 由 0 排至 100 ( 0 代表目前銷售量最低 -> 需求最高 )
varargs mixed query_demand_rank(string city, int num, object product)
{
	int rank;
	string *demand_rank = shopping[city][num]["demand_rank"];

	if( undefinedp(product) )
	{
		return demand_rank;
	}
	else if( product->is_module_product() )
	{
		if( product->query_module() == popproduct )
			return 0;

		rank = member_array(product->query_module(), demand_rank);
	}
	else
	{
		if( base_name(product) == popproduct )
			return 0;
	
		rank = member_array(base_name(product), demand_rank);
	}
	
	if( rank < 0 || sizeof(demand_rank) <= 1 )
		return 0;

	return rank * 100 / (sizeof(demand_rank)-1);
}

int save()
{
	return save_object(DATA_PATH);	
}

int remove()
{
	return save();
}

void refresh_shopping()
{
	int num;
	int money;
	string city;
	string product;
	string *delete_products;
	float buff_bonus;

	refresh_time = time();
	worldmoney = 0;

	foreach(city in CITY_D->query_cities() )
	{
		if( !mapp(shopping[city]) )
			shopping[city] = allocate_mapping(0);

		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			if( !mapp(shopping[city][num]) )
				shopping[city][num] = allocate_mapping(0);

			shopping[city][num]["resident"] = CITY_D->query_section_info(city, num, "resident");
			
			if( shopping[city][num]["totalmoney"] > 0 )
			{
				shopping[city][num]["shopping_power"] = shopping[city][num]["costmoney"] * 100 / shopping[city][num]["totalmoney"];
				worldmoney += shopping[city][num]["money"];
			}

			money = MONEY_PER_RESIDENT;
			
			buff_bonus = 1.0;
			
			if( CITY_D->query_section_info(city, num, SHOPPING_BUFF) > time() )
				buff_bonus += 0.1;
			
			if( CITY_D->query_city_info(city, BATTLEFIELD_SHOPPING_BUFF) > time() )
				buff_bonus += 0.3;
				
			if( BOSS_D->poseidon_exists() )
				buff_bonus -= 0.5;

			money = to_int(money * buff_bonus);

			shopping[city][num]["money"] = shopping[city][num]["resident"] * money;
			shopping[city][num]["totalmoney"] = shopping[city][num]["money"];
			shopping[city][num]["costmoney"] = 0;

			if( !mapp(shopping[city][num]["demand"]) )
				shopping[city][num]["demand"] = allocate_mapping(0);
			else 
			{
				delete_products = allocate(0);
				foreach(product, money in shopping[city][num]["demand"])
				{
					if( money / DEMAND_DECAY_RATE == 0 )
						delete_products |= ({ product });
					else
						shopping[city][num]["demand"][product] = money / DEMAND_DECAY_RATE;
				}
				
				foreach(product in delete_products)
					map_delete(shopping[city][num]["demand"], product);
			}
			
			if( !arrayp(shopping[city][num]["demand_rank"]) )
				shopping[city][num]["demand_rank"] = allocate(0);
		}
	}

	lastworldmoney = worldmoney;

	save();
}

void heart_beat()
{
	int num, lacktime, moneypersec, predictmoney;
	string city;
	mapping demand;
	int time = time();

	if( !(time_tick++ % 3600) )
		refresh_shopping();
	
	if( !(time_tick % 14400) )
		pop_product_choose();

	foreach(city in CITY_D->query_cities() )
	{
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			if( !mapp(shopping[city]) || !mapp(shopping[city][num]) ) continue;

			lacktime = 3600 - (time - refresh_time);

			moneypersec = shopping[city][num]["totalmoney"] / 3600;
			predictmoney = lacktime * moneypersec;
			
			shopping[city][num]["predictmoney"] = predictmoney;
			shopping[city][num]["moneypersec"] = moneypersec;
			
			if( moneypersec > 0 )
				shopping[city][num]["ratio"] = (predictmoney - shopping[city][num]["money"]) / moneypersec;
			
			if( mapp(demand = shopping[city][num]["demand"]) )
				shopping[city][num]["demand_rank"] = sort_array(keys(demand), (: $(demand)[$1] > $(demand)[$2] ? 1 : -1 :));
		}
	}
}

int query_time_tick()
{
	return time_tick;
}

int query_refresh_time()
{
	return refresh_time;
}

int query_world_money()
{
	return lastworldmoney;	
}

void create()
{
	if( !restore_object(DATA_PATH) )
	{
		shopping = allocate_mapping(0);
		refresh_shopping();
		save();
	}

	set_heart_beat(1);
}

string query_name()
{
	return "購物系統(SHOPPING_D)";
}
