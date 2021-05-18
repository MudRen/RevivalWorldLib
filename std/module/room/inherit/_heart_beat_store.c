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
	
				// �ݨD + �ʶR���U�C�A�Q�ʶR�����|�U�C
				if( random(to_int(pow(demand_rank, 1.7))) > 300 || random(pow(demand_rank, 0.9)+50) > random(productdesire*2+50) )
					continue;

				// �ʶR���M����v�T�ƶq
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
				
				// ���O��X�����F
				if( SHOPPING_D->query_shopping_info(city, num, "money") < cost )
					continue;
					
				output_object(master, "products", data[i], amount);
				
				SHOPPING_D->count_demand(city, num, product, cost);
				
				if( security < totalrooms )
				{
					shoppingmsg += HIY"�[�l�W��"+product->query_idname()+HIY"���G�����֤F�@��...�C\n"NOR;
					
					if( boss )
						tell(boss, master->query_room_name()+"�ǨӮ����G���G���@�Ǥp�����b���[�l�W���ӫ~...�C\n");
					
					continue;
				}
					
				total_sell += amount;
				total_earnmoney += cost;

				if( sizeof(msgrooms) )
					shoppingmsg += "�����N"+QUANTITY_D->obj(product, amount)+"�浹�F�U�ȡA�æ���"+HIY+QUANTITY_D->money_info(city, cost)+NOR"�C\n";
				
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
					broadcast(slave, "�}�}�������U�Ȧb���̿��ʵ۰ӫ~...�C\n"); break;
				case 71..140:
					broadcast(slave, "�U�ȭ̻��P�a�s���ۦU�ذӫ~...�C\n"); break;
				case 141..210:
					broadcast(slave, "�U�ȭ̥��Y���զa�Q�׵ۦU�ذӫ~...�C\n"); break;
				case 211..280:
					broadcast(slave, "���x�����̥R�����U�ȭ̪��w���n...�C\n"); break;
				case 281..350:
					broadcast(slave, "�j�q���U�ȭ̿��Ħa�ɨ����ʪ����ֽ�...�C\n"); break;
				case 351..MAX_INT:
					broadcast(slave, "���̨��D�W�����F�D�W�ӨӪ��U�ȡA���x�D�Z...�C\n"); break;
			}
		}
	}
	
	if( objectp(boss) && !random(10) )
	{
		if( security < totalrooms )
			tell(boss, master->query_room_name()+"�����u���X���i�G�ө������ޫO��("+security+")�C��ثe���s������ƶq("+totalrooms+")�A�N���ӫ~�D�Ѫ��ü{...�C\n");
	
		if( management < totalrooms )
			tell(boss, master->query_room_name()+"�����u���X���i�G�ө����޲z�W��("+management+")�C��ثe���s������ƶq("+totalrooms+")�A�y�������c�a�ת��l��(-"+totalrooms+")...�C\n");
	}
}


// �p�ⲣ�~���ʶR���A�ȻP���~�ۨ�����ίS�ʦ����A�P�ؿv���L��
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
				case "�P�P": productdesire += 60; break;
				case "�i��": continue; break;
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
			
			// ����v�T�ʶR��
			productdesire += valuepercent;

			// �H���R��i��
			productdesire += range_random(-5, 5);

			// ���檺�ӫ~
			if( query("badsell", product) )
				productdesire = -9999;

			set("shopping/productdesire/"+replace_string(data[i], "/", "#"), productdesire,  master);
		}
	}
	
	// �����p��
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


// �p��ؿv���R��A�P���~�L��
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

	// �s��ж��Ƽv�T
	desire += sizeof(master->query_slave())+1;

	loc = room->query_location();
	
	// �̷Ӱϰ첣�ͤ��P���ʶR����
	switch( CITY_D->query_coor_data(loc, "region") )
	{
		// �ӷ~��
		case COMMERCE_REGION:
			desire += 20; break;
		// �A�~��
		case AGRICULTURE_REGION:
			desire += 12; break;
		// �u�~��
		case INDUSTRY_REGION:
			desire += 3; break;
	}
	
	roomlong = query("long", room);
	
	// ���ԭz
	if( stringp(roomlong) && strsrch(roomlong, "�o�����") == -1 && strlen(roomlong) > 100 )
		desire += 5;
	
	// �۵P
	if( strlen(CITY_D->query_coor_icon(loc)) > 2 )
		desire += 3;
		
	// �Y����b�u�W, �̷Ӧ���y�O�W�[�ʶR��
	ownerid = query("owner", room);
	
	if( stringp(ownerid) && objectp(ownerob = find_player(ownerid)) )
		desire += ownerob->query_cha();

	if( find_object(BASEBALL_D) && BASEBALL_D->in_season(ownerid) )
		desire += 20;
		
	if( CITY_D->has_building("prodigy", loc[CITY], loc[NUM]) )
		desire *= 1.1;
		
	set("shopping/desire", desire+query("shopping/basic_desire", master), room);
}


//��s�P��ƦW
void update_top_totalsell(object room)
{
	string id = query("owner", room);
	
	if( SECURE_D->is_wizard(id) ) return;

	TOP_D->update_top("storesell", base_name(room), query("totalsell", room), id, room->query_room_name(3), room->query_city());
}

// �߸� (�C 5 ��@��)
void heart_beat(object room)
{
	string *slaves;
	int store_heart_beat;
	
	if( query("function", room) != "front" || !sizeof(query("products", room)) ) return;

	// �p�G SHOPPING_D �S�����J, �h return;
	if( !find_object(SHOPPING_D) ) return;
	
	slaves = room->query_slave() || allocate(0);
		
	if( !(store_heart_beat = query_temp("store_heart_beat", room)) )
		store_heart_beat = set_temp("store_heart_beat", random(1200)+1, room);
	else
		addn_temp("store_heart_beat", 1, room);

	// ���O�欰
	if( find_player(query("owner", room)) )
		virtual_shopping(room);
	else if( !(store_heart_beat % 50) )
		virtual_shopping(room);
	
	// 5 �����p��@���ӫ~�ʶR��
	if( !(store_heart_beat%61) )
		calculate_master_data(room);

	// 10 �����p��@���R��
	if( !(store_heart_beat%121) )
		calculate_shopping_desire(room);
	
	// 45 ������s�@�� TOP
	if( !(store_heart_beat%541) )
		update_top_totalsell(room);
	
	// 20 �����x�s�@��
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
			
		// 10 �����p��@���R��
		if( !(store_heart_beat%121) )
			calculate_shopping_desire(room);
	}
}
