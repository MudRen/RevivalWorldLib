/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : 拍賣精靈
 * Update :
 *  o 2002-09-26 
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <money.h>

#define DATA		"/data/daemon/auction.o"

#define STORE_CAPACITY		100000
#define WAREHOUSE_CAPACITY	-1

#define AUCTION_STATUS_GOING	0
#define AUCTION_STATUS_FINISHED	1
#define AUCTION_STATUS_FAILED	2
#define AUCTION_STATUS_SOLD	3

#define REQUEST_STATUS_GOING	0
#define REQUEST_STATUS_FINISHED	1

array auction;
array request;

int save()
{
	save_object(DATA);
}

array query_auction()
{
	return auction;	
}

void add_auction(mapping data)
{	
	auction += ({ data });
	
	save();
}

void remove_auction(int i)
{
	auction = auction[0..i-1] + auction[i+1..];
	
	save();
}

void add_request(mapping data)
{	
	request += ({ data });
	
	save();
}

void remove_request(int i)
{
	request = request[0..i-1] + request[i+1..];
	
	save();
}

void reset_request()
{
	request = allocate(0);
}

array query_request()
{
	return request;
}

void refresh_top_buyer(int i)
{
	object old_top_buyer;
	object new_top_buyer;

	if( i < 0 || i >= sizeof(auction) ) return;

	if( stringp(auction[i]["top_buyer"]) )
		old_top_buyer = find_player(auction[i]["top_buyer"]);
		
	if( sizeof(auction[i]["auction_data"]) )
	{
		auction[i]["top_buyer"] = auction[i]["auction_data"][0][0];
		auction[i]["top_price"] = auction[i]["auction_data"][0][1];
	}
	
	if( stringp(auction[i]["top_buyer"]) )
		new_top_buyer = find_player(auction[i]["top_buyer"]);
	
	if( old_top_buyer != new_top_buyer && objectp(old_top_buyer) )
		tell(old_top_buyer, "\a"+pnoun(2, old_top_buyer)+"的拍賣出價已經被其他人超過。\n");
}

void refresh_all_top_buyer()
{
	for(int i=0;i<sizeof(auction);i++)
		refresh_top_buyer(i);
}

void do_auction_list(object me, string arg)
{
	int i;
	string msg;
	mapping data;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	int top_price;
	string top_buyer;
	string status;
	
	if( !arg )
	{
		msg = HIB"目前全世界的拍賣品如下：\n"NOR;
		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg +=    "編號 拍賣品                                  數量        目前最高出價    剩餘時間\n"NOR;
		msg += WHT"─────────────────────────────────────────\n"NOR;
	
		foreach(data in auction)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) ) continue;
			
			top_price = to_int(data["top_price"]);
			top_buyer = data["top_buyer"];
		
			if( data["status"] == AUCTION_STATUS_FINISHED )
				status = HIR"結標"NOR;
			else if( data["status"] == AUCTION_STATUS_FAILED )
				status = HIG"流標"NOR;
			else if( data["status"] == AUCTION_STATUS_SOLD )
				status = HIY"售出"NOR;
			else
				status = AUCTION_STATUS_GOING;

			msg += sprintf("%-5d%s%20s %11s\n",
				++i,
				ob->query_idname()+repeat_string(" ", 44-noansi_strlen(ob->query_idname()+NUMBER_D->number_symbol(data["amount"])))+HIG+NUMBER_D->number_symbol(data["amount"])+NOR,
				top_price ? HIY+NUMBER_D->number_symbol(top_price)+NOR : NOR YEL+NUMBER_D->number_symbol(data["base_price"])+NOR,
				status || ((data["minute"]>=60)?HIM+(data["minute"]/60)+"h"+NOR:NOR BLINK MAG+data["minute"]+"m"NOR),
			);
		}

		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg += "list '編號' 觀看該拍賣品的詳細拍賣資料\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(auction) )
		return tell(me, "請輸入正確的編號。\n");
		
	data = auction[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
		error(data["basename"]+" 物件載入失敗");
	
	if( data["status"] == AUCTION_STATUS_FINISHED )
		status = HIR"結標"NOR;
	else if( data["status"] == AUCTION_STATUS_FAILED )
		status = HIG"流標"NOR;
	else if( data["status"] == AUCTION_STATUS_SOLD )
		status = HIY"售出"NOR;
	else
		status = AUCTION_STATUS_GOING;
		
	if( !arrayp(data["list_players"]) )
		data["list_players"] = allocate(0);

	data["list_players"] |= ({ me->query_id(1) });
			
	msg =  "編號 "+i+" 之拍賣品資料如下：\n";
	msg += WHT"───────────────────────────────────────\n"NOR;
	msg += "拍賣者    ："+capitalize(data["seller"])+"\n";
	msg += "拍賣品    ："+ob->query_idname()+"\n";
	msg += "數量      ："+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "拍賣底價  ：$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["base_price"])+"\n";
	msg += "直接購買價：$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["direct_price"])+"\n";
	msg += "剩餘時間  ："+(status || (data["minute"]+" 分鐘"))+"\n";

	if( data["finish_time"] > 0 )
	{
		if( data["status"] == AUCTION_STATUS_FAILED )
			msg += "系統回收  ：此拍賣品將於 "+TIME_D->replace_ctime(data["finish_time"]+14*24*60*60)+" 由系統自動刪除\n";
		else
			msg += "拍賣者領回：得標者未於 "+TIME_D->replace_ctime(data["finish_time"]+7*24*60*60)+" 之前領取拍賣品者，將開放拍賣者領回。\n";
	}

	msg += "競標狀況  ：\n";
	
	if( !sizeof(data["auction_data"]) )
		msg += "    無人出價\n";
	else foreach(array d in data["auction_data"])
	{
		msg += sprintf("    出價者 %-12s  出價 %s\n", (wizardp(me) || d[0] == me->query_id(1)) ? capitalize(d[0]) : "(Unknown)", money(money_unit, d[1]));
	}
	
	ob = new(data["basename"]);
	msg += "\n"+ob->long();
	destruct(ob);
	
	msg += "\n共有 "+sizeof(data["list_players"])+" 位玩家查詢過這項拍賣品。\n";
	msg += WHT"───────────────────────────────────────\n"NOR;
	
	me->more(msg);
}

