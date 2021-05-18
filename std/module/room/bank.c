/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bank.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : 銀行動作繼承物件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <bank.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <roommodule.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

#define INTEREST_RATE	1
#define MAX_INTEREST	100000

// 計算利息
void count_rate(object env, string id)
{
	object player;
	mapping savings;
	int newmoney, interest;
	
	if( !user_exists(id) )
	{
		delete("savings/"+id, env);
		return;
	}

	savings = query("savings/"+id+"/money", env);

	if( !savings || !sizeof(savings) ) return;

	player = find_player(id);

	foreach( string unit, mixed money in savings )
	{
		money = to_int(money);
		
		// 利息
		interest = money * INTEREST_RATE / 1000000;

		if( interest > MAX_INTEREST )
			interest = MAX_INTEREST;

		newmoney = money + interest;

		if( objectp(player) )
			tell(player, env->query_room_name()+"單日結算，利息共 "+HIY+money(unit, interest)+NOR"。\n");
		
		set("savings/"+id+"/money/"+unit, newmoney, env);
	}
}

// 搶劫
void do_rob(object me, string arg)
{
	msg("$ME衝進櫃台大喊："HIR"這是搶劫！！！！！\n"NOR, me, 0, 1);
	msg("只見面前嬌滴滴的櫃台小姐身影突然模糊起來，在$ME還沒來得及反應之前，她已經來到$ME的身後並賞了$ME一招迴旋踢！！\n", me, 0, 1);
	me->faint();
}


// 存錢
void do_deposit(object me, string arg)
{
	int type;
	string unit;
	mixed money, originmoney;
	string id = me->query_id(1);
	object env = environment(me);
	object master = env->query_master();
	mapping moneydata = MONEY_D->query_moneydata(id);

	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "$%s %s", unit, money) != 2 )
		return tell(me, pnoun(2, me)+"想要存進多少錢？(例：$"+env->query_money_unit()+" 100)\n");

	unit = upper_case(unit);
	money = to_int(big_number_check(money));

	if( !MONEY_D->money_unit_exist(unit) )
	{
		tell(me, "並沒有 "+unit+" 這種貨幣。\n");
		return;
	}

	if( !money )
		return tell(me, "請輸入正確的數字。\n");

	if( money <= 0 )
		return tell(me, "金額必須要大於零。\n");

	if( to_int(moneydata["cash"][unit]) < money )
		return tell(me, pnoun(2, me)+"身上沒有那麼多的 $"+unit+" 錢！\n");

	originmoney = to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master));
	set("savings/"+me->query_id(1)+"/money/"+unit, money + originmoney, master);

	me->spend_money(unit, money);

	msg("$ME將 "HIY+money(unit, money)+NOR" 存進了"+query("short", master)+"。\n", me, 0 ,1);

	type = moneydata["bank"][base_name(master)];
	
	if( !(type & GENERAL_DEPOSIT) )
		MONEY_D->set_bank(id, "set", base_name(master), type | GENERAL_DEPOSIT);

	master->save();
}

// 提錢
void do_withdraw(object me, string arg)
{
	string unit;
	mixed money, originmoney;
	string id = me->query_id(1);
	object env = environment(me);
	object master = env->query_master();

	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "$%s %s", unit, money) != 2 )
		return tell(me, pnoun(2, me)+"想要提領多少錢？\n");

	unit = upper_case(unit);
	money = to_int(big_number_check(money));

	if( !money )
		return tell(me, "請輸入正確的數字。\n");

	if( money <= 0 )
		return tell(me, "金額必須要大於零。\n");

	if( to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master)) < money )
		return tell(me, pnoun(2, me)+"在這間銀行的 $"+unit+" 存款並沒有那麼多。\n");

	originmoney = to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master));

	if( originmoney > money )
		set("savings/"+me->query_id(1)+"/money/"+unit, originmoney - money, master);
	else
	{
		delete("savings/"+me->query_id(1)+"/money/"+unit, master);

		if( !sizeof(query("savings/"+me->query_id(1)+"/money", master)) )
		{
			delete("savings/"+me->query_id(1), master);
			MONEY_D->set_bank(id, "delete", base_name(master));
		}
	}

	me->earn_money(unit, money);

	msg("$ME向"+query("short", master)+"提領了 "HIY+money(unit, money)+NOR"。\n", me, 0 ,1);
	
	master->save();
}

