/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : church.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : 教堂
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

void confirm_pray(object me, string type, string arg)
{
	arg = lower_case(arg);
	
	if( arg != "yes" && arg != "y" )
		return tell(me, pnoun(2, me)+"放棄提升屬性能力。\n");

	switch(type)
	{
		case "strength":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 50 點。\n");
				return me->finish_input();
			}
	
			me->add_str(1);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的力量屬性。\n"NOR);
			break;
		case "physique":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 50 點。\n");
				return me->finish_input();
			}
	
			me->add_phy(1);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的體格屬性。\n"NOR);
			break;
		case "intelligence":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 50 點。\n");
				return me->finish_input();
			}
	
			me->add_int(1);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的智商屬性。\n"NOR);
			break;
		case "agility":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 50 點。\n");
				return me->finish_input();
			}
	
			me->add_agi(1);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的敏捷屬性。\n"NOR);
			break;
		case "charisma":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 50 點。\n");
				return me->finish_input();
			}
	
			me->add_cha(1);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的魅力屬性。\n"NOR);
			break;
		case "stamina":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 1 點。\n");
				return me->finish_input();
			}
	
			addn("abi/stamina/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的體力最大值。\n"NOR);
			break;
		case "health":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 1 點。\n");
				return me->finish_input();
			}
	
			addn("abi/health/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的生命最大值。\n"NOR);
			break;
		case "energy":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"的聲望不足 1 點。\n");
				return me->finish_input();
			}
	
			addn("abi/energy/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"增加了 1 點的精神最大值。\n"NOR);
			break;
	}	
	
	me->save();
	me->finish_input();
}

// 祈禱
void do_pray(object me, string arg)
{
	if( !arg )
	{
		if( !me->in_condition(DIE) )
			return tell(me, pnoun(2, me)+"不需要淨化。\n");

       		msg("$ME閉上眼睛開始祈禱，"HIW"淨"NOR WHT"化"NOR"身上的汙穢。\n", me, 0, 1);

	       	me->stop_condition(DIE);
      	
	       	return;
	}
	
	if( !sscanf(arg, "for %s", arg) )
		return tell(me, pnoun(2, me)+"想要祈禱什麼？\n");
	
	arg = lower_case(arg);	

	switch(arg)
	{
		case "strength":
			tell(me, pnoun(2, me)+"提升力量需要 50 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "physique":
			tell(me, pnoun(2, me)+"提升體格需要 50 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "intelligence":
			tell(me, pnoun(2, me)+"提升智商需要 50 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "agility":
			tell(me, pnoun(2, me)+"提升敏捷需要 50 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "charisma":
			tell(me, pnoun(2, me)+"提升魅力需要 50 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "stamina":
			tell(me, pnoun(2, me)+"提升體力需要 1 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "health":
			tell(me, pnoun(2, me)+"提升生命需要 1 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "energy":
			tell(me, pnoun(2, me)+"提升精神需要 1 點聲望，是否確定提升？(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		default:
			tell(me, "沒有 "+arg+" 這個項目，請參考 help pray。\n");
			break;
	}
}


// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"教堂大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    教堂大廳。
HELP,

"pray":
@HELP
可以向神祈禱。
pray			向神祈求淨化
pray for strength	向神祈禱力量提升(每 1 點需 50 聲望)
pray for physique	向神祈禱體格提升(每 1 點需 50 聲望)
pray for intelligence	向神祈禱智商提升(每 1 點需 50 聲望)
pray for agility	向神祈禱敏捷提升(每 1 點需 50 聲望)
pray for charisma	向神祈禱魅力提升(每 1 點需 50 聲望)
pray for stamina	向神祈禱體力提升(每 1 點需 1 聲望)
pray for health		向神祈禱生命提升(每 1 點需 1 聲望)
pray for energy		向神祈禱精神提升(每 1 點需 1 聲望)
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": 0,
                "action":
                        ([
                                "pray"		: (: do_pray :),
                        ]),
	
	]),	

]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIW"教堂"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// 開張儀式費用
	,5000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,10
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
