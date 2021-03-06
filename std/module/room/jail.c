/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : jail.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-24
 * Note   : 監獄
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
#include <object.h>

inherit ROOM_ACTION_MOD;

private void do_arrest(object me, string arg)
{
	object target;
	object env = environment(me);
	string city = env->query_city();
	string reason;

	if( !wizardp(me) && !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, "只有市長或官員能夠逮捕犯人。\n");

	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"想要逮捕哪位犯人？\n");

	if( sscanf(arg, "%s because %s", arg, reason) != 2 )
		return tell(me, "請輸入正確的指令格式。\n");

	foreach(string id in explode(arg, " "))
	{
		reset_eval_cost();

		target = load_user(id);
	
		if( !objectp(target) )
		{
			tell(me, "這個世界並沒有 "+id+" 這位玩家。\n");
			continue;
		}
			
		if( !wizardp(me) && query("city", target) != city )
		{
			tell(me, target->query_idname()+"並不是本市的市民。\n");
			
			if( !userp(target) )
				destruct(target);
	
			continue;
		}
		
		if( target == me )
		{
			tell(me, pnoun(2, me)+"無法逮捕自己。\n");
			continue;
		}
		
		if( !wizardp(me) && CITY_D->is_mayor(city, target) )
		{
			tell(me, pnoun(2, me)+"無法逮捕市長。\n");
			
			if( !userp(target) )
				destruct(target);
				
			continue;
		}
	
		if( query("prisoner", target) )
		{
			tell(me, target->query_idname()+"已經被關進監獄了。\n");
	
			if( !userp(target) )
				destruct(target);
	
			continue;
		}
	
		if( wizardp(me) )
			CHANNEL_D->channel_broadcast("news", target->query_idname()+HIR"因為「"HIW + reason + NOR HIR"」遭到"+me->query_idname()+HIR"的逮捕，關進了巫師監獄接受調查。");
		else
			CHANNEL_D->channel_broadcast("city", target->query_idname()+HIR"因為「"HIW + reason + NOR HIR"」遭到"+me->query_idname()+HIR"的逮捕，關進了政府監獄。", me);
		
		target->move_to_environment(me);
		
		set("prisoner", 1, target);
		set("quit_place", base_name(env), target);
		target->save();
	
		if( !userp(target) )
			destruct(target);
	}
}

private void do_release(object me, string arg)
{
	object target;
	object env = environment(me);
	string city = env->query_city();
	string reason;
	
	if( !wizardp(me) && !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, "只有市長或官員能夠釋放犯人。\n");

	if( !arg || !arg[0] )
		return tell(me, pnoun(2, me)+"想要釋放哪位犯人？\n");
		
	if( sscanf(arg, "%s because %s", arg, reason) != 2 )
		return tell(me, "請輸入正確的指令格式。\n");

	target = load_user(arg);

	if( !objectp(target) )
		return tell(me, "這個世界並沒有 "+arg+" 這位玩家。\n");
		
	if( !wizardp(me) && query("city", target) != city )
		return tell(me, target->query_idname()+"並不是本市的市民。\n");
	
	if( target == me )
		return tell(me, pnoun(2, me)+"無法釋放自己。\n");
		
	if( !same_environment(me, target) )
		return tell(me, target->query_idname()+"並不在此處。\n");
		
	if( !query("prisoner", target) )
		return tell(me, target->query_idname()+"並不是囚犯。\n");
		
	if( wizardp(me) )
	{
		CHANNEL_D->channel_broadcast("news", me->query_idname()+HIR"因為「"HIW + reason + NOR HIR"」將"+target->query_idname()+HIR"從巫師監獄中釋放出來。"NOR);
		msg("$ME拍了拍自己身上的灰塵，回到巫師神殿去了... \n" + NOR , target, 0, 1);
		target->move(load_object(WIZ_HALL));
	}
	else
		CHANNEL_D->channel_broadcast("city", me->query_idname()+HIR"因為「"HIW + reason + NOR HIR"」將"+target->query_idname()+HIR"從政府監獄中釋放出來。"NOR, me);
	
	delete("prisoner", target);
	target->save();
	
		if( !userp(target) )
			destruct(target);
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"jail"	:
	([
		"short"	: HIR"牢"NOR RED"房"NOR,
		"help"	:
			([
"topics":
@HELP
    用來囚禁犯人的地方。
HELP,

"arrest":
@HELP
逮捕某位玩家，用法如下：
  arrest '玩家ID' because '原因'	- 將某位線上市民逮捕(市長與官員指令)
HELP,

"release":
@HELP
釋放某位玩家，用法如下：
  release '玩家ID' because '原因'	- 將某位被關在監獄中的市民釋放(市長與官員指令)
HELP,

			]),
		"action":
			([
				"arrest"	: (: do_arrest :),
				"release"	: (: do_release :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIR"監獄"NOR

	// 開張此建築物之最少房間限制
	,1

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,AGRICULTURE_REGION | INDUSTRY_REGION

	// 開張儀式費用
	,5000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,-100
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});

