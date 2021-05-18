/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : futures.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-20
 * Note   : 期貨交易所
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
#include <futures.h>
#include <money.h>
#include <time.h>

inherit ROOM_ACTION_MOD;

#define MAX_LOT_PRICE			100000000000		// 最大持有部位金額限制

float fund_multiplier(int total_gain)
{
	float multiplier = 1. + ((total_gain / 100000000000)/10.);
	
	if( multiplier < 0. )
		multiplier = 0.;
		
	return multiplier;
}

// 列表
void do_list(object me, string arg)
{
	int count = 0;
	int trading = 0;
	int valid_cover_time;
	int personal_total_gain = query("total_futures_gain", me);
	mapping positions = FUTURES_D->query_positions(me->query_id(1));
	
	string *msg = ({"編號  期貨名稱         買價       賣價       成交       漲跌     平倉仍需秒數    狀態\n"});
	msg += ({"─────────────────────────────────────────────────────\n"});
	
	foreach(array futures in FUTURES_D->query_futures())
	{
		trading = FUTURES_D->is_trading_time(count);
		
		count++;
	
		if( mapp(positions[count-1]) )
			valid_cover_time = positions[count-1]["time"] + futures[FUTURES_VALIDTRADETIME] - time();
		else
			valid_cover_time = 0;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-10s "HIY"%10.2f "HIY"%10.2f "HIY"%10.2f"NOR" %10s "HIM"%16d    %s\n"NOR,
			count,
			futures[FUTURES_NAME], 
			futures[FUTURES_BUYPRICE], 
			futures[FUTURES_SELLPRICE], 
			futures[FUTURES_PRICE], 
			(futures[FUTURES_VARIATION]>0 ? HIR"+" : futures[FUTURES_VARIATION]==0. ? HIW : HIG) + sprintf("%.2f", futures[FUTURES_VARIATION]),
			valid_cover_time >= 0 ? valid_cover_time : 0,
			trading ? HIC"交易中"NOR : NOR CYN"未開盤"NOR
		)});
	}

	msg += ({ sprintf("\n"NOR YEL"%s"HIY" %d%%"NOR"\n%s\n%s", "個人手續費乘數", to_int(fund_multiplier(personal_total_gain)*100), NOR YEL"個人當月期貨損益 "HIY+money(DEFAULT_MONEY_UNIT, personal_total_gain)+NOR, NOR YEL"世界當月期貨損益總合 "HIY+money(DEFAULT_MONEY_UNIT, FUTURES_D->get_world_total_gain())+NOR+"\n") });
	
	msg += ({"─────────────────────────────────────────────────────\n"});
	msg += ({NOR "個人當月期貨損益每增加 "HIG"1000 億"NOR"獲利，即提升 "HIG"10%"NOR" 手續費。\n"NOR });
	msg += ({NOR "個人當月期貨損益每月將歸零重新計算。\n"NOR });
	msg += ({NOR "台灣期貨交易時間為開盤日 "HIM"08:45AM"NOR" 至 "HIM"01:45PM"NOR"。\n" });
	msg += ({NOR "目前個別期貨最大持有部位保證金總額限制為 "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"。\n"NOR });
	msg += ({NOR "請注意，當損益達"HIR" -100% "NOR"時，系統將強迫"HIR"斷頭砍倉"NOR"，所有保證金將全部損失。\n"NOR});
	
	return me->more(implode(msg, "")+"\n");
}

