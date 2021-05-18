/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _list_action.c
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

void output_object(object env, string database, string basename, int amount);

// 列出商品列表
void do_list(object me, string arg, string database, int capacity)
{
	int i, j, size, average_desire, hot, is_trading_post;
	object env = environment(me)->query_master();
	object ob;
	string msg, popproduct, desirehint;
	string setprice, pricepercent, sell;
	mixed value;
	mapping obs;
	string *shelfsort, shelf, totalamount, replacename;
	array data;
	string demand;
	string city = env->query_city();
	int num = env->query_city_num();
	string money_unit = MONEY_D->city_to_money_unit(city);
	
	// 若是物流中心
	if( env->query_building_type() == "trading_post" )
	{
		is_trading_post = 1;
		obs = fetch_variable("allowable_trade");
	}
	else
	{
		is_trading_post = 0;
		obs = query(database, env);
	}
	
	if( !sizeof(obs) )
		return tell(me, query("short", env)+"目前沒有任何物品。\n");
	
	popproduct = SHOPPING_D->query_pop_product();

	msg =  NOR WHT"─────────────────────────────────────────\n"NOR;
	msg += HIW    "編號 商品"+(query("mode", env)?HIR"(管理模式)"NOR:HIC"(營業模式)"NOR)+"                     人氣 數量     單價($"+money_unit+")   價格比 "+(!is_trading_post?"收購 需求":"")+"\n"NOR;
	msg += NOR WHT"─────────────────────────────────────────\n"NOR;
	
	shelfsort = keys(obs);
	
	average_desire = query("shopping/average_desire", env);

	foreach( shelf in shelfsort )
	{
		switch(shelf)
		{
			case 0:		msg += HIC"展示"NOR"\n"; break;
			case "促銷":	msg += HIY"促銷"NOR"\n"; break;
			case "內銷":	msg += HIG"內銷"NOR"\n"; break;
			default:	msg += HIG+shelf+NOR"\n"; break;
		}

		data = copy(obs[shelf]);

		size = sizeof(data);
		
		for(j=0;j<size;j+=2)
		{
			if( catch(ob = load_object(data[j]) ) )
				continue;
		
			// 檔案已經消失, 刪除資料
			if( !objectp(ob) && !file_exists(data[j]) )
			{
				output_object(env, database, data[j], -3);
				continue;
			}
			
			replacename = replace_string(data[j], "/", "#");
			
			setprice = query("setup/price/"+replacename, env);
			value = query("value", ob);
			totalamount = count(totalamount, "+", data[j+1]);

			if( !query("setup/class/"+replacename, env) )
				desirehint = "";
			else
			switch(average_desire + query("shopping/productdesire/"+replacename, env))
			{
				case -MAX_INT..0:
					desirehint = ""; break;
				case 1..30:
					desirehint = NOR WHT"*"; break;
				case 31..60:
					desirehint = NOR WHT"**"; break;
				case 61..90:
					desirehint = NOR WHT"***"; break;
				case 91..120:
					desirehint = HIW"*"NOR WHT"***"; break;
				case 121..150:
					desirehint = HIW"**"NOR WHT"***"; break;
				case 151..180:
					desirehint = HIW"***"NOR WHT"***"; break;
				case 181..210:
					desirehint = HIW"***"NOR WHT"**"BLINK"*"; break;
				case 211..240:
					desirehint = HIW"***"NOR WHT"*"BLINK"**"; break;
				case 241..270:
					desirehint = HIW"***"NOR WHT BLINK"***"; break;
				case 271..300:
					desirehint = HIW"**"BLINK"*"NOR WHT BLINK"***"; break;
				case 301..330:
					desirehint = HIW"*"BLINK"**"NOR WHT BLINK"***"; break;
				case 331..MAX_INT:
					desirehint = HIW BLINK"***"NOR WHT BLINK"***"; break;
				default:
					
					desirehint = HIR"Bug"; break;
			}
			
			if( setprice )
				pricepercent = count(count(setprice, "*", 100), "/", value||1);
			else
				pricepercent = "100";
			
			if( query("setup/sell/"+replacename+"/nosell", env) )
				sell = "";
			else if( !undefinedp(query("setup/sell/"+replacename+"/percent", env)) )
				sell = query("setup/sell/"+replacename+"/percent", env) +"%";
			else if( !undefinedp(query("setup/sell/all/percent", env)) )
				sell = query("setup/sell/all/percent", env) +"%";
			else
				sell = "";

			if( !is_trading_post )
			switch(SHOPPING_D->query_demand_rank(city, num, ob))
			{
				case 0:
					demand = BLU"█"HIB"▌"NOR;
					break;
				case 1..10:
					demand = BLU"█"HIB"▍"NOR;
					break;
				case 11..20:
					demand = BLU"█"HIB"▏"NOR;
					break;
				case 21..30:
					demand = BLU"█"NOR;
					break;
				case 31..40:
					demand = BLU"▉"NOR;
					break;
				case 41..50:
					demand = BLU"▊"NOR;
					break;
				case 51..60:
					demand = BLU"▋"NOR;
					break;
				case 61..70:
					demand = BLU"▌"NOR;
					break;
				case 71..80:
					demand = BLU"▍"NOR;
					break;
				case 81..90:
					demand = BLU"▎"NOR;
					break;
				case 91..99:
					demand = BLU"▏"NOR;
					break;
				case 100:
					demand = BLU"  "NOR;
					break;
			}
			
			if( data[j] == popproduct || (ob->is_module_product() && ob->query_module() == popproduct) )
				hot = 1;	
			else
				hot = 0;

			msg += sprintf(HIW"%-3d-"NOR" %-32s "NOR"%6s "NOR HIC"%-9s"HIY"%-13s"HIG"%5s"HIR"%5s"NOR" %-4s"NOR"%s\n", 
				++i, 
				ob->query_idname(),
				desirehint,
				(data[j+1]==-1?HIC"無限"NOR:data[j+1])+"", 
				NUMBER_D->number_symbol(setprice||value),
				(count(pricepercent, ">", 999)?">999":pricepercent)+"%",
				sell,
				!is_trading_post ? demand : "",
				(hot?HIR"Hot!"NOR:"")+(wizardp(me) ? " "+WHT+(average_desire+" / "+query("shopping/productdesire/"+replacename, env)+" / "+replacename)+NOR:"")
				);
				
			if( arg && to_int(arg) == i )
			{
				ob = new(data[j]);
				tell(me, ob->long());
				destruct(ob);
				return;
			}
		}
	}
	
	msg += NOR WHT"─────────────────────────────────────────\n"NOR;
	msg += sprintf("%-54s%28s\n", NOR WHT"共有 "HIW+i+NOR WHT" 種商品正在販賣中，容量 "+(capacity == -1 ? HIW"無盡"NOR:HIW+NUMBER_D->number_symbol(totalamount)+NOR WHT" / "HIW+NUMBER_D->number_symbol(count(capacity, "*", (sizeof(query("slave", env))+1))))+NOR, query("short", env));
	me->more(msg);
}

