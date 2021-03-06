/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : exchange_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-8
 * Note   : 匯率精靈
 * Update :
 *  o 2002-09-18 Clode 改由 MONEY_D 作中樞管理  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define DATA			"/data/daemon/exchange.o"

#define EXCHANGE		0
#define MONEY_UNIT		1
#define MIN_EXCHANGE		0.1
#define MAX_EXCHANGE		5.0
#define MAX_CHANGE_PERCENT	50.0
#define DEFAULT_GNP		"50000000000"
#define DEFAULT_EXCHANGE	0.9

/*
	一國家需建立"國家外匯管理局"方可與國外進行貨幣交易與貿易
	管理局可操控國家資金干預本國匯率

	匯率運作理論: 利率評價理論, 購買力評價理論
	
	經濟成長率呈正成長 	-> 穩中上升
	國際收支需求 		-> 上升 (貨幣需求量大價值提高)
	通貨膨脹高 		-> 下降 
	利率高 			-> 上升 (吸引外資進入)
	市場預期		-> 根據當國政治, 經濟走向
*/

/* International Exchange Table ([ money_unit : exchange ]) */
/* 匯率為比較值, 非絕對值 */
private mapping IET;
private mapping exchange_buffer;
private mapping exchange_trend;

nomask int save()
{
	return save_object(DATA);
}

// 建立新的匯率資訊
nomask int create_new_exchange(string money_unit)
{
	if( !stringp(money_unit) ) return 0;
	
	if( floatp(IET[money_unit]) )
		return -1;
	
	IET[money_unit] = DEFAULT_EXCHANGE;
	
	return save();
}

// 移除匯率資訊
nomask int remove_exchange(string money_unit)
{
	if( previous_object() != load_object(MONEY_D) ) return 0;
	
	if( !stringp(money_unit) || !floatp(IET[money_unit]) ) return 0;
	
	map_delete(IET, money_unit);

	return save();
}

/* 調整匯率 */
nomask float change_exchange(string money_unit, float fchange)
{
	float original_exchange;
	
	if( !stringp(money_unit) || !floatp(IET[money_unit]) || fchange==0 ) return 0.;
	
	if( (IET[money_unit] == MIN_EXCHANGE && fchange < 0)
	|| (IET[money_unit] == MAX_EXCHANGE && fchange > 0) ) return IET[money_unit];
	
	original_exchange = IET[money_unit];
	
	// 限制最大變動率
	if( abs(fchange)*100/original_exchange > MAX_CHANGE_PERCENT )
	{
		if( fchange < 0 )
			fchange = -original_exchange*MAX_CHANGE_PERCENT/100.;
		else
			fchange = original_exchange*MAX_CHANGE_PERCENT/100.;
	}

	IET[money_unit] += fchange;
	
	// 限制最低貶值數值
	if( IET[money_unit] < MIN_EXCHANGE ) 
		IET[money_unit] = MIN_EXCHANGE;
	else if( IET[money_unit] > MAX_EXCHANGE )
		IET[money_unit] = MAX_EXCHANGE;

	CHANNEL_D->channel_broadcast("city", 
		sprintf("%s"HIY"的 $%s 幣匯率%s %.6f 成為 %.6f，%s幅為 %.3f%%。"NOR,
			CITY_D->query_city_idname(MONEY_D->money_unit_to_city(money_unit)),
			money_unit,
			fchange > 0 ? "升值" : "貶值",
			abs(fchange),
			IET[money_unit],
			fchange > 0 ? "漲":"跌",
			abs(fchange)*100/original_exchange,
	), MONEY_D->money_unit_to_city(money_unit));
		
	return IET[money_unit];
}

nomask void set_exchange(string money_unit, float exchange)
{
	IET[money_unit] = exchange;	
}

nomask void record_exchange_buffer(string money_unit, string money)
{
	int change;
	string city = MONEY_D->money_unit_to_city(money_unit);
	string GNP = CITY_D->query_city_info(city, "GNP");

	// 標準貨幣幣值不變動
	if( money_unit == MONEY_D->query_default_money_unit() ) return;
	
	if( !CITY_D->city_exist(city) ) return;
	
	if( count(GNP, "<", DEFAULT_GNP) )
		GNP = DEFAULT_GNP;

	exchange_buffer[money_unit] = count(exchange_buffer[money_unit], "+", money);
	
	change = to_int(count(count(exchange_buffer[money_unit], "*", 100000), "/", GNP));
	
	// 若總變動量超過 GNP 的 1/100000, 則進行匯率調整
	if( change )
	{
		change_exchange(money_unit, change*0.00001);
		
		exchange_buffer[money_unit] = 0;
	}
}

varargs nomask mixed query_exchange_data(string money_unit)
{
	if( undefinedp(money_unit) )
		return copy(IET);
	
	return IET[money_unit];
}

/* 貨幣兌換 */
int convert(int money, string unit_from, string unit_to)
{
	float ex1, ex2;

	ex1 = IET[unit_from];
	ex2 = IET[unit_to];
	
	if( !floatp(ex1) || !floatp(ex2) ) return 0;

	return to_int(money * ex1 / ex2);
}

// 比較幣值或是做四則運算
int compare(string unit1, string money1, string op, string unit2, string money2)
{
	float ex1, ex2;

	ex1 = IET[unit1];
	ex2 = IET[unit2];
	
	if( !floatp(ex1) || !floatp(ex2) ) return 0;
	
	return count(count(money1,"*",ex1), op, count(money2,"*",ex2));
}

mapping query_exchange_buffer()
{
	return exchange_buffer;
}

string query_default_GNP()
{
	return DEFAULT_GNP;
}

void process_per_hour()
{
	string default_money_unit = MONEY_D->query_default_money_unit();

	string city;
	int flourish;
	float base_ex;
	float shift_ex;

	foreach(string money_unit, float ori_ex in IET)
	{
		if( money_unit == default_money_unit ) continue;

		city = MONEY_D->money_unit_to_city(money_unit);
		flourish = CITY_D->query_city_info(city, "totalflourish");

		base_ex = DEFAULT_EXCHANGE + pow(flourish, 0.5)/1000.;
		
		exchange_trend[money_unit] = base_ex;

		shift_ex = (base_ex - ori_ex)/500.;		

		if( random(10) )
			IET[money_unit] += shift_ex;
		else
			IET[money_unit] -= shift_ex;

		if( IET[money_unit] < MIN_EXCHANGE )
		{
			IET[money_unit] = MIN_EXCHANGE;
			exchange_trend[money_unit] = MIN_EXCHANGE;
		}
		else if( IET[money_unit] > MAX_EXCHANGE )
		{
			IET[money_unit] = MAX_EXCHANGE;
			exchange_trend[money_unit] = MAX_EXCHANGE;
		}

		CHANNEL_D->channel_broadcast("nch", money_unit+" fundamental exchange "+ori_ex+"/"+base_ex+" -> "+shift_ex);
	}

	save();
}

float query_exchange_trend(string money_unit)
{
	return exchange_trend[money_unit];
}

void create()
{
	
	if( clonep() ) 
		destruct(this_object());
	
	IET = allocate_mapping(0);
	
	if( file_size(DATA) < 1 )
		save();
	
	restore_object(DATA);
	
	map_delete(IET, 0);
	
	if( !mapp(exchange_buffer) )
		exchange_buffer = allocate_mapping(0);

	if( !mapp(exchange_trend) )
		exchange_trend = allocate_mapping(0);

	IET[MONEY_D->query_default_money_unit()] = 1.0;

	save();	
}

int remove()
{
	return save();
}

string query_name()
{
	return "匯率系統(EXCHANGE_D)";
}
