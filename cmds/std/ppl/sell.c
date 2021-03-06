/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sell.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-8-22
 * Note   : sell 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>
#include <roommodule.h>

inherit COMMAND;

string help = @HELP
販賣土地的指令

sell here		- 將目前所在的這塊土地賣出

HELP;

#define SELL_PERCENT	20

#define BUILDING_WALL_TABLE	4
#define BUILDING_ROOM_TABLE	5

void sell_land(object me, string owner, array loc, mapping data, string moneyunit, int value, string enterprise, string arg)
{
	int percent;
	mapping building_table = BUILDING_D->query_building_table();
	mapping estdata = ESTATE_D->query_loc_estate(loc);
	
	if( arg != "y" )
	{
		tell(me, pnoun(2, me)+"決定不賣出此地。\n");
		return me->finish_input();
	}
	
        if(!estdata) return tell(me, "土地資料錯誤，請通知巫師處理。\n");

	value = ESTATE_D->query_estate_value(loc);
	
	percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;
	value = value * percent / 100;

	if( belong_to_enterprise(owner) )
	{
		ENTERPRISE_D->change_assets(enterprise, value);
		
		if( sizeof(estdata["roomtable"]) )
			CHANNEL_D->channel_broadcast("news", "企業集團「"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"」關閉並售出在"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"的一棟"+building_table[estdata["type"]][ROOMMODULE_SHORT]+"。\n");
		
		tell(me, pnoun(2, me)+"以整座房地產之 "+percent+"% 價錢賣掉了這處房地產，拿回 "HIY+money(moneyunit, value)+NOR" 元繳回企業資金。\n");
	}
	else
	{
		me->earn_money(moneyunit, value);
		
		if( sizeof(estdata["roomtable"]) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"關閉並售出在"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"的一棟"+building_table[estdata["type"]][ROOMMODULE_SHORT]+"。\n", me);
		
		tell(me, pnoun(2, me)+"以整座房地產之 "+percent+"% 價錢賣掉了這處房地產，拿回 "HIY+money(moneyunit, value)+NOR" 元。\n");
	}
	
	ESTATE_D->remove_estate(loc);
	
	CITY_D->save_city(loc[CITY], loc[NUM]);
	me->finish_input();
}

private void do_command(object me, string arg)
{
	int percent;
	mapping data;
	string moneyunit, enterprise;
	array loc = query_temp("location", me);
	object env = environment(me);
	int value;
	
	if( !arg ) return tell(me, pnoun(2, me)+"想賣什麼？\n");

	if( arg == "here" )
	{
		if( !arrayp(loc) || !env || !env->is_maproom() || !env->query_city() ) 
			return tell(me, "這塊土地無法買賣。\n");
	
		if( !mapp(data = CITY_D->query_coor_data(loc)) )
			return tell(me, "土地資料錯誤，請通知巫師處理。\n");
		
		moneyunit = MONEY_D->city_to_money_unit(loc[CITY]);

		if( belong_to_enterprise(data["owner"]) )
		{
			enterprise = query("enterprise", me);
			
			if( enterprise != data["owner"][11..] )
				return tell(me, pnoun(2, me)+"不是"+ENTERPRISE_D->query_enterprise_color_id(data["owner"][11..])+"企業集團的成員，無權販賣此塊土地。\n");
			
			moneyunit = MONEY_D->query_default_money_unit();
		}
		else if( data["owner"] != me->query_id(1) )
			return tell(me, "這塊地並不是"+pnoun(2, me)+"的。\n");
		
		if( data["claimed"] && ( !me->query_highest_skill() || me->query_skill_level(me->query_highest_skill()) < 10) )
			return tell(me, "這塊土地是"+pnoun(2, me)+"認領的，"+pnoun(2, me)+"必須有任一技能超過 10 級才能賣出此塊土地。\n");

		value = ESTATE_D->query_estate_value(loc);

		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;
		tell(me, "這處房地產價值 "HIY+money(moneyunit, value)+NOR"，若有建築物在上面將會以 "+percent+"% 價格全部售出，"+pnoun(2, me)+"確定要賣出此地？(y/n)");
		input_to( (: sell_land, me, data["owner"], loc, data, moneyunit, value, enterprise :) );
	}
	else
		return tell(me, pnoun(2, me)+"想要賣什麼？\n");
}
