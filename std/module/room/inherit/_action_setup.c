/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _setup_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-23
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

void refresh_class(object env, string database);

void do_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	object master = env->query_master();
	if( !arg )
		return tell(me, pnoun(2, me)+"想要設定什麼東西？\n");
		
	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"並不是這家店的老闆。\n");
		
	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		// 設定允許販賣項目
		case "sell":
		{
			int percent, number;
			object ob;
			string basename, price, option, *worldproductlist;
			int amount;
			
			worldproductlist = PRODUCT_D->query_world_product();

			if( !value )
			{			
				string msg;
				
				msg  = "系統商品列表\n";
				msg += "─────────────────────────────────────\n";
				msg += "編號 物品\n";
				msg += "─────────────────────────────────────\n";

				foreach(basename in worldproductlist)
				{
					// 把該位置空下來
					if( !objectp(ob = load_object(basename)) )
					{
						msg += sprintf("%s%-5s無法載入物品\n"NOR, 
							!(number % 2) ? "" : WHT,
							++number+".");
						
						continue;
					}
					
					msg += sprintf("%s%-5s"NOR"%-34s\n"NOR, 
						!(number % 2) ? "" : WHT,
						++number+".", 
						ob->query_idname());
				}

				msg += "─────────────────────────────────────\n";
				return me->more(msg+"\n");
			}
			
			if( sscanf(value, "%s with %s for %d", option, price, amount) == 3 || sscanf(value, "%s with %s", option, price) == 2 )
			{
				if( option == "all" )
				{
					if( price[<1] != '%' )
						return tell(me, "設定收購物品時只能設定收購百分比。\n");

					percent = to_int(price[0..<2]);
					
					if( percent < 0 || percent > 999 )
						return tell(me, "價格比例只能介於 0% 至 999%。\n");
					
					if( amount )
					{
						if( amount < 0 || amount > 10000000 )
							return tell(me, "數量不能低於 0 或高於 10000000。\n");
							
						delete("setup/sell", master);
						set("setup/sell/all/amount", amount, master);
					}
					else
						delete("setup/sell", master);
					

					set("setup/sell/all/percent", percent, master);
						
					if( amount )
						msg("$ME將所有物品的收構價格比設定為 "+percent+"%，總收購數量為 "+amount+"。\n", me, 0, 1);
					else
						msg("$ME將所有物品的收構價格比設定為 "+percent+"%，總收購數量為無限。\n", me, 0, 1);
				}
				else
				{
					// 商品列表
					if( sscanf(option, "%d", number) == 1 )
					{
						mapping products = query("products", master);
						array productlist = implode(values(products), (:$1+$2:))||allocate(0);
						int size = sizeof(productlist);
						
						for(int i=0;i<size;i+=2)
						{
							if( i/2+1 == number )
							{
								basename = productlist[i];
								ob = load_object(basename);
								basename = replace_string(basename, "/", "#");
							}
						}
						
						if( !basename )
							return tell(me, "請輸入正確的商品編號。\n");
					}
					else if( sscanf(option, "w%d", number) == 1 )
					{			
						if( number <= 0 || number > sizeof(worldproductlist) )
							return tell(me, "請輸入正確的系統物品編號。\n");
					
						basename = worldproductlist[number-1];
						ob = load_object(basename);
						basename = replace_string(basename, "/", "#");
					}
					else
						return tell(me, "請輸入正確的參數。\n");
					
					if( price[<1] != '%' )
						return tell(me, "設定收購物品時只能設定收購百分比。\n");

					percent = to_int(price[0..<2]);
					
					if( percent < 0 || percent > 999 )
						return tell(me, "價格比例只能介於 0% 至 999%。\n");
							
					price = count(count(query("value", ob), "*", percent), "/", 100);

					if( !(price = big_number_check(price)) )
						return tell(me, "請輸入正確的價格。\n");
						
					if( count(price, "<", 0) || count(price, ">", "99999999999") )
						return tell(me, "價格不能低於 0 或高於 99999999999。\n");
							
					percent = to_int(count(count(price, "*", 100), "/", query("value", ob)));
						
					if( percent < 0 || percent > 999 )
						return tell(me, "價格比例只能介於 0% 至 999%。\n");	

					delete("setup/sell/"+basename, master);

					if( amount )
					{
						if( amount < 0 || amount > 10000000 )
							return tell(me, "數量不能低於 0 或高於 10000000。\n");
						
						set("setup/sell/"+basename+"/amount", amount, master);
					}
					
					set("setup/sell/"+basename+"/percent", percent, master);

					if( amount )
						msg("$ME將"+ob->query_idname()+"的收購比設定為 "+percent+"%，價格為 "+price+"，總收購數量為 "+amount+"。\n", me, 0, 1);
					else
						msg("$ME將"+ob->query_idname()+"的收購比設定為 "+percent+"%，價格為 "+price+"，總收購數量為無限。\n", me, 0, 1);
				}
			}
			
			else if( value == "all -d" )
			{
				delete("setup/sell", master);
				msg("$ME停止收購所有物品。\n", me, 0, 1);
			}
			else if( sscanf(value, "%d -d", number) == 1 )
			{
				mapping products = query("products", master);
				array productlist = implode(values(products), (:$1+$2:))||allocate(0);
				int size = sizeof(productlist);
				
				for(int i=0;i<size;i+=2)
				{
					if( i/2+1 == number )
					{
						basename = productlist[i];
						ob = load_object(basename);
						basename = replace_string(basename, "/", "#");
						delete("setup/sell/"+basename, master);
						set("setup/sell/"+basename+"/nosell", 1, master);
						msg("$ME停止收購"+ob->query_idname()+"。\n", me, 0, 1);	
						
						break;
					}
				}
				
				if( !basename )
					return tell(me, "請輸入正確的商品編號。\n");
				
				
			}
			else if( sscanf(value, "w%d -d", number) == 1 )
			{
				if( number <= 0 || number > sizeof(worldproductlist) )
					return tell(me, "請輸入正確的系統物品編號。\n");
					
				basename = worldproductlist[number-1];
				ob = load_object(basename);
				basename = replace_string(basename, "/", "#");
				delete("setup/sell/"+basename, master);
				set("setup/sell/"+basename+"/nosell", 1, master);
				msg("$ME停止收購"+ob->query_idname()+"。\n", me, 0, 1);
			}
			else
				return tell(me, "請輸入正確的參數。\n");
			
			master->save();
			break;
		}
		case "price":
		{
			int i, j, size;
			string unit, money, basename;
			mapping products = query("products", master);
			array productlist;
			int amount;
			
			if( !mapp(products) )
				return tell(me, "這裡並沒有販賣任何商品。\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"想要設定哪個商品的價錢？\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "這裡並沒有販賣任何商品。\n");

			if( sscanf( value, "%d %s", i, money ) != 2 || money == "" )
				return tell(me, pnoun(2, me)+"想要設定哪一個編號商品的價錢？\n");
				
			unit = env->query_money_unit();
			
			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = to_int(productlist[j+1]);
			
				if( j/2+1 == i )
				{
					object product = load_object(basename);

					if( !objectp(product) )
					{
						error(basename+" 商品資料錯誤。");
						continue;
					}

					if( money[<1]=='%' && big_number_check(money[0..<2]) )
						money = count(count(query("value", product), "*", money[0..<2]), "/", 100);
						
					if( !big_number_check(money) || count(money, ">", "9999999999") || count(money, "<=", 0) )
						return tell(me, "商品價格必須介於 1 - 9999999999 之間。\n");
									
					set("setup/price/"+replace_string(basename, "/", "#"), money, master);
					msg("$ME將"+product->query_idname()+"的價錢調整為"+HIY+QUANTITY_D->money_info(unit, money)+NOR" ("+count(count(money,"*",100),"/",query("value", product))+"%)。\n", me, 0, 1);

					master->query_module_file()->calculate_master_data(master, basename);
					master->save();
					return;
				}
			}
			
			return tell(me, "請輸入正確的編號。\n");
		}
		case "class":
		{
			int i, j, size;
			string classname, basename;
			mapping products = query("products", master);
			array productlist;
			int amount;
			
			if( !mapp(products) )
				return tell(me, "目前沒有商品可以設定。\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"想要設定哪種商品的分類？\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "這裡並沒有販賣任何商品。\n");

			if( sscanf( value, "%d %s", i, classname ) != 2 )
				return tell(me, pnoun(2, me)+"想要將哪一個編號的商品設定分類？\n");
			
			classname = remove_ansi(classname);
			
			if( !is_chinese(classname) || strlen(classname) < 2 || strlen(classname) > 10 )
				return tell(me, "分類名稱必須是介於一到五個的中文字。\n");

			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = to_int(productlist[j+1]);
			
				if( j/2+1 == i )
				{
					object product = load_object(basename);
					
					if( !objectp(product) )
					{
						error(basename+" 商品資料錯誤。");
						continue;
					}

					switch(classname)
					{
						case "展示":
							delete("setup/class/"+replace_string(basename, "/", "#"), master);
							break;
						case "促銷":
						{
							mapping setupclass = query("setup/class", master);
						
							if( mapp(setupclass) )
							foreach(string f, string c in setupclass)
							{
								if( c == "促銷" )
								{
									delete("setup/class/"+f, master);
									refresh_class(master, "products");
									master->query_module_file()->calculate_master_data(master, replace_string(f, "#", "/"));
								}
							}

							if( query("badsell", product) )
								return tell(me, "這種商品無法列為促銷類商品。\n");							
							
							set("setup/class/"+replace_string(basename, "/", "#"), classname, master);
							break;
						}
						default:
							set("setup/class/"+replace_string(basename, "/", "#"), classname, master);
							break;
					}
					msg("$ME將"+product->query_idname()+"的分類設定為"+HIY+classname+NOR"。\n", me, 0, 1);
					refresh_class(master, "products");

					master->query_module_file()->calculate_master_data(master, basename);
					master->save();
					return;
				}
			}
			
			return tell(me, "請輸入正確的編號。\n");
		}
		case "mode":
		{
			if( query("mode", master) )
			{
				delete("mode", master);
				msg("$ME將此商店轉換為「營業模式」。\n", me, 0, 1);
			}
			else
			{
				set("mode", 1, master);
				msg("$ME將此商店轉換為「管理模式」。\n", me, 0, 1);
			}
			master->save();
			break;
		}
		default:
			return tell(me, "請輸入正確的設定項目(sell, price, class, mode)。\n");
	}
	
}


