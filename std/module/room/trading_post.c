/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trading_post.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-13
 * Note   : 自由買賣所動作繼承物件
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

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_buy";
inherit __DIR__"inherit/_action_sell";

#define TRADING_POST_CAPACITY	-1
#define DISASSEMBLE_MONEY		500000

nosave mapping allowable_trade = 
([

	"農作":
		({
			"/obj/farming/seed/herbage_seed", -1,
			"/obj/farming/seed/corn_seed", -1,
			"/obj/farming/seed/rice_seedling", -1,
			"/obj/farming/seed/barley_seedling", -1,
			"/obj/farming/seed/wheat_seedling", -1,
			"/obj/farming/seed/herb_seed", -1,
			"/obj/farming/seed/rose_seed", -1,
			"/obj/farming/seed/sunflower_seed", -1,
			"/obj/farming/seed/lily_seed", -1,
			"/obj/farming/seed/cotton_seed", -1,
		}),
	"牧場":
		({
			"/obj/pasture/breed/chicken_stud", -1,
			"/obj/pasture/breed/pig_stud", -1,
			"/obj/pasture/breed/sheep_stud", -1,
			"/obj/pasture/breed/cattle_stud", -1,
			"/obj/pasture/breed/deer_stud", -1,
		}),
	"養殖":
		({
			"/obj/fishfarm/breed/young_shrimp", -1,
			"/obj/fishfarm/breed/young_crab", -1,
			"/obj/fishfarm/breed/clam", -1,
		}),
	"釣魚":
		({
			"/obj/fishing/rod/coarse_rod", -1,
			"/obj/fishing/bait/bread_bug", -1,
			"/obj/fishing/bait/prawn", -1,
			"/obj/fishing/bait/tsou_chip", -1,
			"/obj/fishing/bait/worm", -1,
		}),
	"採集":
		({
			"/obj/collection/tool/coarse_pickax", -1,
			"/obj/collection/tool/coarse_drill", -1,
			"/obj/collection/tool/coarse_bucket", -1,
			"/obj/collection/tool/coarse_saw", -1,
			"/obj/collection/tool/coarse_shovel", -1,
			
		}),
	"食物":
		({
			"/obj/systemproducts/pure_water", -1,
			"/obj/systemproducts/steamed_roll", -1,
			"/obj/systemproducts/dark_plum", -1,
		}),
	"藥丸":
		({
			"/obj/systemproducts/pill_of_purification", -1,
			"/obj/systemproducts/pill_of_invincibility", -1,
			"/obj/systemproducts/pill_of_strength", -1,
			"/obj/systemproducts/pill_of_physique", -1,
			"/obj/systemproducts/pill_of_intelligence", -1,
			"/obj/systemproducts/pill_of_agility", -1,
			"/obj/systemproducts/pill_of_charisma", -1,
		}),
	"其他":
		({
            "/obj/etc/newspaper", -1,
            "/obj/etc/firework", -1,
            "/obj/systemproducts/flatbed_tricycle", -1,
            "/obj/systemproducts/fortune_bag", -1,
		}),

]);

mapping exchange_objects =
([
	"/obj/battlefield/battle_insigne" : ({ HIC"戰"NOR CYN"爭"HIC"勳"NOR CYN"章"NOR, "battle_insigne"}),
]);