void do_request_list(object me, string arg)
{
	int i;
	string msg;
	mapping data;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	string status;
	mapping product_info;
	string idname;

	if( !arg || arg == "all" )
	{
		if( !arg )
			msg = HIB"目前全世界的訂單(僅列出未結案物品)如下：\n"NOR;
		else
			msg = HIB"目前全世界的訂單如下：\n"NOR;
		
		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg +=    "編號 訂單                                    數量     單價($RW) 提出者      狀態   \n"NOR;
		msg += WHT"─────────────────────────────────────────\n"NOR;
		
		foreach(data in request)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) )
			{
				if( objectp(ob = load_object("/std/module/product/"+data["basename"]+"_module")) )
				{
					product_info = ob->query_product_info();
					idname = product_info["name"]+"("+capitalize(product_info["id"])+")";
				}
				else
					error("發生錯誤。\n");
			}
			else
				idname = ob->query_idname();
			
			++i;
			
			if( data["status"] == REQUEST_STATUS_GOING )
				status = YEL"未結案"NOR;
			else if( data["status"] == REQUEST_STATUS_FINISHED )
			{
				if( !arg )
					continue;
					
				status = HIY"已結案"NOR;
			}
			
			msg += sprintf("%-5d%s%14s %-12s%s\n",
				i,
				idname+repeat_string(" ", 44-noansi_strlen(idname+NUMBER_D->number_symbol(data["amount"])))+HIG+NUMBER_D->number_symbol(data["amount"])+NOR,
				HIY+NUMBER_D->number_symbol(data["price"])+NOR,
				HIW+capitalize(data["buyer"])+NOR,
				status
			);
		}

		msg += WHT"─────────────────────────────────────────\n"NOR;
		msg += "list '編號' 觀看該拍賣品的詳細拍賣資料\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(request) )
		return tell(me, "請輸入正確的編號。\n");
		
	data = request[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
	{
		if( objectp(ob = load_object("/std/module/product/"+data["basename"]+"_module")) )
			product_info = ob->query_product_info();
		else
			error("發生錯誤。\n");
	}
				
	if( data["status"] == REQUEST_STATUS_FINISHED )
		status = HIY"已結案"NOR;
	else
		status = YEL"未結案"NOR;
				
	msg =  "編號 "+i+" 之訂單資料如下：\n";
	msg += "───────────────────────────────────────\n";
	msg += "買家      ："+capitalize(data["buyer"])+"\n";
	msg += "標的物    ："+(product_info ? product_info["name"]+"("+capitalize(product_info["id"])+") - "HIY"工業產品"NOR : ob->query_idname())+NOR"\n";
	msg += "數量      ："+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "單價      ："HIY+money(money_unit, data["price"])+NOR"\n";
	msg += "總價      ："HIY+money(money_unit, data["price"] * data["amount"])+NOR"\n";
	msg += "狀態      ："+status+"\n";
	msg += "供貨者    ："+(data["supporter"] ? capitalize(data["supporter"]) : "無")+"\n";
	
	if( !product_info )
	{
		ob = new(data["basename"]);
		msg += "\n"+ob->long();
		destruct(ob);
	}

	msg += "───────────────────────────────────────\n";

	tell(me, msg);
}