void do_futures(object me, string arg)
{
	int count;
	int valid_cover_time;
	int personal_total_gain = query("total_futures_gain", me);
	mapping positions = FUTURES_D->query_positions(me->query_id(1));
	
	string *msg = ({"編號  期貨名稱         保證金/每口     金額/指數      手續費/每口     平倉時限\n"});
	msg += ({"─────────────────────────────────────────\n"});
	
	foreach(array futures in FUTURES_D->query_futures())
	{
		count++;
	
		if( mapp(positions[count-1]) )
			valid_cover_time = positions[count-1]["time"] + futures[FUTURES_VALIDTRADETIME] - time();
		else
			valid_cover_time = 0;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-12s "HIY"%14s"NOR YEL" %14s %12s%-5s "HIM" %10d\n"NOR,
			count,
			futures[FUTURES_NAME], 
			NUMBER_D->number_symbol(futures[FUTURES_MARGIN]), 
			NUMBER_D->number_symbol(futures[FUTURES_VALUE]),
			NUMBER_D->number_symbol(to_int(fund_multiplier(personal_total_gain) * futures[FUTURES_PROCEDUREFUND])),
			"("+(to_int(fund_multiplier(personal_total_gain) * futures[FUTURES_PROCEDUREFUND])*100/futures[FUTURES_MARGIN])+"%)",
			futures[FUTURES_VALIDTRADETIME],
		)});
	}

	msg += ({ sprintf("\n"NOR YEL"%s"HIY" %d%%"NOR"\n%s\n%s", "個人手續費乘數", to_int(fund_multiplier(personal_total_gain)*100), NOR YEL"個人當月期貨損益 "HIY+money(DEFAULT_MONEY_UNIT, personal_total_gain)+NOR, NOR YEL"世界當月期貨損益總合 "HIY+money(DEFAULT_MONEY_UNIT, FUTURES_D->get_world_total_gain())+NOR+"\n") });
	
	msg += ({"─────────────────────────────────────────\n"});
	msg += ({NOR "個人當月期貨損益每增加 "HIG"1000 億"NOR"獲利，即提升 "HIG"10%"NOR" 手續費。\n"NOR });
	msg += ({NOR "個人當月期貨損益每月將歸零重新計算。\n"NOR });
	msg += ({NOR "台灣期貨交易時間為開盤日 "HIM"08:45AM"NOR" 至 "HIM"01:45PM"NOR"。\n" });
	msg += ({NOR "目前個別期貨最大持有部位保證金總額限制為 "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"。\n"NOR });
	msg += ({NOR "請注意，當損益達"HIR" -100% "NOR"時，系統將強迫"HIR"斷頭砍倉"NOR"，所有保證金將全部損失。\n"NOR});
	
	return me->more(implode(msg, "")+"\n");
}

void do_listen(object me, string arg)
{
	array futures = FUTURES_D->query_futures();
	int num, sec;
	
	if( !arg ) return tell(me, "指令格式錯誤。\n");

	if( arg[<4..<1] == "stop" && sscanf(arg, "%d stop", num) == 1 )
	{
		FUTURES_D->stop_listen(me->query_id(1), num-1);
		tell(me, pnoun(2, me)+"停止收聽「"+futures[num-1][FUTURES_NAME]+"」的即時報價。\n");	
	}
	else if( sscanf(arg, "%d %ds", num, sec) == 2 )
	{
		set("futures_listen_tick/"+(num-1), sec, me);
		
		if( num <= 0 || num > sizeof(futures) )
			return tell(me, "沒有這個期貨商品編號。\n");
	
		FUTURES_D->start_listen(me->query_id(1), num-1);
		tell(me, pnoun(2, me)+"開始收聽「"+futures[num-1][FUTURES_NAME]+"」的即時報價。\n");
	}
	else
		return tell(me, "指令格式錯誤。\n");
}

// 利潤
string benefit_ratio(float gain, float margin)
{
	if( gain > 0. )
		return sprintf(NOR RED"+"HIR"%.2f%%"NOR, gain*100./margin);
	else if( gain == 0. )
		return HIW"0.00%"NOR;
	else
		return sprintf(NOR GRN"-"HIG"%.2f%%"NOR, -gain*100./margin);
}

