/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hospital.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-10-19
 * Note   : 醫院動作物件
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

inherit ROOM_ACTION_MOD;

#define BASE_MONEY	10000
#define EXTRA_MONEY	10

#define CURE_FATIGUE	"fatigue"
#define CURE_FOOD	"food"
#define CURE_DRINK	"drink"

void confirm_cure(object me, object target_ob, string money_unit, int total_money, string type, string arg)
{
	string city;
	
	if( !arg || !arg[0] || lower_case(arg) != "y" )
	{
		tell(me, "取消治療。\n");
		return me->finish_input();
	}

	if( !me->spend_money(money_unit, total_money) )
		return tell(me, pnoun(2, me)+"身上的錢不足 "HIY+money(money_unit, total_money)+NOR"。\n");	
	
	switch( type )
	{
		case CURE_FATIGUE:
			target_ob->reset_fatigue();
			msg("$ME花費了 "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"幫$YOU")+"治癒了全身的疲勞酸痛。\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_FATIGUE, time(), target_ob);
			break;
		case CURE_FOOD:
			target_ob->cost_food(target_ob->query_food_cur());
			msg("$ME花費了 "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"幫$YOU")+"進行灌腸，將一堆食物全拉了出來。\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_FOOD, time(), target_ob);
			break;
		case CURE_DRINK:
			target_ob->cost_drink(target_ob->query_drink_cur());
			msg("$ME花費了 "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"幫$YOU")+"進行催吐，將一堆飲料都吐了出來。\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_DRINK, time(), target_ob);
			break;	
	}
	
	city = environment(me)->query_city();
	
	if( CITY_D->city_exist(city) )
		CITY_D->change_assets(city, total_money/5);

	me->save();
	me->finish_input();
}

void do_cure(object me, string arg)
{
	string target;
	object target_ob;
	int total_skill_level;	
	mapping skills;
	int total_money;
	string money_unit = environment(me)->query_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"想要治療什麼？\n");

	target = arg[0..strsrch(arg, " ", -1)-1];
	
	arg = arg[strsrch(arg, " ", -1)+1..];

	if( !stringp(target) || !objectp(target_ob = present(target)) )
		target_ob = me;

	skills = query("skills", target_ob);

	if( mapp(skills) )
	{
		foreach(string skill, mapping data in skills)
			total_skill_level += data["level"];
	}
	else
		total_skill_level = 0;

	switch( arg )
	{
		case CURE_FATIGUE:
		{
			int fatigue = target_ob->query_fatigue();
			
			if( !fatigue )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不需要治療疲勞。\n");
	
			if( query_temp("cure/"CURE_FATIGUE, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不久之前才治療過，必須等待 "+(query_temp("cure/"CURE_FATIGUE, me) + 120 - time())+" 後才能再進行治療。\n");

			total_money = BASE_MONEY + EXTRA_MONEY * fatigue * total_skill_level;
			
			tell(me, pnoun(2, me)+"是否要花費 "HIY+money(money_unit, total_money)+NOR" 來"+(target_ob==me?"":"幫"+target_ob->query_idname())+"治療疲勞？(Y/N)：");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_FATIGUE :));
				
			break;
		}
		case CURE_FOOD:
		{
			int food = target_ob->query_food_cur();
			
			if( !food )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不需要進行灌腸。\n");
	
			if( query_temp("cure/"CURE_FOOD, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不久之前才治療過，必須等待 "+(query_temp("cure/"CURE_FOOD, me) + 120 - time())+" 後才能再進行治療。\n");

			total_money = BASE_MONEY + EXTRA_MONEY * food * total_skill_level;
			
			tell(me, pnoun(2, me)+"是否要花費 "HIY+money(money_unit, total_money)+NOR" 來"+(target_ob==me?"":"幫"+target_ob->query_idname())+"進行灌腸？(Y/N)：");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_FOOD :));

			break;
		}
		case CURE_DRINK:
		{
			int drink = target_ob->query_drink_cur();
			
			if( !drink )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不需要進行灌腸。\n");

			if( query_temp("cure/"CURE_DRINK, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"不久之前才治療過，必須等待 "+(query_temp("cure/"CURE_DRINK, me) + 120 - time())+" 後才能再進行治療。\n");

			total_money = BASE_MONEY + EXTRA_MONEY * drink * total_skill_level;
			
			tell(me, pnoun(2, me)+"是否要花費 "HIY+money(money_unit, total_money)+NOR" 來"+(target_ob==me?"":"幫"+target_ob->query_idname())+"進行催吐？(Y/N)：");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_DRINK :));

			break;
		}
		default:
			tell(me, pnoun(2, me)+"想要治療什麼？\n");
			break;
	}
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"emergency"	:
	([
		"short"	: NOR GRN"急診室"NOR,
		"help"	:
			([
"topics":
@HELP
    醫院，進行醫療的地方。
HELP,

"cure":
@HELP
進行治療的指令，用法如下：
cure fatigue		- 進行身體之疲勞與疾病治療
cure food		- 進行灌腸，使食物降低
cure drink		- 進行催吐，使飲水降低
cure 人名 項目		- 幫他人進行上述的治療(例：cure labor fatigue)

治療價格隨角色之技能等級數值加總而定
HELP,

			]),
		"action":
			([
				"cure"	: (: do_cure :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIW"醫"NOR WHT"院"NOR

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
	,5
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
