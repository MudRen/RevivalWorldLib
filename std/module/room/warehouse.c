/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : warehouse.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-05-26
 * Note   : 模組 - 倉庫
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_import";
inherit __DIR__"inherit/_action_export";
inherit __DIR__"inherit/_action_transport";
inherit __DIR__"inherit/_action_setup";
inherit __DIR__"inherit/_action_auction";

#include <npc.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

#define WAREHOUSE_CAPACITY	-1

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"warehouse"	:
	([
		"short"	: HIR"儲藏庫房"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    倉庫是玩家儲藏物品的地方。
HELP,

"import":
@HELP
將商品上架的指令，用法如下：[管理指令]
  import all		- 將所有身上的物品存入倉庫
  import all rice	- 將所有的 rice 物品存入倉庫
  import 3 rice		- 將 3 件 rice 物品存入倉庫
  import rice		- 將 1 件 rice 物品存入倉庫
HELP,

"export":
@HELP
將商品下架的指令，用法如下：[管理指令]
  export all		- 將所有的物品從倉庫內取出
  export all rice	- 將所有 rice 物品從倉庫內取出
  export 3 rice		- 將 3 件 rice 物品從倉庫內取出
  export rice		- 將 1 件 rice 物品從倉庫內取出
HELP,

"transport":
@HELP
運輸物品的指令，用法如下：
  transport rice to 1 51,22	- 將 1 件 rice 物品運輸至第 1 衛星都市座標(51,22)處的建築物
  transport 3 rice to 2 51,22	- 將 3 件 rice 物品運輸至第 2 衛星都市座標(51,22)處的建築物
  transport 2 3 to 1 51,22	- 將 2 件編號為 3 的物品運輸至 第 1 衛星都市座標(51,22)處的建築物
HELP,

"setup":
@HELP
設定倉庫功能的指令，用法如下：[管理指令]
  setup class 2 食品 		- 將編號為 2 的物品分類到食品類的架子上，可自訂分類
HELP,

"auction":
@HELP
拍賣物品的指令，用法如下：

auction '物品編號' '數量' $'底價' $'直接購買價' '拍賣時間'

例如:
	
auction 1 500 $20000 $1000000 5		- 拍賣編號為 1 的物品 500 個，底標價為 $20000，
					  直接購買價為 $1000000，拍賣時間為 5 小時

幣值單位一律為 $RW

HELP,

"list":
@HELP
列出所有倉庫中的貨品的指令，用法如下：
  list			- 列出所有倉庫中的貨品
  list 2		- 列出第二項貨品的詳細資料
HELP,


			]),
		"master":1,
		"heartbeat":0,	// 實際時間 1 秒為單位
		"action":
			([
				"import"	: (: do_import($1, $2, "products", WAREHOUSE_CAPACITY) :),
				"export"	: (: do_export($1, $2, "products") :),
				"list"		: (: do_warehouse_list($1, $2, "products", WAREHOUSE_CAPACITY) :),
				"transport"	: (: do_transport($1, $2, "products") :),
				"setup"		: (: do_warehouse_setup($1, $2) :),
				"auction"	: (: do_auction($1, $2, "products") :),
			]),
	]),

]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIR"倉庫"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,INDIVIDUAL

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,INDUSTRY_REGION

	// 開張儀式費用
	,2000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,1
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,5
	
	// 建築物時代
	,1
});