// 確認期貨交易
void confirm_trade_futures(object me, int num, array futures, int trade_amount, int old_amount, float old_price, string arg)
{
	int cost;
	int gain;
	int total_gain;
	int margin;
	int cover_amount;
	int new_amount;
	int personal_total_gain = query("total_futures_gain", me);
	float new_price;
	mapping position = FUTURES_D->query_positions(me->query_id(1), num);
	int valid_cover_time = position["time"] + futures[FUTURES_VALIDTRADETIME] - time();

	if( !arg )
		return me->finish_input();

	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "取消期貨交易。\n");
		return me->finish_input();
	}

	if( futures[FUTURES_UPDATETIME] < time() - 180 )
	{
		tell(me, futures[FUTURES_NAME]+"資料尚未更新，暫時停止買賣。\n");
		return me->finish_input();
	}
		
	if( !FUTURES_D->is_trading_time(num) )
		return tell(me, futures[FUTURES_NAME]+"目前不在交易時間中，無法進行交易。\n");

	// 買進
	if( trade_amount > 0 )
	{
		// 平倉
		if( old_amount < 0 )
		{
			// 全部平倉
			if( trade_amount >= abs(old_amount) )
			{
				cover_amount = abs(old_amount);
				trade_amount -= cover_amount;
				old_amount = 0;
			}
			// 部份平倉
			else
			{
				cover_amount = trade_amount;
				trade_amount = 0;
				old_amount += cover_amount;
			}
		}
		
		// 計算平倉損益
		if( cover_amount > 0 )
		{
			if( valid_cover_time > 0 )
			{
				tell(me, pnoun(2, me)+"必須再等待 "+valid_cover_time +" 秒後才能平倉「"HIW+futures[FUTURES_NAME]+NOR"」期貨。\n");
				return me->finish_input();
			}

			gain = cover_amount * (to_int(old_price*100) - to_int(futures[FUTURES_SELLPRICE]*100)) * futures[FUTURES_VALUE] / 100;
			margin = cover_amount * futures[FUTURES_MARGIN];
		
			if( margin + gain > 0 )
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);
			
			total_gain = gain - to_int(cover_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));

			addn("total_futures_gain", total_gain, me);
			
			TOP_D->update_top_futures(me);
			
			tell(me, pnoun(2, me)+"平倉 "+cover_amount+" 口「"+futures[FUTURES_NAME]+"」期貨，共取回 "HIY+money(DEFAULT_MONEY_UNIT, margin+gain)+NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"。\n");
			CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", futures[FUTURES_SELLPRICE])+NOR" 價格("HIY+sprintf("%.2f", old_price)+NOR")"HIR"買進 "+NUMBER_D->number_symbol(cover_amount)+" 口"NOR"「"HIW+futures[FUTURES_NAME]+NOR"」期貨"HIY"平倉"NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"("+benefit_ratio(total_gain, margin)+")。");
			
			position["amount"] += cover_amount;
			
			FUTURES_D->set_positions(me->query_id(1), num, position);
		}			
		
		// 買進期貨
		if( trade_amount > 0 )
		{
			margin = trade_amount * futures[FUTURES_MARGIN];
			
			cost = margin + to_int(trade_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));
				
			if( !me->spend_money(DEFAULT_MONEY_UNIT, cost) )
			{
				tell(me, pnoun(2, me)+"身上的錢不足 "+money(DEFAULT_MONEY_UNIT, cost)+"。\n");
				return me->finish_input();
			}
			else
			{
				if( old_amount != 0 )
				{
					new_amount = old_amount + trade_amount;
					new_price = to_int(((old_amount * to_int(old_price*100)) + (trade_amount * to_int(futures[FUTURES_SELLPRICE]*100))) / new_amount) / 100.;
				}
				else
				{
					new_amount = trade_amount;
					new_price = futures[FUTURES_SELLPRICE];
				}
				
				position["amount"] = new_amount;
				position["price"] = new_price;
				position["time"] = time();
				
				FUTURES_D->set_positions(me->query_id(1), num, position);
				
				tell(me, pnoun(2, me)+"總共支出 "HIY+money(DEFAULT_MONEY_UNIT, cost)+NOR" 買進 "+trade_amount+" 口「"HIW+futures[FUTURES_NAME]+NOR"」期貨。\n");
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", futures[FUTURES_SELLPRICE])+NOR" 價格"HIR"買進 "+NUMBER_D->number_symbol(trade_amount)+" 口"NOR"「"HIW+futures[FUTURES_NAME]+NOR"」期貨。");
			}
		}
	}
	// 賣出
	else
	{
		// 平倉
		if( old_amount > 0 )
		{
			// 全部平倉
			if( abs(trade_amount) >= old_amount )
			{
				cover_amount = old_amount;
				trade_amount += cover_amount;
				old_amount = 0;
			}
			// 部份平倉
			else
			{
				cover_amount = abs(trade_amount);
				trade_amount = 0;
				old_amount -= cover_amount;
			}
		}
		
		// 計算平倉損益
		if( cover_amount > 0 )
		{
			if( valid_cover_time > 0 )
			{
				tell(me, pnoun(2, me)+"必須再等待 "+valid_cover_time +" 秒後才能平倉「"HIW+futures[FUTURES_NAME]+NOR"」期貨。\n");
				return me->finish_input();
			}
			
			gain = cover_amount * (to_int(futures[FUTURES_BUYPRICE]*100) - to_int(old_price*100)) * futures[FUTURES_VALUE] / 100;
			margin = cover_amount * futures[FUTURES_MARGIN];
			
			if( margin + gain > 0 )
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);

			total_gain = gain - to_int(cover_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));

			addn("total_futures_gain", total_gain, me);
			
			TOP_D->update_top_futures(me);
			
			tell(me, pnoun(2, me)+"平倉 "+cover_amount+" 口「"+futures[FUTURES_NAME]+"」期貨，共取回 "HIY+money(DEFAULT_MONEY_UNIT, margin+gain)+NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"。\n");
			CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", futures[FUTURES_BUYPRICE])+NOR" 價格("HIY+sprintf("%.2f", old_price)+NOR")"HIG"賣出 "+NUMBER_D->number_symbol(cover_amount)+" 口"NOR"「"HIW+futures[FUTURES_NAME]+NOR"」期貨"HIY"平倉"NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"("+benefit_ratio(total_gain, margin)+")。");
			
			position["amount"] += -cover_amount;
			
			FUTURES_D->set_positions(me->query_id(1), num, position);
		}			
		
		// 賣出期貨
		if( trade_amount < 0 )
		{
			margin = -trade_amount * futures[FUTURES_MARGIN];

			cost = margin - to_int(trade_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));
				
			if( !me->spend_money(DEFAULT_MONEY_UNIT, cost) )
			{
				tell(me, pnoun(2, me)+"身上的錢不足 "+money(DEFAULT_MONEY_UNIT, cost)+"。\n");
				return me->finish_input();
			}
			else
			{
				if( old_amount != 0 )
				{
					new_amount = old_amount + trade_amount;
					new_price = to_int(((old_amount * to_int(old_price*100)) + (trade_amount * to_int(futures[FUTURES_BUYPRICE]*100))) / new_amount) / 100.;
				}
				else
				{
					new_amount = trade_amount;
					new_price = futures[FUTURES_BUYPRICE];
				}
				
				position["amount"] = new_amount;
				position["price"] = new_price;
				position["time"] = time();
				
				FUTURES_D->set_positions(me->query_id(1), num, position);
				
				tell(me, pnoun(2, me)+"總共支出 "HIY+money(DEFAULT_MONEY_UNIT, cost)+NOR" 賣出 "+(-trade_amount)+" 口「"HIW+futures[FUTURES_NAME]+NOR"」。\n");
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"以 "HIY+sprintf("%.2f", futures[FUTURES_BUYPRICE])+NOR" 價格"HIG"賣出 "+NUMBER_D->number_symbol(-trade_amount)+" 口"NOR"「"HIW+futures[FUTURES_NAME]+NOR"」期貨。");
			}
		}
	}

	me->save();
	me->finish_input();
}