void confirm_bid(object me, int i, object ob, int price, string money_unit, mapping data, string arg)
{
	int num;
	int top_price;
	string top_buyer;
	int next_price;
	object seller;

	if( arg != "yes" && arg != "y" )
	{
		tell(me, pnoun(2, me)+"決定不競標這個拍賣品。\n");
		return me->finish_input();
	}

	num = i+1;

	// 資料不同
	if( !mapp(auction[i]) || 
		auction[i]["status"] != data["status"] ||
		auction[i]["basename"] != data["basename"] ||
		auction[i]["amount"] != data["amount"] ||
		auction[i]["seller"] != data["seller"] ||
		auction[i]["base_price"] != data["base_price"]
	)
	{
		tell(me, "拍賣品編號順序已經改變，請重新競標。\n");
		return me->finish_input();
	}

	if( auction[i]["status"] == AUCTION_STATUS_FINISHED )
	{
		tell(me, "這項拍賣品已經結標。\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == AUCTION_STATUS_FAILED )
	{
		tell(me, "這項拍賣品已經流標。\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == AUCTION_STATUS_SOLD )
	{
		tell(me, "這項拍賣品已經被直接購買。\n");
		return me->finish_input();
	}

	if( !sizeof(auction[i]["auction_data"]) )
	{
		if( price < auction[i]["base_price"] )
		{
			tell(me, "競價價錢必須高於拍賣底價("+money(money_unit, auction[i]["base_price"])+")。\n");
			return me->finish_input();
		}

		if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
		{
			tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
			return me->finish_input();
		}

		auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) });
		auction[i]["list_players"] = allocate(0);

		refresh_top_buyer(i);

		if( price >= auction[i]["direct_price"] )
		{
			auction[i]["status"] = AUCTION_STATUS_SOLD;
			auction[i]["minute"] = 0;
			auction[i]["finish_time"] = time();

			if( auction[i]["seller"] != "" )
				MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
			CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"結標"NOR"：第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	

			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");
			
			me->finish_input();
			return;	
		}

		CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"出價"NOR"：第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的競標價。\n");
		
		save();

		return me->finish_input();
	}
	else
	{
		top_buyer = auction[i]["top_buyer"];
		top_price = auction[i]["top_price"];

		// 直接購買
		if( price >= auction[i]["direct_price"] )
		{
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{
				tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
				return me->finish_input();
			}

			// 將上一位競標者的錢歸還
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			auction[i]["status"] = AUCTION_STATUS_SOLD;
			auction[i]["minute"] = 0;
			auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
			auction[i]["finish_time"] = time();

			refresh_top_buyer(i);
			
			if( auction[i]["seller"] != "" )
				MONEY_D->earn_money(auction[i]["seller"], money_unit, price);

			CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"結標"NOR"：第 "+num+" 編號的拍賣品「"+ob->query_idname()+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");

			save();

			return me->finish_input();
		}
		else
		{
			if( top_buyer == me->query_id(1) )
			{
				tell(me, pnoun(2, me)+"已經是目前最高出價者。\n");
				return me->finish_input();
			}
			
			next_price = top_price + (top_price / 20);
	
			if( next_price == top_price )
				next_price = next_price + 1;
	
			if( price < next_price )
			{
				tell(me, "每次競標的差距必須是目前最高競標價 + 5% 的底價以上(系統計算結果為："+money(money_unit, next_price)+")\n");
				return me->finish_input();
			}
			
			if( price > top_price * 10 )
			{
				tell(me, "競標價不得高於現有最高出價的 10 倍。\n");
				return me->finish_input();
			}
	
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{	
				tell(me, pnoun(2, me)+"的錢不足 "+money(money_unit, price)+"。\n");
				return me->finish_input();
			}

			// 將上一位競標者的錢歸還
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			if( price >= auction[i]["direct_price"] )
			{
				auction[i]["status"] = AUCTION_STATUS_SOLD;
				auction[i]["minute"] = 0;
				auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
				auction[i]["finish_time"] = time();
	
				refresh_top_buyer(i);
				
				if( auction[i]["seller"] != "" )
					MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
				CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"結標"NOR"：第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的直接購買價，拍賣結標。\n");	
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, me->query_idname()+"將 "HIY+money(money_unit, price)+NOR" 繳付給"+pnoun(2, seller)+"。\n");

				return me->finish_input();
			}

			auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
			
			refresh_top_buyer(i);
			
			if( auction[i]["minute"] < 5 )
			{
				auction[i]["minute"] = 5;
				CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"出價"NOR"：第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的競標價，同時延長拍賣時間為 5 分鐘。\n");	
			}
			else
				CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"出價"NOR"：第 "+num+" 編號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」喊出 "HIY+money(money_unit, price)+NOR" 的競標價。\n");	
			
			save();

			return me->finish_input();
		}
	}
	
	return me->finish_input();
}

