/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tax_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-14
 * Note   : 
 * Update :
 *  o 2006-02-23 Clode 重新改寫
 *
 -----------------------------------------
 */
 
#include <ansi.h>
#include <daemon.h>
#include <estate.h>
#include <message.h>
#include <time.h>
#include <money.h>
#include <login.h>

int query_player_tax(string id, int estatetax)
{
	int floors = ESTATE_D->query_total_estate_floors(id);
	
	return floors * estatetax;
}

void game_month_process()
{
	int owetax, *gametime = TIME_D->query_gametime_array();
	int estatetax;
	string city, citizen, money_unit, msg, cityidname;
	object player;
	int totalmoney;

	msg = "重生的世界"+CHINESE_D->chinese_number(gametime[YEAR])+"年"+CHINESE_D->chinese_number(gametime[MON]+1)+"月，";
	
	foreach( city in CITY_D->query_cities() )
	{
		estatetax = CITY_D->query_city_info(city, "estatetax");

		money_unit = MONEY_D->city_to_money_unit(city);
		cityidname = CITY_D->query_city_idname(city);

		foreach( citizen in CITY_D->query_citizens(city) )
		{
			reset_eval_cost();

			totalmoney = query_player_tax(citizen, estatetax);

			catch(player = load_user(citizen));

			if( !objectp(player) )
			{
				CHANNEL_D->channel_broadcast("sys", "稅務：無法載入 "+citizen+" 市民資料");
				continue;
			}

			// 上線未滿 15 天直接免稅
		        if( query("total_online_time", player) < 86400*15 )
			{
				if( userp(player) )
					tell(player, "\n\n"HIY+pnoun(2, player)+"上線未滿 15 天，免收任何稅款。\n\n\n"NOR);
				else
					destruct(player);
				
				continue;
			}

			owetax = query("owetax", player);
	
			if( owetax > 0 && userp(player) )
				tell(player, pnoun(2, player)+"共欠稅 "+owetax+" 次，本次將罰以 "+(owetax*3)+" 倍稅款。");

			if( !undefinedp(query("taxrate", player)) )
				totalmoney *= copy(query("taxrate", player));

			if( totalmoney > 0 && !MONEY_D->spend_money(citizen, money_unit, owetax ? owetax * 3 * totalmoney : totalmoney, FORCED_PAYMENT) )
			{
				owetax = addn("owetax", 1, player);
				
				if( owetax == 1 )
					CHANNEL_D->channel_broadcast("city", "市民"+player->query_idname()+"第 1 次付不起稅款，下次收稅將罰以 3 倍金額，連續累積 7 次欠稅則徵收所有房地產並開除市籍。", player);
				else if( owetax >= 7 )
				{
					if( CITY_D->is_mayor(city, player) && owetax <= 50 )
					{
						CHANNEL_D->channel_broadcast("city", "市長"+player->query_idname()+"共欠稅 "+owetax+" 次。", player);
						tell(player, HIY+pnoun(2, player)+"共欠稅超過 "+owetax+" 次。\n"NOR);
					}
					else
					{
						player->move(STARTROOM);
						CHANNEL_D->channel_broadcast("city", "市民"+player->query_idname()+"共欠稅 "+owetax+" 次，徵收所有房地產並開除市籍。", player);
						delete("owetax", player);
						delete("city", player);
						ESTATE_D->remove_estate(citizen);
						CITY_D->remove_citizen(citizen);
						tell(player, HIY+pnoun(2, player)+"共欠稅超過 "+owetax+" 次，徵收所有房地產並開除市籍。\n"NOR);
					}
					player->save();
					
					if( !userp(player) )
						destruct(player);
	
					continue;	
				}
				else
					CHANNEL_D->channel_broadcast("city", "市民"+player->query_idname()+"連續 "+owetax+" 次付不起稅款，下次收稅將罰以 "+(owetax*3)+" 倍金額，連續累積 7 次欠稅則徵收所有房地產並開除市籍。", player);
			}
			else
				delete("owetax", player);			
			
			CITY_D->change_assets(city, totalmoney);

			player->save();

			if( userp(player) )
				tell(player, "\n\n"HIY+pnoun(2, player)+"總共被徵收稅款 $"+money_unit+" "+NUMBER_D->number_symbol(owetax ? owetax * 3 * totalmoney : totalmoney)+"\n"NOR YEL"詳細稅收項目請利用 tax 指令查詢。\n\n\n"NOR);
			else
				destruct(player);
		}
	}
}

int query_city_tax(string city)
{
	int estatetax;
	string citizen;
	int totalmoney;
	object player;
	int money;

	if( !CITY_D->city_exist(city) ) return 0;

	estatetax = CITY_D->query_city_info(city, "estatetax");

	foreach( citizen in CITY_D->query_citizens(city) )
	{
		player = load_user(citizen);

		if( !objectp(player) ) continue;
		
		money = query_player_tax(citizen, estatetax);
		
		if( !undefinedp(query("taxrate", player)) )
			money *= copy(query("taxrate", player));

		totalmoney += money;
		
		if( !userp(player) )
			destruct(player);
	}
	
	return totalmoney;
}

void create()
{


}
string query_name()
{
	return "稅收系統(TAX_D)";
}
