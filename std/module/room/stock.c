/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stock.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-31
 * Note   : 證券交易所
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>

inherit ROOM_ACTION_MOD;

#define PROCEDURE_FUND	0.02		// 2/100 的手續費
#define STOCK_VALUE	10000		// 一張股票價值

varargs string stockvalue_change_description(float change, string type)
{
	string bgcolor;
	
	switch(type)
	{
		case "t3g2":
			bgcolor = BGRN;
			break;
		case "t3r2":
			bgcolor = BRED;
			break;
		default:
			bgcolor = "";
			break;
	}

	change = to_float(change);

	if( change > 0 )
		return bgcolor+sprintf(HIR"▲%5.2f", change);
	else if( change < 0 )
		return bgcolor+sprintf(HIG"▼%5.2f", -change);
	else
		return bgcolor+sprintf(HIW"  %5.2f", change);
}


void do_list(object me, string arg)
{
	int count;
	mapping stocks = STOCK_D->query_stocks();
	int *stock_sort = sort_array(keys(stocks), 1);
	mapping data;
	string *msg = allocate(0);
	int min, max;
	float percent = PROCEDURE_FUND - me->query_skill_level("stock")/10000.;

	if( arg )
	{
		if( sscanf(arg, "%d %d", min, max) != 2 )
			sscanf(arg, "%d", min);
	}
	
	msg = ({"代號  股票名稱 今日收盤 今日漲跌 漲跌幅度 開盤價 最高價 最低價 成交張數 \n"});
	msg += ({"─────────────────────────────────────────────\n"});

	foreach(int stock in stock_sort)
	{
		count++;

		if( max )
		{
			if( stock < min || stock > max ) continue;
		}
		else
		{
			if( min && stock != min ) continue;
		}

		data = stocks[stock];
		
		if( !data["股票名稱"] ) continue;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-8s "HIY"%8.2f"NOR" %-8s %7.2f%% "NOR WHT"%6.2f "HIB"%6.2f %6.2f "NOR BYEL HIY"%s\n"NOR,
			stock,
			data["股票名稱"], 
			data["收盤"], 
			stockvalue_change_description(data["漲跌"], data["狀態"]), 
			data["漲跌幅"],
			data["開盤"],
			data["最高"],
			data["最低"],
			data["成交張數"]
		)});
	}
	msg += ({"─────────────────────────────────────────────\n"});
	msg += ({MAG"共 "HIM+count+NOR MAG" 檔上市上櫃股票，"YEL"股票單位為 "HIY"10,000"NOR YEL" 元/張，"NOR RED"交易手續費為 "HIR+sprintf("%.2f%%", percent*100.)+NOR RED"，"HIC"收盤資料一天更新一次\n"NOR});
	msg += ({WHT"因數據並非即時更新，因此週一至週五的早上九點至下午四點之間的盤中與資料更新期間禁止交易\n\n"NOR});

	return me->more(implode(msg, ""));
}

void confirm_buystock(object me, int num, int totalcost, int new_amount, float new_value, float cur_value, int amount, string stock_name, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "取消購入股票。\n");
		return me->finish_input();
	}

	if( !me->spend_money(MONEY_D->query_default_money_unit(), totalcost) )
	{
		tell(me, pnoun(2, me)+"身上的錢不足 "+money(MONEY_D->query_default_money_unit(), totalcost)+"。\n");
		return me->finish_input();
	}
	
	set("stock/"+num+"/amount", new_amount, me);
	set("stock/"+num+"/value", new_value, me);
		
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", cur_value)+NOR" 股價購入「"HIW+stock_name+NOR"」股票 "+NUMBER_D->number_symbol(amount)+" 張。");
	me->finish_input();
	
	TOP_D->update_top_rich(me);
	me->save();
}