void do_bid(object me, string arg)
{
	int i, num;
	mixed price;
	string money_unit = MONEY_D->query_default_money_unit();
	object ob;

	if( wizardp(me) )
		return tell(me, "巫師角色不可參與競標。\n");

	if( !arg || sscanf(arg, "%d %s", num, price) != 2 )
		return tell(me, "請輸入正確的競標資料(help bid)。\n");
		
	if( num < 1 || num > sizeof(auction) )
		return tell(me, "請輸入正確的編號。\n");

	i = num - 1;
	
	if( auction[i]["status"] == AUCTION_STATUS_FINISHED )
		return tell(me, "這項拍賣品已經結標。\n");
	else if( auction[i]["status"] == AUCTION_STATUS_FAILED )
		return tell(me, "這項拍賣品已經流標。\n");
	else if( auction[i]["status"] == AUCTION_STATUS_SOLD )
		return tell(me, "這項拍賣品已經被直接購買。\n");

	ob = load_object(auction[i]["basename"]);
	
	if( !objectp(ob) )
		error(auction[i]["basename"]+" 物件載入錯誤。\n");

	if( price == "cancel" )
	{
		if( auction[i]["seller"] != me->query_id(1) )
			return tell(me, "只有原拍賣者才能取消拍賣。\n");

		if( auction[i]["status"] )
			return tell(me, "此拍賣品已經結標或流標或被直接購買了，無法再取消拍賣。\n");

		if( sizeof(auction[i]["auction_data"]) )
			return tell(me, "已經有人參與競標，無法再取消拍賣了。\n");

		auction[i]["status"] = AUCTION_STATUS_FAILED;
		auction[i]["minute"] = 0;
		auction[i]["finish_time"] = time();
		auction[i]["auction_data"] = allocate(0);
		auction[i]["top_buyer"] = 0;
		auction[i]["top_price"] = 0;

		CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"取消"NOR"："+me->query_idname()+"取消第 "+num+" 編號拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」的拍賣。\n");
		
		return;
	}

	if( me->query_id(1) == auction[i]["seller"] )
		return tell(me, "拍賣者不能參與競標。\n");

	if( price == "auto" )
	{
		if( !sizeof(auction[i]["auction_data"]) )
			price = auction[i]["base_price"];
		else if( auction[i]["top_price"] / 20 > 0 )
			price = auction[i]["top_price"] + (auction[i]["top_price"] / 20);
		else
			price = auction[i]["top_price"] + 1;
		
		if( price > auction[i]["direct_price"] )
			price = auction[i]["direct_price"];
	}
	else
		price = to_int(big_number_check(price));
 	
	if( price < 1 || price > 1000000000000 )
		return tell(me, "請輸入正確的競標價(1-1000000000000)。\n");	

	tell(me, "是否確定以 "HIY+money(money_unit, price)+NOR" 的價格競標「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」(y/n)？");
	
	input_to( (: confirm_bid, me, i, ob, price, money_unit, copy(auction[i]) :) );
}

