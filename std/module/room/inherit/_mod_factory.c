/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _factory_action_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-18
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

inherit __DIR__"_object_database_mod.c";

#include <npc.h>
#include <ansi.h>
#include <message.h>
#include <daemon.h>
#include <material.h>
#include <location.h>
#include <skill.h>
#include <citydata.h>

#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define MAINTAIN_STR_COST	300

void input_object(object env, string database, string basename, int amount);
void output_object(object env, string database, string basename, int amount);
varargs mapping query_objects(object env, string database, string module, string shelflimit);
varargs int query_object_amount(object env, string database, string basename, string shelflimit);

// 取得此工廠可以生產的產品 module 物件陣列
string *query_available_products(object env)
{
	string file;
	string *types;
	object module;
	mapping product_info;
	string building_type;
	
	types = allocate(0);
	
	building_type = env->query_building_type();
	
	foreach(file in get_dir(PRODUCT_MODULE_PATH))
	{
		if( file[<9..<1] != "_module.c" ) continue;

		catch( module = load_object(PRODUCT_MODULE_PATH+file) );
		
		if( !objectp(module) ) continue;
		
		product_info = module->query_product_info();	
		
		if( product_info["factory"] == building_type )
		{
			sscanf(file, "%s_module.c", file);
			types += ({ file });
		}
	}
	
	return types;
}

void do_line_action(object me, string arg)
{
	int productnum;
	object product;
	object env = environment(me);
	object master = env->query_master();
	object warehouse = load_module(query("warehouse", master));
	string *productslist;

	if( !wizardp(me) && query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這間工廠的擁有者。\n");
	
	if( !objectp(warehouse) )
		return tell(me, "尚未設定任何倉儲中心。\n");
	
	if( !arg || !arg[0] )
	{
		string file;
		mapping product_info;
		mapping inlay;
		mapping material_data;
		string msg;
		object material_ob;
		mapping total_material = allocate_mapping(0);

		if( !(file = query("product", master)) )
			return tell(me, "尚未設定製造的產品。\n");
			
		catch( product = load_object(file) );
		
		if( !objectp(product) )
			return tell(me, "無法載入 "+file+" 這個產品！請通知巫師處理。\n");
	
		product_info = product->query_product_info();
		inlay = query("design/inlay", product) || allocate_mapping(0);
	
		msg = "目前正在生產的產品是："+product->query_idname()+"\n";
		msg += WHT"────────────────────────────────\n"NOR;
		msg += BRED"原料名稱                       單位需求 / 庫存數量          狀況\n"NOR;
	
		foreach( string basename_or_module, int need in product_info["material"] )
			total_material[basename_or_module] += need;
		foreach( string basename_or_module, int need in inlay )
			total_material[basename_or_module] += need;
			
		// 檢查原料是否足夠
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "工廠原料");
	
			if( !sizeof(material_data) )
			{
				if( !objectp(material_ob = load_object(basename_or_module)) )
				{
					material_ob = load_object("/std/module/product/"+basename_or_module+"_module.c");
					msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_product_info()["name"], need, 0, HIR"原料不足"NOR);
				}
				else	
					msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_idname(), need, 0, HIR"原料不足"NOR);
			}
				
			foreach(string material_file, int amount in material_data)
			{
				material_ob = load_object(material_file);
				
				msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_idname(), need, amount, amount < need ? HIR"原料不足"NOR : HIG"原料充足"NOR);
			}
		}
	
		msg += WHT"────────────────────────────────\n"NOR;
		return tell(me, msg);
	}

	arg = lower_case(arg);
	
	if( arg && sscanf(arg, "setup %d", productnum) == 1 )
	{
		if( query("line/working", master) )
			return tell(me, "生產線正在運作中，必須先停止後才能更改生產產品種類。\n");
		
		productslist = query("products", me);
		
		if( productnum <= 0 || productnum > sizeof(productslist) )
			return tell(me, "沒有編號 "+productnum+" 的產品。\n");
		
		catch( product = load_object(productslist[productnum-1]) );
		
		if( !objectp(product) )
			return tell(me, "無法載入 "+productslist[productnum-1]+" 這個產品！請通知巫師處理。\n");
	
		if( member_array(product->query_module(), query_available_products(master)) == -1 )
			return tell(me, "這間工廠無法生產這種產品。\n");
		
		set("product", base_name(product), master);
		master->save();	
		msg("$ME將此生產線的製造產品設定為「"+product->query_idname()+"」。\n", me, 0, 1);
	}
	else if( arg == "start" || sscanf(arg, "start %d", productnum) == 1 )
	{
		if( !query("product", master) )
			return tell(me, "尚未設定欲製造的產品，無法開始生產。\n");

		catch( product = load_object(query("product", master)) );
		
		if( !objectp(product) )
			return tell(me, "無法載入 "+query("product", master)+" 這個產品！請通知巫師處理。\n");

		if( productnum <= 0 )
			productnum = 0;
			
		set("line/working", 1, master);
		set("line/amount", productnum, master);
		master->save();
		
		if( !productnum )
			msg("$ME啟動了生產線上所有的設備，開始無限量生產"+product->query_idname()+"。\n", me, 0, 1);
		else
			msg("$ME啟動了生產線上所有的設備，開始生產 "+productnum+" 個"+product->query_idname()+"。\n", me, 0, 1);
		
	}
	else if( arg == "stop" )
	{
		delete("line", master);
		master->save();
		msg("$ME將生產線上所有的設備關機，停止生產產品。\n", me, 0, 1);
	}

	else tell(me, "請輸入正確的指令格式。(help line)\n", me, 0, 1);
}