void do_account(object me, string arg)
{
	string msg = "";
	object env = environment(me);
	object master = env->query_master();

	mapping savings = query("savings/"+me->query_id(1)+"/money", master);

	if( !savings || !sizeof(savings) )
		return tell(me, pnoun(2, me)+"在這間銀行並沒有任何的存款。\n");

	msg += pnoun(2, me)+"在"+query("short", master)+"共有以下存款：\n";
	msg += NOR WHT"──────────────────────────────────\n"NOR;
	foreach( string unit, string money in savings )
	msg += " "+HIC+money(unit, money)+NOR"\n";

	msg += NOR WHT"──────────────────────────────────\n"NOR;

	me->more(msg);
}

void do_information(object me, string arg)
{	
	string msg="";	
	object env = environment(me);
	object master = env->query_master();

	msg += "\n單日存款複利利率為："+INTEREST_RATE+" / 100000\n";

	msg += "存款人資料：\n";

	if( !mapp(query("savings", master)) )
		msg += " 無任何存款資料\n";
	else
		foreach( string id, mapping savings in query("savings", master) )
	{
		msg += " "+capitalize(id)+" :\n";

		foreach( string moneyunit, string money in savings["money"] )
			msg += "      "+money(moneyunit, money)+"\n";
	}

	me->more(msg);
}

void do_autotransfer(object me, string arg)
{
	int type;
	string id = me->query_id(1);
	object master = environment(me)->query_master();
	string bankfile = base_name(master);
	mapping moneydata = MONEY_D->query_moneydata(id);
	
	if( !userp(me) ) return;

	type = moneydata["bank"][bankfile];
	
	if( !type )
		return tell(me, pnoun(2, me)+"在這間銀行並沒有任何存款，無法設定自動轉帳。\n");
		
	if( type & AUTOTRANSFER )
	{
		MONEY_D->set_bank(id, "set", bankfile, type ^ AUTOTRANSFER);
		return msg("$ME取消了在這間銀行所設定的自動轉帳功能。\n", me, 0, 1);
	}
	else
	{
		MONEY_D->set_bank(id, "set", bankfile, type | AUTOTRANSFER);
		msg("$ME在此申請了銀行的自動轉帳功能，每次轉帳將酌收 "+query("transfercost", master)+" 元的手續費用。\n", me, 0, 1);
	}
	
	master->save();
}