void do_auction_receive(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	object cloneob;

	if( !arg )
		return tell(me, "請輸入正確的指令(help receive)。\n");

	// 運輸到某個地點
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "請輸入正確的編號。\n");

		i--;

		if( !auction[i]["status"] )
			return tell(me, "此項拍賣品還在競標程序中。\n");

		if( auction[i]["status"] == AUCTION_STATUS_FAILED && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "流標物品必須由拍賣者領回。\n");

		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" 資料載入錯誤。\n");

		if( auction[i]["status"] != AUCTION_STATUS_FAILED && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"並沒有得標這個拍賣品。\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "這座城市並沒有第 "+num+" 衛星都市。\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "座標"+loc_short(loc)+"處並沒有任何建築物。\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"並不是"+pnoun(2, me)+"的建築物。\n");
		
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
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", auction[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"無法再容納這麼多的物品了。\n");

		if( auction[i]["status"] != AUCTION_STATUS_FAILED )
		{
			log_file("daemon/auction", me->query_idname()+"得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」");
			msg("$ME將得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」運輸到"+query("short", delivery)+loc_short(loc)+"。\n", me, 0, 1);	
		}
		else
			msg("$ME將「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領回並運輸到"+query("short", delivery)+loc_short(loc)+"。\n", me, 0, 1);
			
		delivery->query_module_file()->input_object(delivery_master, "products", auction[i]["basename"], auction[i]["amount"]);
		remove_auction(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "請輸入正確的編號。\n");
		
		i--;

		if( !auction[i]["status"] )
			return tell(me, "此項拍賣品還在競標程序中。\n");

		if( auction[i]["status"] == AUCTION_STATUS_FAILED && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "流標物品必須由拍賣者領回。\n");
			
		
		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" 資料載入錯誤。\n");
	
		if( auction[i]["status"] != AUCTION_STATUS_FAILED && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"並沒有得標這個拍賣品。\n");

		cloneob = new(auction[i]["basename"]);
		
		if( !me->available_get(cloneob, auction[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"拿不動這麼多東西。\n");
		}
			
		if( auction[i]["status"] != AUCTION_STATUS_FAILED )
		{
			log_file("daemon/auction", me->query_idname()+"得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」");
			msg("$ME將得標物品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領取到身上。\n", me, 0, 1);
		}
		else
			msg("$ME將「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」領回到身上。\n", me, 0, 1);
				
		if( !cloneob->no_amount() )
		{
			cloneob->set_amount(auction[i]["amount"]);
			cloneob->move(me);
		}
		else
			cloneob->move(me, auction[i]["amount"]);
		

		remove_auction(i);
	}
	else
		return tell(me, "請輸入正確的收貨指令(help receive)。\n");
}

void do_request_receive(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	object cloneob;

	if( !arg )
		return tell(me, "請輸入正確的指令(help receive)。\n");

	// 運輸到某個地點
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "請輸入正確的編號。\n");

		i--;

		if( request[i]["status"] != REQUEST_STATUS_FINISHED )
			return tell(me, "此項訂單尚無人供貨。\n");

		if( request[i]["buyer"] != me->query_id(1) )
			return tell(me, "這張訂單不是"+pnoun(2, me)+"下的。\n");

		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
			error(request[i]["basename"]+" 資料載入錯誤。\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "這座城市並沒有第 "+num+" 衛星都市。\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "座標"+loc_short(loc)+"處並沒有任何建築物。\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"並不是"+pnoun(2, me)+"的建築物。\n");
		
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
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"沒有辦法接收任何商品。\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", request[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"無法再容納這麼多的物品了。\n");

		msg("$ME將貨物「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」運輸到"+query("short", delivery)+loc_short(loc)+"。\n", me, 0, 1);	
			
		delivery->query_module_file()->input_object(delivery_master, "products", request[i]["basename"], request[i]["amount"]);
		remove_request(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "請輸入正確的編號。\n");
		
		i--;

		if( request[i]["status"] != REQUEST_STATUS_FINISHED )
			return tell(me, "此項訂單尚無人供貨。\n");

		if( request[i]["buyer"] != me->query_id(1) )
			return tell(me, "這張訂單不是"+pnoun(2, me)+"下的。\n");
		
		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
			error(request[i]["basename"]+" 資料載入錯誤。\n");

		cloneob = new(request[i]["basename"]);
		
		if( !me->available_get(cloneob, request[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"拿不動這麼多東西。\n");
		}
			
		msg("$ME將貨物「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」領取到身上。\n", me, 0, 1);
				
		if( !cloneob->no_amount() )
		{
			cloneob->set_amount(request[i]["amount"]);
			cloneob->move(me);
		}
		else
			cloneob->move(me, request[i]["amount"]);
		
		remove_request(i);
	}
	else
		return tell(me, "請輸入正確的收貨指令(help receive)。\n");
}