void do_list_action(object me, string arg)
{	
	int i;
	string file, msg;
	object product;
	object env = environment(me);
	object master = env->query_master();
	string *productslist;
	string *available_products = query_available_products(master);
	string description;
	string current_product = query("product", master);
	string owner = query("owner", master);
	object ownerob;
	
	if( owner == me->query_id(1) )
		ownerob = me;
	else if( user_exists(owner) )
		ownerob = load_user(owner);
	else
		return tell(me, "無法查詢資料。\n");
			
	productslist = query("products", ownerob);
	
	if( !sizeof(productslist) )
		return tell(me, (ownerob == me ? pnoun(2, me) : ownerob->query_idname())+"尚未設計出任何可以生產的產品。\n");
	
	msg = (ownerob == me ? pnoun(2, me) : ownerob->query_idname())+"目前已經研發完成的產品如下：\n";
	msg += WHT"────────────────────────────────\n"NOR;
	msg += "編號 產品名稱                       此工廠可生產   基本價值\n";
	msg += WHT"────────────────────────────────\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		if( current_product == file )
			description = HIM"生產中"NOR;
		else if( member_array(product->query_module(), available_products) != -1 )
			description = HIC"可生產"NOR;
		else
			description = HIR"不可生產"NOR;

		msg += sprintf("%-4d %-31s%-15s%s\n", (++i), product->query_idname(), description, NUMBER_D->number_symbol(product->query_value()));
	}
	
	msg += WHT"────────────────────────────────\n"NOR;
	
	me->more(msg);
	
	if( !userp(ownerob) )
		destruct(ownerob);
}