void do_convert(object me, string arg)
{
	int ratio;
	string msg;
	string default_money_unit = MONEY_D->query_default_money_unit();
	string src_unit, src_city, dst_unit, dst_city;
	mixed src_money, dst_money;

	if( !userp(me) ) return;

	//return tell(me, "匯制修改中，停止兌換任何外幣。\n");

	if( !arg || sscanf(arg, "$%s %s %*(to|TO) $%s", src_unit, src_money, dst_unit) != 4 )
		return tell(me, pnoun(2, me)+"要將哪種貨幣兌換為哪種貨幣？(例：convert $CT 100 to $DL)。\n");

	src_unit = upper_case(src_unit);
	dst_unit = upper_case(dst_unit);
	
	src_city = MONEY_D->money_unit_to_city(src_unit);
	dst_city = MONEY_D->money_unit_to_city(dst_unit);

	if( src_unit == dst_unit )
		return tell(me, "無法兌換成相同貨幣。\n");

	if( !wizardp(me) && dst_unit == "WZ" )
		return tell(me, pnoun(2, me)+"不能兌換這種貨幣。\n");

	if( src_unit != default_money_unit && !CITY_D->city_exist(src_city) )
		return tell(me, "使用 $"+src_unit+" 貨幣的城市已經消失，此種貨幣已不再具有價值。\n");
	
	if( dst_unit != default_money_unit && !CITY_D->city_exist(dst_city) )
		return tell(me, "使用 $"+dst_unit+" 貨幣的城市已經消失，此種貨幣已不再具有價值。\n");
	
	if( !(src_money = to_int(big_number_check(src_money))) || src_money < 1 )
		return tell(me, "請輸入正確的貨幣兌換數量。\n");
	
	if( src_money < 10000 )
		return tell(me, "每次兌換至少要 10,000 以上金額。\n");

	if( !MONEY_D->money_unit_exist(src_unit) )
		return tell(me, "這個世界上並沒有 "+src_unit+" 這種貨幣。\n");

	if( !MONEY_D->money_unit_exist(dst_unit) )
		return tell(me, "這個世界上並沒有 "+dst_unit+" 這種貨幣。\n");
	
	dst_money = to_int(EXCHANGE_D->convert(src_money, src_unit, dst_unit));

	if( !me->spend_money(src_unit, src_money) )
		return tell(me, pnoun(2, me)+"身上並沒有那麼多的 "+src_unit+" 現金。\n");

	ratio = 50 - to_int(me->query_skill_level("exchange")/5);
	dst_money -= (dst_money * ratio) / 1000;

	msg = "\n目前國際匯率比值 "HIC+src_unit+NOR":"HIC+dst_unit+NOR" 為 "HIG+EXCHANGE_D->query_exchange_data(src_unit)+NOR":"HIG+EXCHANGE_D->query_exchange_data(dst_unit)+NOR"。\n";
	msg += "扣除 "+ratio+"/1000 交易費後，"+pnoun(2, me)+"兌換 "HIY"$"+src_unit+" "+NUMBER_D->number_symbol(src_money)+NOR" 得到 "HIY"$"+dst_unit+" "+NUMBER_D->number_symbol(dst_money)+NOR"。\n\n";

	log_file("command/convert", me->query_idname()+"兌換 $"+src_unit+" "+NUMBER_D->number_symbol(src_money)+" 得到 $"+dst_unit+" "+NUMBER_D->number_symbol(dst_money));
	me->earn_money(dst_unit, dst_money);
	tell(me, msg);
}

void do_list(object me, string arg)
{
	string msg, city;
	float ex;
	mapping exchange = EXCHANGE_D->query_exchange_data();
	string *sort_exchange;

	msg = HIC"各大城市"NOR CYN"貨幣匯率比值如下：\n"NOR;
	msg += NOR WHT"────────────────────────────────\n"NOR;
	msg += sprintf("%-24s "HIG"%-5s"NOR HIW"%6.10f\n"NOR WHT"────────────────────────────────\n\n"NOR, HIW"重生的世界標準貨幣"NOR, MONEY_D->query_default_money_unit(), EXCHANGE_D->query_exchange_data(MONEY_D->query_default_money_unit()));
	msg += NOR HIY"城市                   "NOR GRN"幣種               "NOR WHT"匯率      "NOR YEL"城市總繁榮度\n"NOR;
	msg += NOR WHT"────────────────────────────────\n"NOR;
	
	sort_exchange = sort_array(keys(exchange), (: $(exchange)[$1] < $(exchange)[$2] ? 1 : -1 :));
		
	foreach( string unit in sort_exchange )
	{
		ex = exchange[unit];

		city = MONEY_D->money_unit_to_city(unit);
		
		if( city == "wizard" ) continue;

		if( CITY_D->city_exist(city) )
			msg += sprintf("%-24s "HIG"%-5s    "NOR HIW"%10.10f   "HIY"%15s\n"NOR, CITY_D->query_city_idname(city), unit, ex, NUMBER_D->number_symbol(CITY_D->query_city_info(city, "totalflourish")));
	}

	msg += NOR WHT"────────────────────────────────\n"NOR;

	me->more(msg);
}