// 購入期貨
void do_buy_futures(object me, string arg)
{
	int num, amount;
	array futures_list = FUTURES_D->query_futures();
	int old_amount;
	float old_price;
	int personal_total_gain = query("total_futures_gain", me);
	mapping position;
	
	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式，例：buy 1 100。\n");

	if( amount < 1 || amount > 1000000 )
		return tell(me, "每次能夠交易的期貨口數為 1 至 1000000。\n");
		
	if( num < 1 || num > sizeof(futures_list) )
		return tell(me, "沒有 "+num+" 這一編號期貨。\n");
	
	num--;

	if( futures_list[num][FUTURES_UPDATETIME] < time() - 180 )
		return tell(me, futures_list[num][FUTURES_NAME]+"資料尚未更新，暫時停止買賣。\n");

	position = FUTURES_D->query_positions(me->query_id(1), num);
	
	old_amount = position["amount"];
	old_price = position["price"] || 0.;

	if( amount < old_amount/5 )
		return tell(me, "每次交易口數不得低於目前持有口數的 20%。\n");
		
	if( (amount + old_amount) * futures_list[num][FUTURES_MARGIN] > MAX_LOT_PRICE )
		return tell(me, "同一期貨最大持有部位保證金金額不可超過 "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"，「"HIW+futures_list[num][FUTURES_NAME]+NOR"」目前最大只能持有 "+(MAX_LOT_PRICE/futures_list[num][FUTURES_MARGIN])+" 口。\n");
		
	tell(me,
		WHT"期貨名稱      "NOR HIW+futures_list[num][FUTURES_NAME]+NOR"\n"
		"───────────────────\n"
		YEL"目前期貨賣價  "HIY+sprintf("%.2f", futures_list[num][FUTURES_SELLPRICE])+"\n"NOR
		GRN"目前持倉均價  "HIG+sprintf("%.2f", old_price)+"\n"NOR
		GRN"目前持有部位  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"買進期貨口數  "HIC+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"買進期貨總價  "HIC+money(DEFAULT_MONEY_UNIT, amount * futures_list[num][FUTURES_MARGIN])+"\n"NOR
		YEL"交易手續費用  "HIY+money(DEFAULT_MONEY_UNIT, to_int(amount * futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)))+NOR YEL" (平倉時不需支付)\n"NOR
		YEL"初期買進損益  "HIY+sprintf("%.2f%%", (-futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)*100./futures_list[num][FUTURES_MARGIN]))+NOR YEL" (需至少上漲 "HIY+sprintf("%.2f", futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)/futures_list[num][FUTURES_VALUE])+NOR YEL" 點才可損益兩平)\n"NOR
		"───────────────────\n"
		HIY"是否確定買進期貨?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_trade_futures, me, num, futures_list[num], amount, old_amount, old_price :));
}

