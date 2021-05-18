/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recycle.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : 垃圾回收場動作物件
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

#define RECYCLE_MONEY	0.7

void do_recycle(object me, string arg)
{
	int value;
	int money = 0;
	string moneyunit = MONEY_D->query_default_money_unit();
	int amount;
	string option;
	object ob, env = environment(me);

	if( !arg )
		return tell(me, "目前回收總數："+NUMBER_D->number_symbol(query("numbers", env))+" 個。\n");

	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		if( option == "all" ) ;
		// 如果 option 並不是數字單位
		else if( !big_number_check(option) )
		{
			arg = option + " " + arg;
			amount = 1;	
		}	
		else if( to_int(option) < 1 ) 
			amount = 1;
		else
			amount = to_int(option);
	}
	else amount = 1;
	
	if( objectp(ob = present(arg, me)) )
	{
		if( ob->no_amount() )
		{
			string idname = ob->query_idname();
			string basename = base_name(ob);
			object *obs = filter_array(all_inventory(me), (: base_name($1) == $(basename) :));
			int size = sizeof(obs);

			if( option == "all" ) 
				amount = size;
			else if( amount > size )
				return tell(me, pnoun(2, me)+"身上只有 "+size+" "+(query("unit",ob)||"個")+ob->query_idname()+"。\n");

			size = amount;
			
			value = query("value", ob);

			while(amount--)
			{
				if( !objectp(ob = present(arg, me)) )
				{
					size--;
					continue;
				}
			
				if( ob->is_keeping() )
				{
					tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留(keep)狀態。\n");
					size--;
					continue;
				}
				
				destruct(ob);
			}
			
			money = size * RECYCLE_MONEY * value;
			set("numbers", count(query("numbers", env), "+", size), env);
			msg("$ME把 "+size+" 個"+idname+"丟進資源回收桶，獲得回收金 "HIY+money(moneyunit, money)+NOR"，目前回收總數："+NUMBER_D->number_symbol(query("numbers", env))+" 個。\n", me, 0, 1);	
		} 
		else
		{
			if( option == "all" )
				amount = ob->query_amount();
			else if( amount > ob->query_amount() ) 
				return tell(me, pnoun(2, me)+"身上只有 "+ob->query_amount()+" "+(query("unit",ob)||"個")+ob->query_idname()+"。\n");
				
			if( ob->is_keeping() )
				return tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留(keep)狀態。\n");
			
			// 因匯差過大會造成買賣獲利, 暫時 mark
			value = query("value", ob);
			money = amount * RECYCLE_MONEY * value;
	
			if( money < 0 )
				money = 0;
			
			me->earn_money(moneyunit, money);
	
			set("numbers", count(query("numbers", env), "+", amount), env);
			msg("$ME把"+QUANTITY_D->obj(ob, amount)+"丟進資源回收桶，獲得回收金 "HIY+money(moneyunit, money)+NOR"，目前回收總數："+NUMBER_D->number_symbol(query("numbers", env))+" 個。\n", me, 0, 1);
			
			destruct(ob, amount);
		}
		return;
	}

	return tell(me, pnoun(2, me)+"身上沒有 "+arg+" 這件物品。\n");
}

string look_room(object room)
{
	if( query("function", room) == "recycle" )
		return NOR GRN"回收總數 "HIG+NUMBER_D->number_symbol(query("numbers", room))+NOR GRN" 個"NOR"\n";
}	

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"recycle"	:
	([
		"short"	: NOR GRN"回收室"NOR,
		"help"	:
			([
"topics":
@HELP
    回收中心可以回收所有任何不具價值的物品，同時為了獎勵環保，每回收一件物品可得 10 元獎金。
HELP,

"recycle":
@HELP
回收資源的指令(可獲得原價 $RW 70% 的回收金)，用法如下：
  recycle		- 顯示目前已回收總數
  recycle '物品ID'	- 回收該類物品
  recycle 3 '物品ID'	- 回收 3 件該類物品
  recycle all '物品ID'	- 回收所有該類物品
HELP,

			]),
		"action":
			([
				"recycle"	: (: do_recycle :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIG"垃圾回收場"NOR

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
	,30
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
