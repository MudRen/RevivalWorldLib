/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_1.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIW"水"HIC"晶"HIG"洞"HIY"窟"NOR);
	set("long",@TEXT
    這裡是一個極為隱密的洞窟，但洞內極為明亮，原來是因為洞窟內佈滿了各式
各樣五彩繽紛的水晶，有方形、圓形、菱形等等，各種結晶的方式都可以在這裡見
到，如此奇幻的地方實在讓人膛目結舌。只可惜這些水晶實在太過堅硬，想帶走的
話似乎不是那麼簡單的事。
TEXT);
	set("exits", ([ "out" : ({ 69, 53, 0, "cnossus", 2, 0, "69/53/0/cnossus/2/0" }) ]));

	set("objects", ([
		"/quest/crystalcave/npc_seller" : 1 ,
	]));

	::reset_objects();
}
void do_buy(object me, string arg)
{
	int num;
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string file;
	string moneyunit = MONEY_D->query_default_money_unit();
	int money;

	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"想要買什麼顏色的元素(red、orange、yellow、green、blue、indigo、purple)？");
	
	sscanf(arg, "%d %s", num, arg);

	switch(arg)
	{
		case "red":
			file = "/obj/gem/gem"+0;
			break;
		case "orange":
			file = "/obj/gem/gem"+1;
			break;
		case "yellow":
			file = "/obj/gem/gem"+2;
			break;
		case "green":
			file = "/obj/gem/gem"+3;
			break;
		case "blue":
			file = "/obj/gem/gem"+4;
			break;
		case "indigo":
			file = "/obj/gem/gem"+5;
			break;
		case "purple":
			file = "/obj/gem/gem"+6;
			break;
		default:
			return seller->do_command("say "+pnoun(2, me)+"想要買什麼顏色的元素(red、orange、yellow、green、blue、indigo、purple)？");
			break;
	}

	if( !num )
		num = 1;

	if( num < 1 || num > 1000 )
		return seller->do_command("say 請輸入正常的數量(1~1000)。");

	money = num*100000000;

	if( !me->spend_money(moneyunit, money) )
		return seller->do_command("say "+pnoun(2, me)+"身上的金錢不足 "HIY+money(moneyunit, money)+NOR"。\n");

	ob = new(file);
	ob->set_amount(num);
	
	msg("$ME花費 "HIY+money(moneyunit, money)+NOR" 向$YOU購買 "+num+" 顆"+ob->query_idname()+"。\n", me, seller, 1);
	log_file("command/buy_element", me->query_id(1)+" 購買 "+num+" 顆"+ob->query_idname());

	ob->set_keep();
	
	if( !me->available_get(ob) )
	{
		tell(me, pnoun(2, me)+"拿不動"+ob->query_idname()+"了，只好先放在地上。\n");
		ob->move_to_environment(me);
	}
	else
		ob->move(me);
}

mapping actions = 
([
	"buy" : (: do_buy :),
]);