void do_support(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	mapping product_info;
	mapping products;

	if( !arg )
		return tell(me, "請輸入正確的指令(help support)。\n");

	// 運輸到某個地點
	if( sscanf(arg, "%d from %d %d,%d", i, num, x, y) == 4 )
	{
		int myamount;
		
		if( i < 1 || i > sizeof(request) )
			return tell(me, "請輸入正確的編號。\n");

		i--;

		if( request[i]["status"] == REQUEST_STATUS_FINISHED )
			return tell(me, "此項訂單已經結案。\n");

		if( request[i]["buyer"] == me->query_id(1) )
			return tell(me, "這是"+pnoun(2, me)+"所提出的訂單，若要取消此筆訂單請參考 help request。\n");
			
		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
		{
			if( objectp(ob = load_object("/std/module/product/"+request[i]["basename"]+"_module")) )
				product_info = ob->query_product_info();
			else
				error("發生錯誤。\n");
		}

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "這座城市並沒有第 "+num+" 衛星都市。\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "座標"+loc_short(loc)+"處並沒有任何建築物。\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"並不是"+pnoun(2, me)+"的建築物。\n");
		
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
				return tell(me, delivery->query_room_name()+"沒有辦法提供任何貨品。\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"沒有辦法提供任何貨品。\n");
		}

		myamount = delivery->query_module_file()->query_object_amount(delivery_master, "products", request[i]["basename"], "訂單供貨");
		
		if( myamount < request[i]["amount"] )
			return tell(me, delivery->query_room_name()+"的「訂單供貨」分類裡沒有這麼多的相關貨品。\n");

		if( product_info )
		{
			products = delivery->query_module_file()->query_objects(delivery_master, "products", request[i]["basename"], "訂單供貨");
			
			foreach(string basename, int count in products)
			{
				if( count < request[i]["amount"] ) continue;

				me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
		
				ob = load_object(basename);

				msg("$ME從"+query("short", delivery)+loc_short(loc)+"提供「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」，收取 "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"。\n", me, 0, 1);
				CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"結案"NOR"："+me->query_idname()+"供給 "+capitalize(request[i]["buyer"])+" 第 "+(i+1)+" 編號的訂單，貨物名：「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」。\n");

				delivery->query_module_file()->output_object(delivery_master, "products", basename, request[i]["amount"]);
				
				request[i]["status"] = REQUEST_STATUS_FINISHED;
				request[i]["supporter"] = me->query_idname();
				request[i]["basename"] = basename;

				save();
		
				delivery_master->save();	
				
				return;	
			}

			return tell(me, delivery->query_room_name()+"的「訂單供貨」分類裡沒有這麼多的相關貨品。\n");
		}
		else
		{ 
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
	
			msg("$ME從"+query("short", delivery)+loc_short(loc)+"提供「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」，收取 "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"結案"NOR"："+me->query_idname()+"供給 "+capitalize(request[i]["buyer"])+" 第 "+(i+1)+" 編號的訂單，貨物名：「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」。\n");

			delivery->query_module_file()->output_object(delivery_master, "products", request[i]["basename"], request[i]["amount"]);
			
			request[i]["status"] = REQUEST_STATUS_FINISHED;
			request[i]["supporter"] = me->query_idname();
			
			save();
	
			delivery_master->save();
		}
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "請輸入正確的編號。\n");
		
		i--;

		if( request[i]["status"] == REQUEST_STATUS_FINISHED )
			return tell(me, "此項訂單已經結案。\n");
	
		foreach(ob in filter_array(all_inventory(me), (: base_name($1) == $(request)[$(i)]["basename"] || $1->query_module() == $(request)[$(i)]["basename"] :)))
		{
			if( ob->is_keeping() )
			{
				tell(me, pnoun(2, me)+"必須先取消"+ob->query_idname()+"的保留狀態。\n");
				continue;
			}
	
			if( ob->query_amount() < request[i]["amount"] )
			{
				tell(me, ob->query_idname()+"的數量不足。\n");
				continue;
			}
				
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
	
			msg("$ME提供「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」，收取 "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"結案"NOR"："+me->query_idname()+"供給 "+capitalize(request[i]["buyer"])+" 第 "+(i+1)+" 編號的訂單，貨物名：「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」。\n");

			request[i]["status"] = REQUEST_STATUS_FINISHED;
			request[i]["supporter"] = me->query_idname();
			request[i]["basename"] = base_name(ob);	

			destruct(ob, request[i]["amount"]);
	
			save();
			
			return;
		}
			
		return tell(me, pnoun(2, me)+"的身上沒有可供貨的物品或是數量不足。\n");
	}
	else
		return tell(me, "請輸入正確的供貨指令(help support)。\n");
}

