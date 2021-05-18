/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_transport.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-29
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <npc.h>

int over_capacity(object env, string database, int amount, int capacity);
void input_object(object env, string database, string basename, int amount);
void output_object(object env, string database, string basename, int amount);

#define STORE_CAPACITY		100000
#define WAREHOUSE_CAPACITY	-1

// 運輸物品
void do_transport(object me, string arg, string database)
{
	int i, which = 1, size, capacity;
	object master, product, delivery, delivery_master;
	mapping products;
	string tmp, env_city, productname, option, basename;
	int cost, amount = 1, pamount; 
	string delivery_owner;
	int delivery_x, delivery_y, num;
	array productlist;
	array loc;
	
	master = environment(me)->query_master();

	env_city = master->query_city();

	if( !arg )
		return tell(me, pnoun(2, me)+"想要運輸什麼物品？\n");

	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是此處的擁有者。\n");

	if( sscanf(arg, "%s to %d %d,%d", arg, num, delivery_x, delivery_y) != 4 )
		return tell(me, "請輸入正確的運輸目的地位置。\n");

	if( !CITY_D->city_exist(env_city, num-1) )
		return tell(me, "這座城市並沒有第 "+num+" 衛星都市。\n");

	loc = arrange_city_location(delivery_x-1, delivery_y-1, env_city, num-1);
	
	delivery = load_module(loc);
	
	if( !objectp(delivery) )
		return tell(me, "座標"+loc_short(loc)+"處並沒有任何建築物。\n");
		
	delivery_owner = query("owner", delivery);
	
	if( delivery_owner != me->query_id(1) )
	{
		if( ENTERPRISE_D->same_enterprise(delivery_owner, me->query_id(1)) )
			tell(me, delivery->query_room_name()+"為企業集團旗下產業，擁有者為 "+capitalize(delivery_owner)+"，允許直接運輸。\n");
		else
			return tell(me, delivery->query_room_name()+"並不是"+pnoun(2, me)+"的建築物。\n");
	}
	
	delivery_master = delivery->query_master();

	switch( delivery->query_building_type() )
	{
		case "store":
			capacity = STORE_CAPACITY;
			break;
		case "warehouse":
			capacity = WAREHOUSE_CAPACITY;
			break;
		default:
			return tell(me, delivery->query_room_name()+"沒有辦法接收任何物品。\n");
	}
	
	switch( query("function", delivery) )
	{
		case "front":
		case "warehouse":
			break;
		default:
			return tell(me, delivery->query_room_name()+"沒有辦法接收任何物品。\n");
	}
	
	if( delivery_master == master )
		return tell(me, pnoun(2, me)+"欲運送的物品地點不能與此地相同或有相同連鎖中心。\n");

		
	sscanf(arg, "%s %s", option, productname);
	
	if( !productname || (option != "all" && !(amount = to_int(option))) )
	{
		productname = arg;
		amount = 1;
	}
	
	if( sscanf( productname, "%s %d", tmp, which ) == 2 )
		productname = tmp;

	if( which < 1 ) which = 1;

	products = query(database, master);
	
	if( !sizeof(products) )
		return tell(me, query("short", master)+"目前沒有任何物品。\n");

	productlist = implode(values(products), (:$1+$2:))||allocate(0);
	size = sizeof(productlist);

	for(i=0;i<size;i+=2)
	{
		basename = productlist[i];
		pamount = to_int(productlist[i+1]);

		if( catch(product = load_object(basename)) )
			continue;

		// 檔案已經消失, 刪除資料
		if( !objectp(product) && !file_exists(basename) )
		{
			output_object(master, database, basename, -3);
			continue;
		}
		
		if( (i+2)/2 == to_int(big_number_check(productname)) || (product->query_id(1) == lower_case(productname) && !(--which)) )
		{
			if( option == "all" )
				amount = pamount;
			else if( amount > pamount )
				return tell(me, "這裡並沒有這麼多"+(query("unit", product)||"個")+product->query_idname()+"。\n");
			else if( amount < 1 )
				return tell(me, "請輸入正確的運輸數量。\n");

			if( query("flag/no_give", product) && delivery_owner != me->query_id(1) )
				return tell(me, product->query_idname()+"無法運輸給其他玩家。\n");
			
			cost = amount * to_int(query("setup/price/"+replace_string(basename, "/", "#"), master)||query("value", product));
			
			// 數量超過可上架數量
			if( over_capacity(delivery_master, database, amount, capacity) )
			{
				master->delay_save(300);
				delivery_master->delay_save(300);
				return tell(me, delivery->query_room_name()+"無法再容納這麼多的物品了。\n");
			}
				
			input_object(delivery_master, database, basename, amount);

			output_object(master, database, basename, amount);
			
			if( find_player(delivery_owner) == me )
				msg("$ME將"+QUANTITY_D->obj(product, amount)+"運輸至"+delivery->query_room_name()+"。\n", me, 0, 1);
			else
				msg("$ME將"+QUANTITY_D->obj(product, amount)+"運輸至"+delivery->query_room_name()+"。\n", me, find_player(delivery_owner), 1);
			
			if( me->query_id(1) != delivery_owner )
				log_file("command/transport", me->query_idname()+"將"+QUANTITY_D->obj(product, amount)+"運輸給 "+delivery_owner+"。");
			
			master->delay_save(300);
			delivery_master->delay_save(300);
			return;
		}
	}
	
	return tell(me, "這裡並沒有 "+productname+" 這種物品。\n");
}