void do_transferdata(object me, string arg)
{
	int num, x, y;
	object env = environment(me);
	string city = env->query_city();
	string file;
	string old_bank_file = base_name(env);
	object target;
	mapping savings = query("savings", env);
	mapping moneydata;

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"不是本市的市長，無法轉移存檔資料。\n");

	if( !sizeof(savings) )
		return tell(me, "這裡沒有存款資料。\n");
		
	if( !arg || sscanf(arg, "%d %d,%d", num, x, y) != 3 )
		return tell(me, "請輸入正確的銀行座標格式(例：2 24,59)。\n");
	
	file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "bank");
	
	if( !objectp(target = load_module(file)) || query("function", target) != "lobby"  )
		return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並不是銀行大廳。\n");
		
	if( target == env )
		return tell(me, "無法轉移存款資料到一位置。\n");

	foreach(string id, mapping data in savings)
	{
		moneydata = MONEY_D->query_moneydata(id);

		if( mapp(data["money"]) )
		foreach(string unit, string money in data["money"])
		{
			set("savings/"+id+"/money/"+unit, count(money, "+", query("savings/"+id+"/money/"+unit, target)), target);
		}
		
		MONEY_D->set_bank(id, "set", file, moneydata["bank"][old_bank_file]);
		MONEY_D->set_bank(id, "delete", old_bank_file);
	}
	
	delete("savings", env);
	env->save();
	
	target->save();
	
	msg("$ME將所有存款資料轉移至"+target->query_room_name()+"。\n", me, 0, 1);
	
	MONEY_D->save();
}


void heart_beat(object env)
{
	int timetick = addn("timetick", 1, env);

	// 因為 heart_beat 不準, 用 time_precision 來校正
	if( !(timetick % 3600) )
	{
		mapping savings = query("savings", env);
		
		// 只有連鎖主店可以發放利息
		if( env->query_master() != env ) return;

		if( sizeof(savings) )
		foreach( string id, mapping data in savings )
			count_rate(env, id);

		env->save();
	}
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
    	"lobby"	:
    	([
		"short"	: HIW"銀行大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    銀行大廳是玩家們存錢領錢的地方。
HELP,

"deposit":
@HELP
將現金存入銀行的指令，用法如下：
  deposit $CT 100	- 將現金 $CT 100 存入銀行帳號內
HELP,

"withdraw":
@HELP
將存款提領為現金的指令，用法如下：
  withdraw $CT 100	- 提領出 $CT 100 的存款
HELP,

"account":
@HELP
查詢目前的存款狀況，用法如下：
  account		- 顯示目前的存款狀況
HELP,

"info":
@HELP
查詢目前銀行所有存款資料，用法如下：[管理指令]
  info			- 顯示所有存款資料
HELP,

"transferdata":
@HELP
轉移目前銀行所有存款資料，用法如下：[管理指令]
  transferdata 2 50,50	- 將這裡所有的存款資料轉移至第二都市的 50,50 銀行大廳中(會自動累加金錢)
HELP,

			]),
		"heartbeat":1,	// 實際時間 1 秒為單位
		"master":1,
		"action":
		([
	    		"deposit"	: (: do_deposit :),
	    		"withdraw"	: (: do_withdraw :),
	    		"account"	: (: do_account :),
	    		"information"	: (: do_information :),
	    		"autotransfer"	: (: do_autotransfer :),
	    		"rob"		: (: do_rob :),
	    		"transferdata"	: (: do_transferdata :),
		]),
    	]),
    	"exchange":
    	([
		"short"	: HIW"匯兌室"NOR,
		"help"	:
			([
"topics":
@HELP
    這是一間匯兌室。
HELP,

"convert":
@HELP
兌換貨幣的指令，用法如下：
  convert $CT 100 to $DL	- 將 $CT 100 兌換成 $DL
HELP,

"list":
@HELP
列出目前所有城市貨幣匯率的對照表，用法如下：
  list			- 列出匯率對照表
HELP,
			]),
		"action":
		([
	    		"convert"	: (: do_convert :),
	    		"list"		: (: do_list :),
		]),
    	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIY"銀行"NOR

	// 開張此建築物之最少房間限制
	,2

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
	,40
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
