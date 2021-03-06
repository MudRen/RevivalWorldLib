/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_auction.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

#define DEFAULT_PAYMENT		10000

void output_object(object env, string database, string basename, int amount);

void do_auction(object me, string arg, string database)
{
	int num, i, j;
	int amount;
	int auc_amount;
	mixed auc_base_price;
	mixed auc_direct_price;
	int auc_hour;
	mapping objects;
	string *shelfsort, shelf;
	string basename;
	object ob;
	object env = environment(me);
	object master = env->query_master();
	string money_unit = MONEY_D->query_default_money_unit();
	mapping auc_data = allocate_mapping(0);
	
	if( query("owner", env) != me->query_id(1) )
		return tell(me, "這裡是別人的建築物。\n");

	if( !arg )
		return tell(me, pnoun(2, me)+"想要拍賣哪一個物品？\n");
		
	if( sscanf(arg, "%d %d $%s $%s %d", num, auc_amount, auc_base_price, auc_direct_price, auc_hour) != 5 )
		return tell(me, "請輸入正確的拍賣資料(help auction)。\n");
		
	if( auc_amount < 1 || auc_amount > 9999999 )
		return tell(me, "請輸入正確的數量(1-9999999)。\n");
		
	auc_base_price = to_int(big_number_check(auc_base_price));
	auc_direct_price = to_int(big_number_check(auc_direct_price));

	if( auc_base_price < 1 || auc_base_price > 999999999999 )
		return tell(me, "請輸入正確的底標價格(1 - 999999999999)。\n");
		
	if( auc_direct_price < 1 || auc_direct_price > 999999999999 )
		return tell(me, "請輸入正確的直接購買價格(1 - 999999999999)。\n");
		
	if( auc_direct_price <= auc_base_price )
		return tell(me, "直接購買價不得等於或低於底價。\n");

	if( auc_hour < 1 || auc_hour > 7*24 )
		return tell(me, "競標時間最少 1 個小時，最多 168 小時。\n");
		
	objects = query(database, master);

	if( !sizeof(objects) )
		return tell(me, env->query_room_name()+"裡並沒有任何物品。\n");

	shelfsort = keys(objects) | (query("shelfsort", master) || allocate(0));

	foreach( shelf in shelfsort )
	{
		for(i=0;i<sizeof(objects[shelf]);i+=2)
		{
			if( ++j == num )
			{
				basename = objects[shelf][i];
				amount = to_int(objects[shelf][i+1]);

				if( catch(ob = load_object(basename)) )
					continue;
	
				// 若物品已經消失
				if( !objectp(ob) )
				{
					error(basename+" 商品資料錯誤。");
					continue;
				}
				
				if( query("flag/no_give", ob) )
					return tell(me, ob->query_idname()+"無法設定為訂單。\n");
		
				if( amount < auc_amount )
					return tell(me, ob->query_idname()+"沒有這麼多個。\n");

				if( !me->spend_money(money_unit, DEFAULT_PAYMENT) )
					return tell(me, pnoun(2, me)+"沒有足夠的錢繳交手續費 "HIY+money(money_unit, DEFAULT_PAYMENT)+NOR"。\n");
				else
					tell(me, pnoun(2, me)+"繳交了拍賣手續費 "+HIY+money(money_unit, DEFAULT_PAYMENT)+NOR"。\n");

				output_object(master, database, basename, auc_amount);
				master->save();

				auc_data["seller"] = me->query_id(1);
				auc_data["basename"] = basename;
				auc_data["amount"] = auc_amount;
				auc_data["minute"] = auc_hour*60;
				auc_data["base_price"] = auc_base_price;
				auc_data["direct_price"] = auc_direct_price;
				
				AUCTION_D->add_auction(auc_data);
				
				CHANNEL_D->channel_broadcast("auc", HIB"新商品："NOR+me->query_idname()+"拍賣「"+QUANTITY_D->obj(ob, auc_amount, 1)+"」，底價："HIY+money(money_unit, auc_base_price)+NOR"，直接購買價："HIY+money(money_unit, auc_direct_price)+NOR"，剩餘時間："HIR+(auc_hour*60)+NOR" 分鐘");
				return;
			}
		}
	}
	
	tell(me, "商品編號錯誤。\n");
}
