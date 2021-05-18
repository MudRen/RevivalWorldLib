
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : prodigy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-07
 * Note   : 世界奇觀動作繼承物件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <condition.h>

inherit ROOM_ACTION_MOD;

void do_pray(object me, string arg)
{
	object env = environment(me);

	if( env->query_city() != query("city", me) )
		return tell(me, pnoun(2, me)+"不是這個城市的市民，無法在這裡祈求奇蹟。\n");
	
    msg("$ME跪在奇觀內殿中央輕聲囑禱，一股神聖的力量突然湧出，內殿角落緩緩散出"HIR"紅"NOR RED"、"HIB"藍"NOR BLU"、"HIG"綠"NOR GRN"、"HIY"黃"NOR"四種顏色的淡煙，將$ME包覆起來...\n", me, 0, 1);

	me->start_condition(LIFE_MIRACLE);
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"naos"	:
	([
		"short"	: HIR"奇觀內殿"NOR,
		"help"	:
			([
"topics":
@HELP
    奇觀內殿。
HELP,

"pray":
@HELP
祈求奇蹟的降臨。
HELP,

			]),
		"action":
			([
				"pray"	: (: do_pray :),
			]),
	]),

]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIR"世界"NOR RED"奇觀"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,1

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,1000000000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,1000

	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});

