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

#define BOOK_COST	3000000000

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIC"剪"NOR CYN"風居"NOR);
	set("long",@TEXT
    這裡是一個隱士的家，隱士坐在房子的一角，見到有人進來也不看一眼，只專
注在自己手上的書。房子內除了滿滿的書籍以外幾乎沒有其他的東西了，看那位隱
士身材魁梧，臉上一道極長的刀疤從額頭右方畫至下巴，似乎曾經是位英勇的戰士
，卻不知為何此時的他卻只在乎手中的書，似乎世界上的一切都已經與他無關了。

TEXT);
	set("exits", ([ 
		"west" : ({ 38, 56, 0, "sixcastle", 3, 0, "38/56/0/sixcastle/3/0" }) ,
	]));
	
	set("objects", ([
		"/quest/hermit/npc_seller" : 1 ,
	]));
	::reset_objects();
}
void do_buy(object me, string arg)
{
	object ob;
	object seller = filter_array(all_inventory(this_object()), (: $1->is_seller_npc() :))[0];
	string moneyunit = MONEY_D->query_default_money_unit();
	
	if( !arg )
		return seller->do_command("say "+pnoun(2, me)+"想要買哪種技能書？要買就快一點，買完趕快離開我的視線");

	if( !objectp(ob = new("/obj/book/skill/"+lower_case(replace_string(arg, " ", "-")))) )
		return seller->do_command("say 哼，不要以為我不懂，這世界上根本沒有 "+arg+" 這種技能書...");

	if( !me->spend_money(moneyunit, BOOK_COST) )
	{
		destruct(ob);
		return seller->do_command("say "+pnoun(2, me)+"身上的錢根本不夠 "HIY+money(moneyunit, BOOK_COST)+NOR"。\n");
	}

	msg("$ME花費 "HIY+money(moneyunit, BOOK_COST)+NOR" 向$YOU購買"+ob->query_idname()+"。\n", me, seller, 1);
	log_file("command/buy_skill_book", me->query_id(1)+" 購買"+ob->query_idname());

	ob->set_keep();
	
	if( !me->available_get(ob) )
	{
		tell(me, pnoun(2, me)+"拿不動"+ob->query_idname()+"了，只好先放在地上。\n");
		ob->move_to_environment(me);
	}
	else
		ob->move(me);
	
	seller->do_command("say 買完就快離開，不要在這裡干擾我");
}

mapping actions = 
([
	"buy" : (: do_buy :),
]);
