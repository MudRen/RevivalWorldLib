/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : 模組 - 貿易市場
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <npc.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

void do_auction_list(object me, string arg)
{
	AUCTION_D->do_auction_list(me, arg);
}

void do_bid(object me, string arg)
{
	AUCTION_D->do_bid(me, arg);
}

void do_auction_receive(object me, string arg)
{
	AUCTION_D->do_auction_receive(me, arg);
}

void do_request_list(object me, string arg)
{
	AUCTION_D->do_request_list(me, arg);
}

void do_request_receive(object me, string arg)
{
	AUCTION_D->do_request_receive(me, arg);
}

void do_request(object me, string arg)
{
	AUCTION_D->do_request(me, arg);
}

void do_support(object me, string arg)
{
	AUCTION_D->do_support(me, arg);
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: HIG"拍賣"NOR GRN"大廳"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    拍賣所是拍賣物品的地方。
HELP,

"bid":
@HELP
競標指令

bid '編號' '價錢'	- 提出競標價，幣值單位為 $RW (例: bid 1 1000)
bid '編號' auto		- 自動以最低允許價格來喊價
bid '編號' cancel	- 拍賣者可利用此指令來終止某個尚無人出價的拍賣(例: bid 1 cancel)
HELP,

"receive":
@HELP
得標物品付款與收件

receive '編號'			- 將得標物品收到身上
receive '編號' to 1 50,50	- 收取得標物品與付款並直接將物品運輸到第 1 衛星都市的 50,50 座標處

註1：流標之拍賣品亦需由拍賣者使用此指令領回
註2：領取得標物品一律收取得標價的 1% 手續費
HELP,

"request":
@HELP
提出商品訂單

request '數量' '物品名稱' per '單價'	- 提出某物品的訂單
request '編號' cancel			- 取消某筆訂單
HELP,

"list":
@HELP
列出拍賣物品

list			- 列出目前正在拍賣的物品(僅列出未結案物品)
list all		- 列出目前正在拍賣的物品(含已結案物品)
list '編號'		- 查詢某個編號拍賣物品的詳細拍賣資料

HELP,

			]),
		"master":1,
		"action":
			([
				"list"		: (: do_auction_list($1, $2) :),
				"receive"	: (: do_auction_receive($1, $2) :),
				"bid"		: (: do_bid($1, $2) :),
			]),
	]),

	"trade"	:
	([
		"short"	: HIC"交易"NOR CYN"大廳"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    交易大廳是下訂單的地方。
HELP,

"support":
@HELP
供貨指令

support '編號'				- 從身上供貨
support '編號' from 1 50,50		- 從第 1 衛星都市的 50,50 座標倉庫供給貨物
					  (必須將供貨物品放置在倉庫的「訂單供貨」分類中)
HELP,

"receive":
@HELP
訂單物品收件

receive '編號'				- 將供貨收到身上
receive '編號' to 1 50,50		- 收取供貨並直接將物品運輸到第 1 衛星都市的 50,50 座標處

HELP,

"request":
@HELP
提出商品訂單

request '數量' '物品名稱' per '單價'	- 提出某特定物品或工業產品的訂單(需手續費 10000)
request '編號' cancel			- 取消某筆訂單
HELP,

"list":
@HELP
列出訂單

list			- 列出目前正在拍賣的物品(僅列出未結案物品)
list all		- 列出目前正在拍賣的物品(含已結案物品)
list '編號'		- 查詢某個編號拍賣物品的詳細拍賣資料

HELP,

			]),
		"master":1,
		"action":
			([
				"list"		: (: do_request_list($1, $2) :),
				"receive"	: (: do_request_receive($1, $2) :),
				"support"	: (: do_support($1, $2) :),
				"request"	: (: do_request($1, $2) :),
			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIB"拍賣所"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,5000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,20
	
	// 最高可加蓋樓層
	,2
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});