void do_warehouse_list(object me, string arg, string database, int capacity)
{
	int i, j, size;
	object env, ob;
	string msg, money_unit;
	string setprice;
	mapping obs;
	string *shelfsort, shelf, totalamount;
	array data;
	
	env = environment(me)->query_master();
	
	money_unit = MONEY_D->city_to_money_unit(env->query_city());
	
	// 若是物流中心
	if( env->query_building_type() == "trading_post" )
		obs = fetch_variable("allowable_trade");
	else
		obs = query(database, env);
	
	if( !sizeof(obs) )
		return tell(me, query("short", env)+"目前沒有任何物品。\n");
	
	msg =  NOR WHT"────────────────────────────────────\n"NOR;
	msg += HIW    "編號  物品                                                          數量\n"NOR;
	msg += NOR WHT"────────────────────────────────────\n"NOR;
	
	shelfsort = keys(obs);
	
	foreach( shelf in shelfsort )
	{
		if( !shelf )
			msg += HIC"展示\n"NOR;
		else
			msg += shelf + "\n";

		data = copy(obs[shelf]);

		size = sizeof(data);
		
		for(j=0;j<size;j+=2)
		{
			if( catch(ob = load_object(data[j]) ) )
				continue;
		
			// 檔案已經消失, 刪除資料
			if( !objectp(ob) && !file_exists(data[j]) )
			{
				output_object(env, database, data[j], -3);
				continue;
			}
			
			setprice = query("setup/price/"+replace_string(data[j], "/", "#"), env);
			totalamount = count(totalamount, "+", data[j+1]);

			msg += sprintf(HIW"%-4d-"NOR" %-50s%16s\n", 
				++i, 
				ob->query_idname(), 
				to_int(data[j+1])==-1?"無限":to_string(data[j+1]),
				);

			if( arg && to_int(arg) == i )
			{
				ob = new(data[j]);
				tell(me, ob->long());
				destruct(ob);
				return;
			}
		}
	}
	
	msg += NOR WHT"────────────────────────────────────\n"NOR;
	msg += sprintf("%-54s%18s\n", NOR WHT"共有 "HIW+i+NOR WHT" 種物品，容量 "+(capacity == -1 ? HIW"無盡"NOR:HIW+NUMBER_D->number_symbol(totalamount)+NOR WHT" / "HIW+NUMBER_D->number_symbol(count(capacity, "*", (sizeof(query("slave", env))+1))))+NOR, query("short", env));
	me->more(msg);
}