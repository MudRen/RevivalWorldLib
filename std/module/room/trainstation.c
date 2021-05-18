/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trainstation.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-11
 * Note   : 火車站
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
#include <location.h>
#include <delay.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

#define TICKET_PRICE		100000
#define TICKET			"/obj/systemproducts/train_ticket"

string *check_trains(object room)
{
	object target;
	string* trains;
	string* deleted_trains;

	if( !sizeof(trains = query("trains", room)) )
		return 0;

	deleted_trains = allocate(0);

	foreach(string file in trains)
	{
		if( !objectp(target = load_module(file)) || query("function", target) != "platform" )
			deleted_trains |= ({ file });
	}
	
	if( sizeof(deleted_trains) )
	{
		trains -= deleted_trains;
		set("trains", trains, room);
		room->save();
	}
	
	return trains;
}

void do_setup(object me, string arg)
{
	string value;
	object env = environment(me);
	string city = env->query_city();
	string* trains;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要設定什麼項目？(help setup)\n");

	sscanf(arg, "%s %s", arg, value);
	
	if( !CITY_D->is_mayor_or_officer(city, me) )
		return tell(me, pnoun(2, me)+"不是這座城市的市長或官員，無法設定火車月台班次。\n");

	trains = check_trains(env) || allocate(0);

	switch(arg)
	{
		case "add":
		{
			int num, x, y;
			string file;
			object target;

			if( !value || (sscanf(value, "'%s' %d %d,%d", city, num, x, y) != 4 && sscanf(value, "%s %d %d,%d", city, num, x, y) != 4) )
				return tell(me, "請輸入正確的火車站座標(例如：anycity 1 21,33)。\n");
			
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "trainstation");

			if( !objectp(target = load_module(file)) || query("function", target) != "platform" )
				return tell(me, "座標"+loc_short(city, num-1, x-1, y-1)+"並沒有火車站月台。\n");

			trains |= ({ file });
			set("trains", trains, env);
			
			env->save();
			env->startup_heart_beat();
			msg("$ME增加了前往"+CITY_D->query_city_name(city, num-1)+target->query_room_name()+"的班次。\n", me, 0, 1);
			break;	
		}
		case "delete":
		{
			int num = to_int(value);
			
			if( num < 1 || num > sizeof(trains) )
				return tell(me, "請輸入正確的編號。\n");
				
			trains = trains[0..(num-2)] + trains[num..];
			
			set("trains", trains, env);
			
			env->save();
			msg("$ME刪除了編號 "+num+" 的班次。\n", me, 0, 1);
			break;
		}
		default:
			return tell(me, "請輸入正確的指令(help setup)。\n");
	}
}

void do_list(object me, string arg)
{
	int num;
	string msg="";
	object env = environment(me);
	string* trains;
	object target;

	if( !sizeof(trains = check_trains(env)) )
		return tell(me, "目前沒有任何火車班次。\n");
	
	msg += WHT"──────────────────────────────────\n"NOR;
	msg += HIW"班次   目的地\n"NOR;
	msg += WHT"──────────────────────────────────\n"NOR;
	
	foreach(string file in trains)
	{
		target = load_module(file);
		msg += sprintf(HIY"%4d"NOR"   %s\n", ++num, CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name());
	}

	msg += WHT"──────────────────────────────────\n"NOR;
	
	me->more(msg);
}


// 購買車票
void do_buy(object me, string arg)
{
	object ob;
	string money_unit = environment(me)->query_money_unit();
	
	if( !me->spend_money(money_unit, TICKET_PRICE) )
		return tell(me, pnoun(2, me)+"身上的錢不足 "HIY+money(money_unit, TICKET_PRICE)+NOR"。\n");

	ob = new(TICKET);
	msg("$ME花了 "HIY+money(money_unit, TICKET_PRICE)+NOR+" 買了$YOU。\n", me, ob, 1);

	ob->move(me);
}