void do_exchange(object me, string arg)
{
	object ob;
	string basename;

	if( !arg )
		return tell(me, pnoun(2, me)+"想要向古董商兌換什麼物品？\n");	
	
	if( !objectp(ob = present(arg, me)) )
		return tell(me, "這裡沒有 "+arg+" 這項物品。\n");
		
	basename = base_name(ob);
	
	if( undefinedp(exchange_objects[basename]) )
		return tell(me, ob->query_idname()+"不算是古董，無法兌換。\n");
		
	if( arrayp(exchange_objects[basename]) )
	{
		int amount = ob->query_amount();
		
		msg("$ME將"+ob->query_idname()+"交給古董商，兌換為「"+exchange_objects[basename][0]+"」點數 "+amount+" 點。\n", me, 0, 1);
	
		log_file("command/trading_post_exchange", me->query_idname()+"將"+ob->query_idname()+"交給古董商，兌換為「"+exchange_objects[basename][0]+"」點數 "+amount+" 點。");
		
		addn(exchange_objects[basename][1], amount, me);
		
		destruct(ob);
	}
	else if( stringp(exchange_objects[basename]) )
	{
		object newob = new(exchange_objects[basename]);

		newob->set_amount(ob->query_amount());
		
		msg("$ME將"+ob->query_idname()+"交給古董商，兌換全新的"+newob->query_idname()+"。\n", me, 0, 1);
	
		log_file("command/trading_post_exchange", me->query_idname()+"將"+ob->query_idname()+"交給古董商，兌換全新的"+newob->query_idname()+"。");
		
		destruct(ob);
	
		if( !me->available_get(newob, newob->query_amount()) )
		{
			tell(me, pnoun(2, me)+"拿不動那麼多的東西，只好先放在地上了。\n");
			newob->move_to_environment(me);
		}
		else
			newob->move(me);
	}
		
	me->save();
}

void do_disassemble(object me, string arg)
{
	object ob;
	object material;
	int too_heavy;
	string msg="";
	mapping materials;
	int random_amount;
	int count = 1;
	string money_unit = environment(me)->query_money_unit();
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要拆解什麼物品？\n");	
	
	sscanf(arg, "%d %s", count, arg);
	
	if( count < 1 ) count = 1;
		
	if( !objectp(ob = present(arg, me)) )
		return tell(me, "這裡沒有 "+arg+" 這項物品。\n");
	
	if( count > ob->query_amount() )
		return tell(me, pnoun(2, me)+"身上沒有這麼多的"+ob->query_idname()+"。\n");
		
	if( ob->is_keeping() )
		return tell(me, pnoun(2, me)+"必須先解除"+ob->query_idname()+"的保留狀態。\n");
	
	if( ob->is_module_product() )
		materials = query("design/inlay", ob);
	else
		materials = query("disassemble", ob);

	if( !sizeof(materials) )
		return tell(me, ob->query_idname()+"無法拆解。\n");

	if( !ob->is_module_product() )
	{
		if( !me->spend_money(money_unit, DISASSEMBLE_MONEY * count) )
			return tell(me, pnoun(2, me)+"身上的錢不足 "HIY+money(money_unit, DISASSEMBLE_MONEY * count)+NOR"。\n");

		msg("$ME花費 "HIY+money(money_unit, DISASSEMBLE_MONEY * count)+NOR" 將 "+count+" "+(query("unit", ob)||"個")+ob->query_idname()+"交給工人拆解，換回下列物品：\n", me, 0, 1);
	}

	for(int i=0;i<count;++i)
	{
		foreach(string material_name, int amount in materials)
		{
			material = new(material_name);
			
			if( ob->is_module_product() )
			{
				if( amount > 8000 )
					random_amount = amount * 99 / 100;
				else
					random_amount = amount;
			}
			else if( random(5) <= random(amount) )
				random_amount = random(amount + 1);
			else
				random_amount = 0;
				
			if( random_amount <= 0 || random_amount > amount )
				continue;
			
			material->set_amount(random_amount);
			
			msg += NOR YEL"．"NOR+trim(QUANTITY_D->obj(material, random_amount))+"\n";
			
			if( query("special", material) )
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"自"+ob->query_idname()+"上拆解出「"+QUANTITY_D->obj(material, random_amount)+"」");
	
			if( !me->available_get(material, material->query_amount()) )
			{
				too_heavy = 1;
				material->move_to_environment(me);
			}
			else
				material->move(me);
		}
	}
	
	if( msg == "" )
		msg += NOR YEL"．"HIY"未拆解出任何物品\n"NOR;

	else
		log_file("command/disassemble", msg);

	msg(msg, me, 0, 1);

	if( too_heavy )
		tell(me, pnoun(2, me)+"拿不動那麼多的東西，只好先放在地上了。\n");

	destruct(ob, count);

	me->save();
}