void do_buystock(object me, string arg)
{
	mapping stocks = STOCK_D->query_stocks();
	int num, amount;
	float old_value;
	float new_value;
	float cur_value;
	int old_amount;
	int new_amount;
	int cost;
	int cost_extra;
	int *nowtime = TIME_D->query_realtime_array();
	float percent;

	return tell(me, "股票系統目前無法處理重新掛牌、除權、除息等問題，短期內將無法購買股票。\n");

	if( !wizardp(me) && nowtime[2] >= 1 && nowtime[2] <= 5 && nowtime[1] >= 9 && nowtime[1] <= 16 )
		return tell(me, "週一至週五的早上九點至下午四點禁止交易。\n");

	if( STOCK_D->query_last_update_time() < time() - 24*60*60 )
		return tell(me, "股票資料尚未更新，無法進行交易。\n");

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式，例：buy 1101 1。\n");

	if( amount < 1 )
		return tell(me, pnoun(2, me)+"至少必須購買一張股票。\n");

	if( !mapp(stocks[num]) || !stocks[num]["股票名稱"] )
		return tell(me, "沒有 "+num+" 這一檔股票。\n");

	if( stocks[num]["狀態"] == "t3r2" )
		return tell(me, "這檔股票已經漲停，無法再買了。\n");

	old_amount = query("stock/"+num+"/amount", me);
	new_amount = amount + old_amount;

	if( new_amount > 1000000 )
		return tell(me, "每檔股票最多只能購買 1,000,000 張。\n");
		
	old_value = to_float(query("stock/"+num+"/value", me));
	cur_value = stocks[num]["收盤"];
	new_value = (old_value * old_amount + cur_value * amount) / (amount + old_amount);

	percent = PROCEDURE_FUND - me->query_skill_level("stock")/10000.;

	cost = to_int(to_int(cur_value*100) * STOCK_VALUE * amount / 100);
	cost_extra = to_int(cost * percent);

	if( cost + cost_extra < 1 )
		return tell(me, "資料計算錯誤，請通知巫師處理。\n");

	tell(me,
		WHT"股票名稱      "NOR HIW+num+" "+stocks[num]["股票名稱"]+NOR"\n"
		"───────────────────\n"
		YEL"今日收盤價格  "HIY+sprintf("%.2f", cur_value)+"\n"NOR
		GRN"欲購股票張數  "HIG+NUMBER_D->number_symbol(amount)+"\n"NOR
		GRN"目前持股價格  "HIG+sprintf("%.2f", old_value)+"\n"NOR
		GRN"目前持股張數  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"平均之後價格  "HIC+sprintf("%.2f", new_value)+"\n"NOR
		CYN"欲購股票總值  "HIC+money(MONEY_D->query_default_money_unit(), cost)+"\n"NOR
		CYN"交易手續費用  "HIC+money(MONEY_D->query_default_money_unit(), cost_extra)+HIR"("+sprintf("%.2f", percent*100.)+"%)\n"NOR
		CYN"購入總共花費  "HIC+money(MONEY_D->query_default_money_unit(), cost + cost_extra)+"\n"NOR
		"───────────────────\n"
		HIY"是否確定購入股票?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_buystock, me, num, cost + cost_extra, new_amount, new_value, cur_value, amount, num+" "+stocks[num]["股票名稱"] :));
}

void confirm_sellstock(object me, int num, int totalearn, int new_amount, float old_value, float cur_value, int amount, string stock_name, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "取消販賣股票。\n");
		return me->finish_input();
	}
	
	me->earn_money(MONEY_D->query_default_money_unit(), totalearn);
	
	if( new_amount <= 0 )
		delete("stock/"+num, me);
	else
		set("stock/"+num+"/amount", new_amount, me);
	
	if( cur_value == old_value )
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", cur_value)+NOR" 股價售出「"HIW+stock_name+NOR"」股票 "+NUMBER_D->number_symbol(amount)+" 張。");
	else if( cur_value > old_value )
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", cur_value)+NOR" 股價售出「"HIW+stock_name+NOR"」股票 "+NUMBER_D->number_symbol(amount)+" 張，獲利 "HIY+money(MONEY_D->query_default_money_unit(), to_int(to_int((cur_value-old_value)*100)*STOCK_VALUE/100*amount))+NOR"。");
	else
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", cur_value)+NOR" 股價售出「"HIW+stock_name+NOR"」股票 "+NUMBER_D->number_symbol(amount)+" 張，認賠 "HIY+money(MONEY_D->query_default_money_unit(), to_int(to_int((old_value-cur_value)*100)*STOCK_VALUE/100*amount))+NOR"。");
	
	me->finish_input();
	TOP_D->update_top_rich(me);
	me->save();
}

