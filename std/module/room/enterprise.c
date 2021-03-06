/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : enterprise.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-03-31
 * Note   : 企業大樓
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
#include <condition.h>
#include <delay.h>

inherit ROOM_ACTION_MOD;

void do_invest(object me, string arg)
{
	string moneyunit, enterprise, default_moneyunit;
	int money;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要投資或是退資多少錢？\n");
		
	enterprise = query("enterprise", me);
	default_moneyunit = MONEY_D->query_default_money_unit();
	
	if( sscanf(arg, "$%s %d", moneyunit, money) == 2 )
	{
		moneyunit = upper_case(moneyunit);

		if( !(money = to_int(big_number_check(money))) )
			return tell(me, "請輸入正確的數字。\n");

		if( moneyunit != default_moneyunit )
			return tell(me, "企業集團的增資或退資僅接受 $"+default_moneyunit+" 種類的貨幣。\n");
				
		if( money > 0 )
		{
			if( !me->spend_money(moneyunit, money) )
				return tell(me, pnoun(2, me)+"沒有這麼多的現金。\n");
			
			ENTERPRISE_D->change_assets(enterprise, money);
			ENTERPRISE_D->change_invest(enterprise, me->query_id(1), money);
			
			CHANNEL_D->channel_broadcast("ent", me->query_idname()+"增資 "HIY+money(default_moneyunit, money)+NOR" 作為企業資金。\n", me);
		}
		else if( money < 0 )
		{
			if( !ENTERPRISE_D->change_assets(enterprise, money) )
				return tell(me, "目前企業組織內並沒有那麼多的資金可供周轉。\n");
			
			me->earn_money(default_moneyunit, -money);
			ENTERPRISE_D->change_invest(enterprise, me->query_id(1), money);

			CHANNEL_D->channel_broadcast("ent", me->query_idname()+"從企業集團中退資 "HIY+money(default_moneyunit, -money)+NOR"。\n", me);
		}
		else 
			return tell(me, pnoun(2, me)+"想要投資或退資多少錢？\n");
	}
	else
		return tell(me, "請 help invest 輸入正確的指令格式。\n");
	
}

void do_read(object me, string arg)
{
	object env = environment(me);
	
	if( query("enterprise", me) != query("owner", env)[11..] )
		return tell(me, pnoun(2, me)+"不是這間企業的成員無法使用這間閱讀室。\n");

	msg("$ME隨手拿起一本財經雜誌開始閱讀...。\n", me, 0, 1);
	me->start_delay(REST_DELAY_KEY, 10, pnoun(2, me)+"正在閱讀雜誌。\n", "閱讀完畢。\n");
        me->start_condition(FINANCEKNOWLEDGE);
}

void do_release(object me, string arg)
{
	string stockamount;
	string available_stockamount;
	string enterprise;
	object env = environment(me);
	
	if( !wizardp(me) )
		return tell(me, "測試中。\n");

	enterprise = query("owner", env)[11..];
	
	if( !ENTERPRISE_D->is_president(enterprise, me->query_id(1)) )
		return tell(me, pnoun(2, me)+"不是這個企業的總裁。\n");

	if( count(arg, "<=", 0) )
		return tell(me, "請輸入大於 0 的數字。\n");
		
	stockamount = count(arg, "+", ENTERPRISE_D->query_enterprise_info(enterprise, "stockamount"));
	available_stockamount = count(arg, "+", ENTERPRISE_D->query_enterprise_info(enterprise, "available_stockamount"));
	CHANNEL_D->channel_broadcast("stock", ENTERPRISE_D->query_enterprise_color_id(enterprise)+"企業集團宣布釋出股票 "HIC+NUMBER_D->number_symbol(arg)+NOR" 張，總釋出股票 "HIC+NUMBER_D->number_symbol(stockamount)+NOR" 張。\n");
	
	ENTERPRISE_D->set_enterprise_info(enterprise, "stockamount", stockamount);
	ENTERPRISE_D->set_enterprise_info(enterprise, "available_stockamount", available_stockamount);
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"fund"	:
	([
		"short"	: WHT"資金處理室"NOR,
		"help"	:
			([
"topics":
@HELP
    處理資金的地方。
HELP,

"invest":
@HELP
注入或取出資金。
invest $RW 10000	將個人資金投資進入企業集團
invest $RW -10000	退回個人投資資金
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"invest" : (: do_invest :),
			]),
	
	]),
	"reading"	:
	([
		"short"	: WHT"閱讀室"NOR,
		"help"	:
			([
"topics":
@HELP
    企業內部的圖書室。
HELP,

"read":
@HELP
閱讀書籍。
read			閱讀書籍
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"read" : (: do_read :),
			]),
	
	]),	
	"directorate"	:
	([
		"short"	: WHT"董事會"NOR,
		"help"	:
			([
"topics":
@HELP
    企業董事辦公的地方。
HELP,

"release":
@HELP
釋出股票
release			釋出股票
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([
				"release" : (: do_release :),
			]),
	
	]),	
	"president"	:
	([
		"short"	: HIW"總裁"WHT"辦公室"NOR,
		"help"	:
			([
"topics":
@HELP
    企業總裁辦公的地方。
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
		"action":
			([

			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	WHT"企業大樓"NOR

	// 開張此建築物之最少房間限制
	,4

	// 城市中最大相同建築數量限制(0 代表不限制)
	,5

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,ENTERPRISE

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,30000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,0
	
	// 最高可加蓋樓層
	,5
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