void do_setup_action(object me, string arg)
{
	string option;
	int x, y, num;
	object warehouse;
	string file;
	string city;
	object env = environment(me);
	object master = env->query_master();
	array loc = env->query_location();

	city = loc[CITY];

	if( !arg )
		return tell(me, pnoun(2, me)+"想要設定什麼項目？(help setup)\n");
	
	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這個工廠的擁有者。\n");

	sscanf(arg, "%s %s", arg, option);

	switch(arg)
	{	
		case "warehouse":
		{
			if( option == "-d" )
			{
				if( !query("warehouse", master) )
					return tell(me, "這裡原本便沒有設定倉儲中心。\n");
					
				delete("delivery", master);
				return tell(me, "取消存貨目標完畢。\n");
			}

			if( !option || sscanf(option, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "請輸入正確的倉庫座標(例 1 21,33)。\n");
			
			// 交貨至倉庫
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");

			if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
				return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並沒有倉庫。\n");
			
			if( warehouse != warehouse->query_master() )
			{
				file = base_name(warehouse->query_master());
				warehouse = warehouse->query_master();
			}

			if( query("owner", warehouse) != me->query_id(1) )
				return tell(me, warehouse->query_room_name()+"並不屬於"+pnoun(2, me)+"。\n");
			
			set("warehouse", file, master);
			
			master->save();
			
			msg("$ME設定"+warehouse->query_room_name()+"為此生產線的倉儲中心。\n", me, 0, 1);
			
			break;
		}

		default:
			return tell(me, "請輸入正確的選項(help setup)。\n");
	}
}

void broadcast_msg(object *rooms, string msg)
{
	foreach(object room in rooms)
		broadcast(room, msg);
}

// 1 秒呼叫一次
void heart_beat(object room)
{
	int timecost;
	int factory_heart_beat;
	object master = room->query_master();
	object product;
	string product_file;
	string ownerid = query("owner", master);
	object owner = find_player(ownerid);

	if( !objectp(owner) || room != master || query("function", master) != "line" || !query("line/working", master) ) return;

	if( !(factory_heart_beat = query_temp("factory_heart_beat", master)) )
		factory_heart_beat = set_temp("factory_heart_beat", random(1200)+1, master);
	else
		factory_heart_beat = addn_temp("factory_heart_beat", 1, master);

	product_file = query("product", master);
	
	if( catch( product = load_object(product_file) ) || !objectp(product) )
		return;
	
	timecost = to_int(pow(to_float(query("value", product)), 0.32));

	if( timecost < 1 )
		timecost = 1;

	set_temp("timecost", timecost, master);

	// 進行生產
	if( !(factory_heart_beat % timecost) )
	{
		object *userinside_room = allocate(0);
		int management;
		int not_enough_material;
		string *slaves = master->query_slave() || allocate(0);
		int amount = sizeof(slaves) + 1;
		string msg = "";
		int material_count;
		int total_need;
		mapping material_data;
		string material_file;
		mapping product_info = product->query_product_info();
		mapping product_info_temp;
		object warehouse = load_module(query("warehouse", master));
		object material_ob;
		mapping inlay = query("design/inlay", product);
		int total_amount = query("line/amount", master);
		mapping total_material = allocate_mapping(0);

		if( !objectp(warehouse) || query("function", warehouse) != "warehouse" )
		{
			broadcast(master, "工廠缺少倉儲中心的支援，導致生產停止。\n");
			delete("line", master);
			master->save();
			return;
		}

		if( total_amount > 0 && total_amount < amount )
			amount = total_amount;

		if( !mapp(inlay) )
			inlay = allocate_mapping(0);

		foreach(object worker in filter(all_inventory(master), (: $1->is_module_npc() && query("boss", $1) == $(ownerid) && query("job/type", $1) == WORKER :)))
			management += worker->query_skill_level("factorymanage")/5;
	
		set("management", management, master);
		
		if( management < sizeof(slaves)+1 )
		{
			broadcast(master, "員工缺乏工廠管理能力，生產停止。\n");
			delete("line", master);
			master->save();
			return;
		}
	
		// 計算是否有玩家在廠內
		foreach(string file in ({ base_name(master) }) + slaves)
			if( sizeof(filter_array(all_inventory(load_object(file)), (: userp($1) :))) )
				userinside_room += ({ load_object(file) });

		foreach( string basename_or_module, int need in product_info["material"] )
			total_material[basename_or_module] += need;
		foreach( string basename_or_module, int need in inlay )
			total_material[basename_or_module] += need;
			
		// 檢查原料是否足夠
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "工廠原料");

			if( implode(values(material_data), (: $1 + $2 :)) < need * amount )
			{
				not_enough_material = 1;

				if( sizeof(userinside_room) )
				{
					if( objectp(material_ob = load_object(basename_or_module)) )
						msg += "倉庫「工廠原料」分類中缺乏足夠的「"+material_ob->query_idname()+"」原料(至少需要 "+(need*amount)+" 個)。\n";
					else if( objectp(material_ob = load_object("/std/module/product/"+basename_or_module+"_module.c")) )
					{
						product_info_temp = material_ob->query_product_info();
						msg += "倉庫「工廠原料」分類中缺乏足夠的「"+product_info_temp["name"]+"("+capitalize(product_info_temp["id"])+")"+"」原料(至少需要 "+(need*amount)+" 個)。\n";
					}
					else
						msg += "倉庫「工廠原料」分類中缺乏足夠的「"+basename_or_module+"」原料(至少需要 "+(need*amount)+" 個)。\n";
				}
			}
		}
		
		if( not_enough_material )
		{
			broadcast_msg(userinside_room, msg);
			return;
		}
		
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "工廠原料");
			
			total_need = need * amount;

			foreach(material_file, material_count in material_data)
			{
				if( material_count >= total_need )
				{
					output_object(warehouse, "products", material_file, total_need);
					
					if( sizeof(userinside_room) )
						msg += "所有連鎖工廠產線共消耗了 "+total_need+" 個"+load_object(material_file)->query_idname()+"。\n";

					break;
				}
				else
				{
					output_object(warehouse, "products", material_file, material_count);
				
					if( sizeof(userinside_room) )
						msg += "所有連鎖工廠產線共消耗了 "+material_count+" 個"+load_object(material_file)->query_idname()+"。\n";
						
					total_need -= material_count;

					continue;
				}
			}
		}
		
		if( sizeof(userinside_room) )
			msg += "所有連鎖工廠產線共製造生產出 "+amount+" 個"+product->query_idname()+"。\n";
			
		input_object(warehouse, "products", product_file, amount);
		
		if( sizeof(userinside_room) )
			broadcast_msg(userinside_room, msg);

		PRODUCT_D->count_product_manufacture(product_file, amount);

		if( !(factory_heart_beat % (timecost*100)) )
			warehouse->delay_save(600);
			
		if( total_amount > 0 && addn("line/amount", -amount, master) <= 0 )
		{
			broadcast(master, "已達目標產量，生產停止。\n");
			delete("line", master);
			master->save();
		}
	}
}

string look_room(object room)
{
	if( query("function", room) == "line" )
	{
		string msg = "";
		object master = room->query_master();
		int timecost = query_temp("timecost", master);
		int factory_heart_beat = query_temp("factory_heart_beat", master);
		object warehouse = load_module(query("warehouse", master));
		object product = load_module(query("product", master));
		int amount = query("line/amount", master);
		int working = query("line/working", master);
		
		msg += "產線運作："+(working ? HIG"生產中"NOR GRN"(下次產出時間："+(timecost ? (timecost - factory_heart_beat%timecost)+" 秒後":"未知")+")"NOR : HIR"停止中"NOR)+"\n";

		if( working )
			msg += "剩餘產量："+(!amount ? "無限量" : amount+" 個")+"\n";

		msg += "製造產品："+(objectp(product) ? product->query_idname() : "無")+"\n";
		msg += "倉儲中心："+(objectp(warehouse) ? warehouse->query_room_name() : "無")+"\n";
		msg += "工廠管理："HIY+(sizeof(master->query_slave())+1)+"/"+query("management", master)+NOR"\n\n";
		return msg;
	}
}