void do_board(object me, string arg)
{
	object room = environment(me);
	object train_ob = query_temp("train_ob", room);
	object* ob = filter_array(all_inventory(me), (: base_name($1) == TICKET :));

	if( environment(me) == this_object() )
		return tell(me, pnoun(2, me)+"已經在火車上了。\n");
		
	if( !sizeof(ob) )
		return tell(me, pnoun(2, me)+"必須先前往車站大廳購買車票。\n");

	if( !objectp(train_ob) )
		return tell(me, "火車尚未進站，請在月台上稍候。\n");

	msg("$ME將"+ob[0]->query_idname()+"交給了收票員並搭上了"+train_ob->query_idname()+"。\n", me, 0, 1);
	me->move(train_ob);
	broadcast(environment(me), me->query_idname() + "搭上了本班列車。\n", 0, me);
	destruct(ob, 1);
}

void heart_beat(object room)
{
	int tick;
	string* trains;
	string file;
	object target;
	
	if( !sizeof(trains = check_trains(room)) )
	{
		room->stop_heart_beat();
		return;
	}

	if( !stringp(file = query("train", room)) )
	{
		file = trains[addn_temp("next", 1, room)%sizeof(trains)];
			set("train", file, room);
	}

	if( !objectp(target = load_module(file)) )
		return;
	
	tick = addn_temp("tick", 1, room);

	// 0 秒
	if( !(tick%30) )
	{
		object train_ob;
		
		target = load_module(file);
		
		train_ob = new("/obj/etc/train", target);
		
		train_ob->move(room);

		broadcast(room, "新列車已經進站，本列車將於 30 秒後前往"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"，請欲搭乘本班列車的乘客儘速上車。\n");
		
		set_temp("train_ob", train_ob, room);
	}
	// 20 秒
	else if( !((tick+26)%30) )
		broadcast(room, "前往"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"的列車在 10 秒鐘後即將離站，請欲搭乘本班列車的乘客儘速上車。\n");
	// 30 秒
	else if( !((tick+24)%30) )
	{
		object train_ob = query_temp("train_ob", room);
		
		if( !objectp(train_ob) )
			return;

		broadcast(room, "前往"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"的列車已經離站。\n");

		train_ob->start();

		delete("train", room);

		delete_temp("train_ob", room);
	}
}

string look_room(object room)
{
	string msg = "";

	if( query("function", room) == "platform" )
	{
		string file = query("train", room);
		object train_ob = query_temp("train_ob", room);
		int tick = query_temp("tick", room);
		object target;

		target = load_module(file);
		
		if( !objectp(target) )
			msg += HIG"目前月台上無任何列車可搭\n\n"NOR;
		else if( objectp(train_ob) )
			msg += HIY"本班列車"NOR YEL"目的地："+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+" ("WHT"將於 "HIW+(30-((tick%30)*5))+WHT" 秒鐘後離站"NOR")\n\n";
		else
			msg += HIY"下班列車"NOR YEL"目的地："+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+" ("WHT"將於 "HIW+(150-((tick%30)*5))+WHT" 秒鐘後進站"NOR")\n\n";
	}
	return msg;
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"platform"	:
	([
		"short"	: HIC"車站"NOR CYN"月台"NOR,
		"help"	:
			([
"topics":
@HELP
    旅客搭車的月台。
HELP,

"setup":
@HELP
setup add anycity 1 21,33	- 增加座標 ancity 1 21,33 之火車站為列車目的地
setup delete 編號		- 刪除某班班次(依照 list 編號刪除)
HELP,

"board":
@HELP
board			- 支出車票搭上火車
HELP,

"list":
@HELP
list			- 列出這個月台通往的目的地
HELP,

			]),
		"heartbeat":5,	// 實際時間 1 秒為單位
		"master":1,
		"action":
			([
				"setup" :	(: do_setup :),
				"board" :	(: do_board :),
				"list"	:	(: do_list :),
			]),
	
	]),	
	"lobby"	:
	([
		"short"	: HIY"車站"NOR YEL"大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    購買車票的大廳。
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"master":1,
		"action":
			([
				"buy"	:	(: do_buy :),
			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIY"火車站"NOR

	// 開張此建築物之最少房間限制
	,3

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,GOVERNMENT

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,COMMERCE_REGION

	// 開張儀式費用
	,50000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,40
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,2
});
