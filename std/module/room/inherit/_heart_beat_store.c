/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _heart_beat_store.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-28
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <map.h>
#include <location.h>
#include <npc.h>

void output_object(object env, string database, string basename, int amount);
void refresh_class(object env, string database);

void virtual_shopping(object master)
{
	int i, size, average_desire, productdesire, num, security, management, ratio, totalrooms, cost;
	string city, replace_file_name, shoppingmsg="";
	object product, *msgrooms, slave, boss;
	mapping products;
	int value, amount;
	int *nums;
	int demand_rank;
	int total_sell=0, total_earnmoney=0;

	if( query("mode", master) )
		return;
	
	products = query("products", master);
	
	if( !mapp(products) || !sizeof(products) )
		return;

	city = master->query_city();
	nums = ({ master->query_city_num() });

	msgrooms = allocate(0);
	security = query("shopping/security", master);
	management = query("shopping/management", master);
	
	foreach( string file in (master->query_slave() || allocate(0)) + ({ base_name(master) }) )
	{
		if( !objectp(slave = load_object(file)) ) continue;
		
		if( sizeof(filter_array(all_inventory(slave), (: userp($1) :))) )
			msgrooms += ({ slave });
		
		average_desire += query("shopping/desire", slave);
		
		nums += ({ slave->query_city_num() });
	}

	totalrooms = sizeof(master->query_slave())+1;
	average_desire /= totalrooms;
	set("shopping/average_desire", average_desire, master);
	
	boss = find_player(query("owner", master));

	foreach( string shelf, array data in products )
	{	
		size = sizeof(data);
		
		if( !shelf ) continue;
			
		for(i=0;i<size;i+=2)
		{
			num = nums[random(sizeof(nums))];

			ratio = SHOPPING_D->shopping_cost_ratio(city, num);

			if( ratio > 1 && random(ratio) > random(10) ) continue;

			replace_file_name = replace_string(data[i], "/", "#");
			
			productdesire = average_desire + query("shopping/productdesire/"+replace_file_name, master);
			
			if( productdesire > random(360) )
			{				
				if( catch(product = load_object(data[i])) )
					continue;
		
				if( !objectp(product) && !file_exists(data[i]) )
				{
					output_object(master, "products", data[i], -3);
					continue;
				}

				if( query("badsell", product) )
					continue;

				value = to_int(query("setup/price/"+replace_file_name, master) || query("value", product));

				demand_rank = SHOPPING_D->query_demand_rank(city, num, product);

				if( demand_rank == 100 && random(100) )
					continue;
	
				// 需求 + 購買慾愈低，被購買的機會愈低
				if( random(to_int(pow(demand_rank, 1.7))) > 300 || random(pow(demand_rank, 0.9)+50) > random(productdesire*2+50) )
					continue;

				// 購買欲和價格影響數量
				amount = to_int(ceil( pow(productdesire, 0.9) * pow(totalrooms, 0.4) / pow(to_float(value)/100., 0.3) ));

				if( ratio > 1 )
					amount /= pow(ratio, 0.5);

				amount = range_random(to_int(amount/3), to_int(amount));

				if( amount <= 0 )
				{
					if( !random(100) )
						amount = 1;
					else
						continue;
				}

				if( amount > to_int(data[i+1]) )
					amount = to_int(data[i+1]);

				cost = amount * value;
				
				// 消費支出不夠了
				if( SHOPPING_D->query_shopping_info(city, num, "money") < cost )
					continue;
					
				output_object(master, "products", data[i], amount);
				
				SHOPPING_D->count_demand(city, num, product, cost);
				
				if( security < totalrooms )
				{
					shoppingmsg += HIY"架子上的"+product->query_idname()+HIY"似乎隱約少了一些...。\n"NOR;
					
					if( boss )
						tell(boss, master->query_room_name()+"傳來消息：似乎有一些小偷正在偷架子上的商品...。\n");
					
					continue;
				}
					
				total_sell += amount;
				total_earnmoney += cost;

				if( sizeof(msgrooms) )
					shoppingmsg += "店員將"+QUANTITY_D->obj(product, amount)+"交給了顧客，並收取"+HIY+QUANTITY_D->money_info(city, cost)+NOR"。\n";
				
				SHOPPING_D->shopping_cost_money(city, num, cost);
			}
		}
	}

	if( total_sell > 0 )
	{
		addn("money", total_earnmoney, master);
		addn("totalsell", total_sell, master);
		set("lastselltime", time(), master);
	}

	if( sizeof(msgrooms) )
	{
		foreach( slave in msgrooms )
		{
			broadcast(slave, shoppingmsg);
			
			if( !random(3) )
			switch(average_desire)
			{
				case 1..70:
					broadcast(slave, "稀稀落落的顧客在店裡選購著商品...。\n"); break;
				case 71..140:
					broadcast(slave, "顧客們輕鬆地瀏覽著各種商品...。\n"); break;
				case 141..210:
					broadcast(slave, "顧客們交頭接耳地討論著各種商品...。\n"); break;
				case 211..280:
					broadcast(slave, "熱鬧的店裡充滿著顧客們的歡笑聲...。\n"); break;
				case 281..350:
					broadcast(slave, "大量的顧客們興奮地享受著購物的樂趣...。\n"); break;
				case 351..MAX_INT:
					broadcast(slave, "店裡走道上擠滿了聞名而來的顧客，熱鬧非凡...。\n"); break;
			}
		}
	}
	
	if( objectp(boss) && !random(10) )
	{
		if( security < totalrooms )
			tell(boss, master->query_room_name()+"的員工提出報告：商店的巡邏保全("+security+")低於目前的連鎖門市數量("+totalrooms+")，將有商品遭竊的疑慮...。\n");
	
		if( management < totalrooms )
			tell(boss, master->query_room_name()+"的員工提出報告：商店的管理上限("+management+")低於目前的連鎖門市數量("+totalrooms+")，造成城市繁榮度的損失(-"+totalrooms+")...。\n");
	}
}


