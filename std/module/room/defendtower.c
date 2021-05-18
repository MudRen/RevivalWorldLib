/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : defendtower.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-15
 * Note   : 防禦塔
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

void do_arrow_fire(object me, string arg)
{
	
	
	
}

void do_stone_fire(object me, string arg)
{
	
	
	
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"arrow"	:
	([
		"short"	: HIG"弓箭"NOR GRN"塔"NOR,
		"heartbeat":5,	// 實際時間 1 秒為單位
		"master":1,
		"help"	:
			([
"topics":
@HELP
	弓箭塔可對附近大範圍進行弓箭射擊。
HELP,

"fire":
@HELP
弓箭射擊控制指令，用法如下：
	fire			- 進行大範圍弓箭射擊
HELP,
			]),
		"action":
			([
				"fire":	(: do_arrow_fire :),
			]),
	]),
	
	"stone"	:
	([
		"short"	: HIW"投"NOR WHT"石塔"NOR,
		"heartbeat":5,	// 實際時間 1 秒為單位
		"master":1,
		"help"	:
			([
"topics":
@HELP
	投石塔可對附近小範圍進行投石攻擊。
HELP,

"fire":
@HELP
投石攻擊控制指令，用法如下：
	fire			- 進行小範圍投石攻擊
HELP,
			]),
		"action":
			([
				"fire":	(: do_stone_fire :),
			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIY"防禦"NOR YEL"塔"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,INDIVIDUAL

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,AREA_REGION

	// 開張儀式費用
	,3000000

	// 建築物關閉測試標記
	,1

	// 繁榮貢獻度
	,0
	
	// 最高可加蓋樓層
	,2
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,3
});