void do_element(object me, string arg)
{
	object ob;
	object newob;
	int amount;

	string to;
	string from;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要向元素大盤商兌換什麼物品？\n");	
	
	if( sscanf(arg, "%d %s to %s", amount, from, to) != 3 )
		return tell(me, "請輸入正確的指令格式。\n");

	if( !objectp(ob = present(from, me)) )
		return tell(me, "這裡沒有 "+from+" 這項物品。\n");
		
	if( !ob->is_gem_object() )
		return tell(me, "只有元素才能進行兌換。\n");
			
	if( amount <= 0 )
		amount = 1;

	if( amount > ob->query_amount() )
		return tell(me, pnoun(2, me)+"身上沒有這麼多的"+ob->query_idname()+"。\n");
		
	switch(lower_case(to))
	{
		case "red": 
			newob = new("/obj/gem/gem0");
			break;
		case "orange":
			newob = new("/obj/gem/gem1");
			break;
		case "yellow":
			newob = new("/obj/gem/gem2");
			break;	
		case "green":
			newob = new("/obj/gem/gem3");
			break;	
		case "blue":
			newob = new("/obj/gem/gem4");
			break;	
		case "indigo":
			newob = new("/obj/gem/gem5");
			break;	
		case "purple":
			newob = new("/obj/gem/gem6");
			break;	
		default:
			return tell(me, "沒有 "+to+" 這種種類的元素。\n");
	}
	
	log_file("command/element", me->query_idname()+"將"+amount+" "+ob->query_idname()+"交給大盤商，兌換成"+newob->query_idname()+"。");
	
	newob->set_amount(amount);
		
	msg("$ME將 "+amount+" 個"+ob->query_idname()+"交給大盤商，全部兌換成"+newob->query_idname()+"。\n", me, 0, 1);
	
	destruct(ob, amount);
	
	if( !me->available_get(newob, newob->query_amount()) )
	{
		tell(me, pnoun(2, me)+"拿不動那麼多的東西，只好先放在地上了。\n");
		newob->move_to_environment(me);
	}
	else
		newob->move(me);
		
	me->save();
}

// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"買賣大廳"NOR,
		"help"	:
			([
"topics":
@HELP
    買賣大廳是玩家購買與販賣商品的地方，由於是由政府開設，僅會以五成價格向玩家收購商品。
HELP,

"list":
@HELP
列出所有販賣中的商品的指令，用法如下：
  list			- 列出所有販賣中的商品
  list 2		- 列出第二項商品的詳細資料
HELP,

"buy":
@HELP
購買商品的指令，用法如下：
  buy all rice			- 買下所有的 rice 商品
  buy 3 rice			- 買下 3 件 rice 商品
  buy rice			- 買下 1 件 rice 商品
  buy 3 2			- 買下 3 件編號為 2 的商品
  buy 2				- 買下 1 件編號為 2 的商品
  buy rice to center 1 51,22	- 買下 1 件 rice 商品並送達 center 城市第 1 衛星都市座標(51,22)處的建築物
HELP,
			]),
		"action":
			([
				"list"		: (: do_list($1, $2, "products", TRADING_POST_CAPACITY) :),
				"buy"		: (: do_buy($1, $2, "products", "GOVERNMENT") :),
			]),
	]),
	"workroom"	:
	([
		"short"	: HIW"工坊"NOR,
		"help"	:
			([
"topics":
@HELP
    工坊可以用來交換古董物品，以及將一些稀有物品拆解成元素。
HELP,

"exchange":
@HELP
向古董商兌換物品的指令，用法如下：
  exchange '物品'		- 向古董商兌換身上的某項物品
HELP,

"element":
@HELP
向大盤商兌換元素的指令，用法如下：
  element '數量' '顏色' to '顏色' - 向大盤商兌換元素的指令(ex: element 5 blue to red)
  				    (顏色種類包含 red/orange/yellow/green/blue/indigo/purple)
HELP,

"disassemble":
@HELP
請工人幫您將物品拆解成元素，用法如下：
  disassemble '物品'		- 拆解某項物品
HELP,
			]),
		"action":
			([
				"exchange"	: (: do_exchange :),
				"disassemble"	: (: do_disassemble :),
				"element"	: (: do_element :),
			]),
	]),
]);


// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	NOR YEL"物流中心"NOR

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
	,2
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,1
});