void do_request(object me, string arg)
{
	int num, i;
	int amount;
	mixed price;
	string name;
	object ob, product_module;
	mapping request_data = allocate_mapping(0);
	string money_unit = MONEY_D->query_default_money_unit();
	mapping product_info;

	if( !arg )
		return tell(me, "請輸入正確的指令(help request)。\n");
		
	if( sscanf(arg, "%d %s per %s", amount, name, price) != 3 )
	{
		if( sscanf(arg, "%d cancel", num) == 1 )
		{
			i = num - 1;
			
			if( i < 0 || i >= sizeof(request) )
				return tell(me, "沒有編號 "+num+" 的訂單。\n");
				
			if( request[i]["buyer"] != me->query_id(1) )
				return tell(me, "編號 "+num+" 的訂單並不是"+pnoun(2, me)+"所提出的，無法取消。\n");
			
			if( request[i]["status"] != REQUEST_STATUS_GOING )
				return tell(me, "編號 "+num+" 的訂單已經有人供貨了，無法取消。\n");
	
			if( objectp(ob = load_object("/std/module/product/"+request[i]["basename"]+"_module")) )
					product_info = ob->query_product_info();
			else if( !objectp(ob = load_object(request[i]["basename"])) )
					error("發生錯誤。\n");

			tell(me, pnoun(2, me)+"取消了編號 "+num+" 的訂單。\n");

			if( product_info )
				CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"取消"NOR"："+me->query_idname()+"取消編號 "+num+" 的工業產品「"+product_info["name"]+"("+capitalize(product_info["id"])+")」訂單。\n");
			else	
				CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"取消"NOR"："+me->query_idname()+"取消編號 "+num+" 的「"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"」訂單。\n");
	
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
			
			remove_request(i);
	
			return;
		}
	
		return tell(me, "請輸入正確的指令(help request)。\n");
	}

	price = to_int(big_number_check(price));

	if( price < 1 || price > 9999999999999 )
		return tell(me, "單價範圍必須在 1 至 "+9999999999999+" 之間。\n");
	
	if( amount < 1 || amount > 100000 )
		return tell(me, "數量範圍必須在 1 至 "+100000+" 之間。\n");
	
	if( !objectp(product_module = load_object("/std/module/product/"+replace_string(lower_case(name), " ", "_")+"_module.c"))  && !objectp(ob = present(name, me)) )
		return tell(me, pnoun(2, me)+"的身上並沒有「"+name+"」這項物品。\n");

	if( objectp(ob) && query("flag/no_give", ob) )
		return tell(me, ob->query_idname()+"無法設定為訂單。\n");
		
	if( !me->spend_money(money_unit, amount * price + 10000) )
		return tell(me, pnoun(2, me)+"的現金不足 "HIY+money(money_unit, amount * price + 10000)+NOR"。\n");

	request_data["buyer"] = me->query_id(1);
	request_data["amount"] = amount;
	request_data["price"] = price;
	request_data["time"] = time();
	
	if( objectp(product_module) )
	{
		product_info = product_module->query_product_info();

		request_data["basename"] = replace_string(lower_case(name), " ", "_");
	
		msg("$ME花費 "+HIY+money(money_unit, amount * price)+NOR" 的訂單金額與 "+HIY+money(money_unit, 10000)+NOR+" 的手續費提出訂單。\n", me, 0, 1);
		CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"提出"NOR"："+me->query_idname()+"提出工業產品「"+product_info["name"]+"("+capitalize(product_info["id"])+")」、數量 "HIG+amount+NOR"、單價 "HIY+money(money_unit, price)+NOR" 的訂單。\n");
	}
	else
	{
		request_data["basename"] = base_name(ob);

		msg("$ME花費 "+HIY+money(money_unit, amount * price)+NOR" 的訂單金額與 "+HIY+money(money_unit, 10000)+NOR+" 的手續費提出訂單。\n", me, 0, 1);
		CHANNEL_D->channel_broadcast("auc", HIC"訂單"NOR CYN"提出"NOR"："+me->query_idname()+"提出「"+ob->query_idname()+"」、數量 "HIG+amount+NOR"、單價 "HIY+money(money_unit, price)+NOR" 的訂單。\n");
	}
	
	add_request(request_data);
}