void do_warehouse_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	object master = env->query_master();

	if( !arg )
		return tell(me, pnoun(2, me)+"想要設定什麼東西？\n");
		
	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"並不是這間倉庫的擁有者。\n");
		
	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		case "class":
		{
			int i, j, size;
			string classname, basename;
			mapping products = query("products", master);
			array productlist;
			int amount;
			
			if( !mapp(products) )
				return tell(me, "目前沒有物品可以設定分類。\n");

			productlist = implode(values(products), (:$1+$2:))||allocate(0);
			
			if( !value )
				return tell(me, pnoun(2, me)+"想要設定哪種物品的分類？\n");
			
			size = sizeof(productlist);
			
			if( !size )
				return tell(me, "這裡並沒有販賣任何物品。\n");

			if( sscanf( value, "%d %s", i, classname ) != 2 )
				return tell(me, pnoun(2, me)+"想要將哪一個編號的物品設定分類？\n");
			
			classname = remove_ansi(classname);
			
			if( !is_chinese(classname) || strlen(classname) < 2 || strlen(classname) > 10 )
				return tell(me, "分類名稱必須是介於一到五個的中文字。\n");

			for(j=0;j<size;j+=2)
			{
				basename = productlist[j];
				amount = to_int(productlist[j+1]);
				
				if( j/2+1 == i )
				{
					object product = load_object(basename);
				
					if( !objectp(product) )
					{
						error(basename+" 商品資料錯誤。");
						continue;
					}

					if( classname == "展示")
						delete("setup/class/"+replace_string(basename, "/", "#"), master);
					else
						set("setup/class/"+replace_string(basename, "/", "#"), classname, master);

					msg("$ME將"+product->query_idname()+"的分類設定為"+HIY+classname+NOR"。\n", me, 0, 1);
					refresh_class(master, "products");

					master->save();
					return;
				}
			}
			
			return tell(me, "請輸入正確的編號。\n");
		}

		default:
			return tell(me, "請輸入正確的設定項目(class)。\n");
	}
	
}