// 計算產品基本購買慾，僅與產品自身價格或特性有關，與建築物無關
varargs void calculate_master_data(object master, string spec_product)
{
	int i, size, desire, security, productdesire, valuepercent, management;
	string popproduct = SHOPPING_D->query_pop_product();
	object product, inv;
	mapping products;
	int value, setvalue;
	string owner = query("owner", master);
	int has_air_conditioner = 0;
	
	products = query("products", master);

	if( mapp(products) )
	foreach( string shelf, string *data in products )
	{	
		size = sizeof(data);
		
		for(i=0;i<size;i+=2)
		{
			if( spec_product && data[i] != spec_product ) continue;
			
			productdesire = 0;

			switch(shelf)
			{
				case "促銷": productdesire += 60; break;
				case "展示": continue; break;
				default: break;
			}
			
			if( catch(product = load_object(data[i])) )
				continue;

			if( !objectp(product) && !file_exists(data[i]) )
			{
				output_object(master, "products", data[i], -3);
				continue;
			}

			if( data[i] == popproduct || (product->is_module_product() && product->query_module() == popproduct) )
				productdesire += 40;
	
			value = to_int(query("value", product));
			
			if( value <= 0 )
			{
				set("shopping/productdesire/"+replace_string(data[i], "/", "#"), -10000,  master);
				continue;
			}

			setvalue = to_int(query("setup/price/"+replace_string(data[i], "/", "#"), master)) || value;
			
			valuepercent = 250 * (value - setvalue) / value;
			
			if( valuepercent < -10000 || valuepercent > 10000 ) valuepercent = -10000;
			
			// 價格影響購買欲
			productdesire += valuepercent;

			// 隨機買氣波動
			productdesire += range_random(-5, 5);

			// 難賣的商品
			if( query("badsell", product) )
				productdesire = -9999;

			set("shopping/productdesire/"+replace_string(data[i], "/", "#"), productdesire,  master);
		}
	}
	
	// 店員計算
	foreach( inv in all_inventory(master) )
	{
		if( query("job/type", inv) == CLERK && query("boss", inv) == owner )
		{
			desire += inv->query_skill_level("eloquence")/5 + inv->query_cha()*2;
			security += inv->query_skill_level("security")*2;
			management += inv->query_skill_level("storemanage")*2;
		}
		else if( inv->query_module() == "air_conditioner" )
		{
			has_air_conditioner = 1;

			if( !random(to_int(pow(to_float(query("effect", inv)), 0.9))) )
				destruct(inv, 1);			
		}
	}
	
	if( has_air_conditioner == 1 )
		desire += 500;

	set("shopping/basic_desire", desire/100, master);
	set("shopping/security", security/6, master);
	set("shopping/management", management/5, master);
}