void heart_beat()
{
	int size = sizeof(auction);
	object ob;
	object seller;
	string money_unit = MONEY_D->query_default_money_unit();

	for(int i=0;i<size;i++)
	{
		// 超過 14 天未領取的物品由系統刪除
		if( auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 14*24*60*60 )
		{
			ob = load_object(auction[i]["basename"]);

			if( !objectp(ob) ) continue;

			CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"充公"NOR"："+"編號 "+(i+1)+" 號的流標品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」超過 14 天無人領回，物品充公。\n");

			auction[i] = 0;
		}
		// 超過 7 天未領取的物品宣告流標
		else if( (auction[i]["status"] == 1 || auction[i]["status"] == 3) && auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 7*24*60*60 )
		{
			ob = load_object(auction[i]["basename"]);

			if( !objectp(ob) ) continue;

			auction[i]["status"] = 2;
			auction[i]["finish_time"] = time();
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, pnoun(2, seller)+"的拍賣品超過 7 天無人領取，開放拍賣者領回。\n");
				
			CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"流標"NOR"："+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」超過 7 天無人領取，宣告流標。\n");
		}
		else if( auction[i]["minute"] && !(--auction[i]["minute"]) )
		{
			ob = load_object(auction[i]["basename"]);
			
			if( !objectp(ob) ) continue;

			if( !sizeof(auction[i]["auction_data"]) )
			{
				auction[i]["status"] = 2;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"流標"NOR"："+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」宣告流標。\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"的拍賣品已經流標，請自行領回。\n");
					
				if( auction[i]["seller"] == "" )
					auction[i] = 0;
			}
			else
			{
				refresh_top_buyer(i);
				
				if( auction[i]["seller"] != "" )
					MONEY_D->earn_money(auction[i]["seller"], money_unit, auction[i]["top_price"]);

				auction[i]["status"] = 1;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"結標"NOR"："+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」以 "HIY+money(money_unit, auction[i]["top_price"])+NOR" 價格結標。\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"的拍賣品成功結標，收取 "HIY+money(money_unit, auction[i]["top_price"])+NOR"。\n");
			}
		}
		else if( auction[i]["minute"] == 30 || auction[i]["minute"] == 15 || auction[i]["minute"] == 10 || auction[i]["minute"] == 5 || auction[i]["minute"] == 1 )
		{
			ob = load_object(auction[i]["basename"]);
			
			CHANNEL_D->channel_broadcast("auc", HIB"拍賣"NOR BLU"訊息"NOR"："+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」即將在 "+auction[i]["minute"]+" 分鐘後結標，目前出價 "HIY+money(money_unit, auction[i]["top_price"])+NOR"。\n");
		}
	}
	
	auction -= ({ 0 });
}

int remove()
{
	return save();
}

void fix_auction()
{

}

// 強迫流標
void force_give_up_auction(int i)
{
	object ob = load_object(auction[i]["basename"]);
	CHANNEL_D->channel_broadcast("auc", HIG"拍賣"NOR GRN"流標"NOR"："+"編號 "+(i+1)+" 號的拍賣品「"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"」宣告流標。\n");	
	
	auction[i]["status"] = 2;
	auction[i]["finish_time"] = time();
	auction[i]["auction_data"] = allocate(0);
}

void fix_request()
{

}

void create()
{
	if( !restore_object(DATA) )
	{
		auction = allocate(0);

		save_object(DATA);
	}
	
	if( !arrayp(request) )
		request = allocate(0);

	set_heart_beat(60);
}

string query_name()
{
	return "拍賣系統(AUCTION_D)";
}