// 售出期貨
void do_sell_futures(object me, string arg)
{
	int num, amount;
	array futures_list = FUTURES_D->query_futures();
	int old_amount;
	float old_price;
	int personal_total_gain = query("total_futures_gain", me);
	mapping position;

	if( !userp(me) ) return;
		
	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "請輸入正確的格式，例：buy 1 100。\n");

	if( amount < 1 || amount > 1000000 )
		return tell(me, "每次能夠交易的期貨口數為 1 至 1000000。\n");
		
	if( num < 1 || num > sizeof(futures_list) )
		return tell(me, "沒有 "+num+" 這一編號期貨。\n");
		
	num--;

	if( futures_list[num][FUTURES_UPDATETIME] < time() - 180 )
		return tell(me, futures_list[num][FUTURES_NAME]+"資料尚未更新，暫時停止買賣。\n");

	position = FUTURES_D->query_positions(me->query_id(1), num);
	
	old_amount = position["amount"];
	old_price = position["price"] || 0.;

	if( amount < old_amount/5 )
		return tell(me, "每次交易口數不得低於目前持有口數的 20%。\n");
			
	if( (amount - old_amount) * futures_list[num][FUTURES_MARGIN] > MAX_LOT_PRICE )
		return tell(me, "同一期貨最大持有部位保證金金額不可超過 "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"，「"HIW+futures_list[num][FUTURES_NAME]+NOR"」目前最大只能持有 "+(MAX_LOT_PRICE/futures_list[num][FUTURES_MARGIN])+" 口。\n");
			
	tell(me,
		WHT"期貨名稱      "NOR HIW+futures_list[num][FUTURES_NAME]+NOR"\n"
		"───────────────────\n"
		YEL"目前期貨買價  "HIY+sprintf("%.2f", futures_list[num][FUTURES_BUYPRICE])+"\n"NOR
		GRN"目前持倉均價  "HIG+sprintf("%.2f", old_price)+"\n"NOR
		GRN"目前持有部位  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"賣出期貨口數  "HIC+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"賣出期貨總價  "HIC+money(DEFAULT_MONEY_UNIT, amount * futures_list[num][FUTURES_MARGIN])+"\n"NOR
		YEL"交易手續費用  "HIY+money(DEFAULT_MONEY_UNIT, to_int(amount * futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)))+NOR YEL" (平倉不需支付)\n"NOR
		YEL"初期賣出損益  "HIY+sprintf("%.2f%%", (-futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)*100./futures_list[num][FUTURES_MARGIN]))+NOR YEL" (需至少下跌 "HIY+sprintf("%.2f", futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)/futures_list[num][FUTURES_VALUE])+NOR YEL" 點才可損益兩平)\n"NOR
		"───────────────────\n"
		HIY"是否確定賣出期貨?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_trade_futures, me, num, futures_list[num], -amount, old_amount, old_price :));
}

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
顯示期貨即時資訊
list		列出所有目前可交易期貨即時資料
HELP,

"futures":
@HELP
顯示期貨資訊
futures		列出所有目前可交易期貨設定
HELP,

"listen":
@HELP
收聽即時期貨資訊
listen 1 5s		設定每 5 秒鐘收聽一次編號 1 期貨的即時報價
listen 1 stop	停指收聽編號 1 期貨的即時報價
HELP,

"buy":
@HELP
買入期貨的指令
buy 1 300	買入編號 1 的期貨 300 口
HELP,

"sell":
@HELP
賣出期貨的指令
sell 1 100	賣出編號 1 的期貨 100 口
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"futures": (: do_futures :),
				"listen" : (: do_listen :),
				"buy" : (: do_buy_futures :),
				"sell" : (: do_sell_futures :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIC"期貨"NOR CYN"交易所"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

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
	,2
});