void do_sellstock(object me, string arg)
{
	mapping stocks = STOCK_D->query_stocks();
	int num, amount;
	int old_amount;
	int new_amount;
	float cur_value;
	float old_value;
	int earn;
	int *nowtime = TIME_D->query_realtime_array();

	if( !wizardp(me) && nowtime[2] >= 1 && nowtime[2] <= 5 && nowtime[1] >= 9 && nowtime[1] <= 16 )
		return tell(me, "週一至週五的早上九點至下午四點禁止交易。\n");

	if( STOCK_D->query_last_update_time() < time() - 24*60*60 )
		return tell(me, "股票資料尚未更新，無法進行交易。\n");

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式，例：sell 1101 1。\n");

	if( amount < 1 )
		return tell(me, pnoun(2, me)+"至少必須出售一張股票。\n");
	
	if( !mapp(stocks[num]) || !stocks[num]["股票名稱"] )
		return tell(me, "沒有 "+num+" 這一檔股票。\n");

	if( stocks[num]["狀態"] == "t3g2" )
		return tell(me, "這檔股票已經跌停，無法再賣了。\n");

	old_amount = query("stock/"+num+"/amount", me);
	new_amount = old_amount - amount;
	
	old_value = to_float(query("stock/"+num+"/value", me));
	cur_value = stocks[num]["收盤"];

	if( old_amount < 1 )
		return tell(me, pnoun(2, me)+"手上並沒有 "+num+" 這檔股票。\n");

	if( new_amount < 0 )
		return tell(me, "這檔股票"+pnoun(2, me)+"手上只有 "+old_amount+" 張。\n");

	earn = to_int(to_int(cur_value*100) * amount * STOCK_VALUE / 100);
	
	if( earn < 1 )
		return tell(me, "資料計算錯誤，請通知巫師處理。\n");

	tell(me,
		WHT"股票名稱      "NOR HIW+num+" "+stocks[num]["股票名稱"]+NOR"\n"
		"───────────────────\n"
		YEL"今日收盤價格  "HIY+sprintf("%.2f", cur_value)+"\n"NOR
		GRN"目前持股價格  "HIG+sprintf("%.2f", old_value)+"\n"NOR
		GRN"目前持股利潤  "HIG+sprintf("%.2f%%", (cur_value - old_value) * 100. / old_value)+"\n"NOR
		GRN"欲售股票張數  "HIG+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"售出總共獲得  "HIC+money(MONEY_D->query_default_money_unit(), earn)+"\n"NOR
		"───────────────────\n"
		HIY"是否確定售出股票?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_sellstock, me, num, earn, new_amount, old_value, cur_value, amount, num+" "+stocks[num]["股票名稱"] :));
}

/*
void do_clearstock(object me, string arg)
{
	mapping stockdata = query("stock", me);
	
	if( !sizeof(stockdata) )
		return tell(me, pnoun(2, me)+"的手中未持有任何股票。\n");

	foreach( string enterprise, mapping data in stockdata )	
	{
		if( !ENTERPRISE_D->enterprise_exists(enterprise) )
		{
			tell(me, "清除 "+enterprise+" 的股票資料。\n");
			delete("stock/"+enterprise, me);
			me->save();
		}
	}
	
	tell(me, "完成下市股票清理。\n");
}
*/

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: NOR HIW"交易所大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    證券交易的地方。
HELP,

"list":
@HELP
顯示股市資訊
list		列出所有臺灣上市上櫃股票市場資料
list 1101	列出代號 1101 股票的資料
list 1100 1300  列出代號 1100 至 1300 之間所有的股票資料
HELP,

"buy":
@HELP
買入股票的指令
buy 1101 300	買入 1101(台泥) 股票 300 張
HELP,

"sell":
@HELP
賣出股票的指令
sell 1101 100	賣出 1101(台泥) 股票 100 張
HELP,

"clear":
@HELP
請除已下市股票
clear		清除已下市股票資料(廢紙)
HELP,

			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"buy" : (: do_buystock :),
				"sell" : (: do_sellstock :),
				//"clear" : (: do_clearstock :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIG"證券"NOR GRN"交易所"NOR

	// 開張此建築物之最少房間限制
	,2

	// 城市中最大相同建築數量限制(0 代表不限制)
	,5

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,ENTERPRISE

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,75000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,0
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
