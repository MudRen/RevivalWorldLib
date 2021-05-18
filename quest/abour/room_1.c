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

#define BOOK_COST	5000000000

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIG"農居"NOR GRN"涼亭"NOR);
	set("long",@TEXT
    這裡是一個農村中的小涼亭，涼亭裡坐著一位提著布包的旅客，似乎正在此地
休息。往四週看去附近的農家正在努力耕種著，空氣中飄來陣陣的稻草香，稻草隨
風搖曳所發出來的簌簌聲也不斷地傳了進來，這樣的景色讓人心曠神怡，整個心情
不由自主地愉悅起來。

TEXT);
	set("exits", ([ 
		"east" : ({ 76, 47, 0, "lightforest", 0, 0, "76/47/0/lightforest/0/0" }) ,
		"west" : ({ 74, 47, 0, "lightforest", 0, 0, "74/47/0/lightforest/0/0" }) ,
		"north" : ({ 75, 46, 0, "lightforest", 0, 0, "75/46/0/lightforest/0/0" }) ,
		"south" : ({ 75, 48, 0, "lightforest", 0, 0, "75/48/0/lightforest/0/0" }) ,
	]));
	
	set("objects", ([
		"/quest/abour/npc_seller" : 1 ,
	]));
	::reset_objects();
}
void do_buy(object me, string arg)
{
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string moneyunit = MONEY_D->query_default_money_unit();
	
	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"想要買什麼配方書(請參考 plist)？");

	ob = new("/obj/book/product/"+lower_case(replace_string(arg, " ", "_")));

	if( !ob->query_idname() )
	{
		destruct(ob);
		return seller->do_command("say 哦，我沒有 "+arg+" 這種配方書耶...服務不周真是抱歉...");
	}
	
	if( !me->spend_money(moneyunit, BOOK_COST) )
	{
		destruct(ob);
		return seller->do_command("say "+pnoun(2, me)+"身上的金錢不足 "HIY+money(moneyunit, BOOK_COST)+NOR"。");
	}

	msg("$ME花費 "HIY+money(moneyunit, BOOK_COST)+NOR" 向$YOU購買"+ob->query_idname()+"。\n", me, seller, 1);
	log_file("command/buy_product_book", me->query_id(1)+" 購買"+ob->query_idname());

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