// 計算建築物買氣，與產品無關
void calculate_shopping_desire(object room)
{
	int desire;
	array loc;
	object ownerob, master;
	string roomlong, ownerid;
	mapping products;

	master = room->query_master();
	
	products = query("products", master);
	
	if( !mapp(products) || !sizeof(products) || BUILDING_D->is_building_room_locked(room) )
	{
		delete("shopping/desire", room);
		return;
	}

	// 連鎖房間數影響
	desire += sizeof(master->query_slave())+1;

	loc = room->query_location();
	
	// 依照區域產生不同的購買欲望
	switch( CITY_D->query_coor_data(loc, "region") )
	{
		// 商業區
		case COMMERCE_REGION:
			desire += 20; break;
		// 農業區
		case AGRICULTURE_REGION:
			desire += 12; break;
		// 工業區
		case INDUSTRY_REGION:
			desire += 3; break;
	}
	
	roomlong = query("long", room);
	
	// 長敘述
	if( stringp(roomlong) && strsrch(roomlong, "油漆氣味") == -1 && strlen(roomlong) > 100 )
		desire += 5;
	
	// 招牌
	if( strlen(CITY_D->query_coor_icon(loc)) > 2 )
		desire += 3;
		
	// 若老闆在線上, 依照老闆魅力增加購買慾
	ownerid = query("owner", room);
	
	if( stringp(ownerid) && objectp(ownerob = find_player(ownerid)) )
		desire += ownerob->query_cha();

	if( find_object(BASEBALL_D) && BASEBALL_D->in_season(ownerid) )
		desire += 20;
		
	if( CITY_D->has_building("prodigy", loc[CITY], loc[NUM]) )
		desire *= 1.1;
		
	set("shopping/desire", desire+query("shopping/basic_desire", master), room);
}


//更新銷售排名
void update_top_totalsell(object room)
{
	string id = query("owner", room);
	
	if( SECURE_D->is_wizard(id) ) return;

	TOP_D->update_top("storesell", base_name(room), query("totalsell", room), id, room->query_room_name(3), room->query_city());
}

// 心跳 (每 5 秒一次)
void heart_beat(object room)
{
	string *slaves;
	int store_heart_beat;
	
	if( query("function", room) != "front" || !sizeof(query("products", room)) ) return;

	// 如果 SHOPPING_D 沒有載入, 則 return;
	if( !find_object(SHOPPING_D) ) return;
	
	slaves = room->query_slave() || allocate(0);
		
	if( !(store_heart_beat = query_temp("store_heart_beat", room)) )
		store_heart_beat = set_temp("store_heart_beat", random(1200)+1, room);
	else
		addn_temp("store_heart_beat", 1, room);

	// 消費行為
	if( find_player(query("owner", room)) )
		virtual_shopping(room);
	else if( !(store_heart_beat % 50) )
		virtual_shopping(room);
	
	// 5 分鐘計算一次商品購買慾
	if( !(store_heart_beat%61) )
		calculate_master_data(room);

	// 10 分鐘計算一次買氣
	if( !(store_heart_beat%121) )
		calculate_shopping_desire(room);
	
	// 45 分鐘更新一次 TOP
	if( !(store_heart_beat%541) )
		update_top_totalsell(room);
	
	// 20 分鐘儲存一次
	if( !(store_heart_beat%241) )
		room->save();

	foreach(string file in slaves)
	{
		reset_eval_cost();

		if( !objectp(room = load_object(file)) ) continue;

		if( !(store_heart_beat = query_temp("store_heart_beat", room)) )
			store_heart_beat = set_temp("store_heart_beat", random(10000)+1, room);
		else
			addn_temp("store_heart_beat", 1, room);
			
		// 10 分鐘計算一次買氣
		if( !(store_heart_beat%121) )
			calculate_shopping_desire(room);
	}
}
