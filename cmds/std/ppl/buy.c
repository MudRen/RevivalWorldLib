/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : buy.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-5
 * Note   : buy 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
購買指令
    buy here			- 購買此塊土地
    buy here for enterprise	- 幫企業購買此塊土地
    
HELP;

private void do_command(object me, string arg)
{
	if( !arg ) return tell(me, pnoun(2, me)+"想買什麼？\n");

	if( arg == "here" || arg == "here for enterprise" )
	{
		string money_unit, owner;
		int price;
		array loc;
		object env = environment(me);
		mapping data;
		
		if( !env || !env->is_maproom() ) 
			return tell(me, pnoun(2, me)+"無法買下這土地。\n");
	
		loc = query_temp("location", me);
		
		if( load_object(MAP_D->query_map_system(loc)) != load_object(CITY_D) )
			return tell(me, pnoun(2, me)+"無法在荒郊野外購買土地。\n");

		if( !CITY_D->query_coor_data(loc, "region") )
			return tell(me, pnoun(2, me)+"只能購買已經經過市政府規劃的土地。\n");
		
		data = CITY_D->query_coor_data(loc);

		if( query("city", me) != query("city", env) )
			return tell(me, pnoun(2, me)+"不是"+CITY_D->query_city_idname(query("city", env))+"的市民，無法購買本市的土地。\n");

		if( wizardp(me) && loc[CITY] != "wizard" )
			return tell(me, "巫師僅能夠在"+CITY_D->query_city_idname("wizard")+"購買土地與建造建築物。\n");

		if( nullp(owner = data["owner"]) )
		{
			if( nullp(data[TYPE]) )
			{
				money_unit = MONEY_D->city_to_money_unit(loc[CITY]);
				price = to_int(data[VALUE]);
	
				if( arg == "here" )
				{
					if( !me->spend_money(money_unit, price) )
						return tell(me, "這塊地價值 $"+money_unit+" "+NUMBER_D->number_symbol(price)+" 元，"+pnoun(2, me)+"身上的 $"+money_unit+" 現金不夠了。\n");
					
					CITY_D->change_assets(query("city", env), price);
					
					if( CITY_D->set_coor_data(loc, "owner", me->query_id(1)) )
						tell(me, pnoun(2, me)+"花了 "+money(money_unit, price)+" 元買下了這塊土地。\n");
					
					CITY_D->set_coor_icon(loc, BWHT"□"NOR);
					
					// 設定房地產資訊
					ESTATE_D->set_land_estate(me->query_id(1), loc);
				}
				else if( arg == "here for enterprise" )
				{
					string enterprise = query("enterprise", me);
					
					money_unit = MONEY_D->query_default_money_unit();
	
					if( !enterprise )
						return tell(me, pnoun(2, me)+"並沒有加入任何企業集團。\n");
						
					if( !ENTERPRISE_D->enterprise_exists(enterprise) )
						return tell(me, pnoun(2, me)+"所加入的企業集團已經不存在。\n");
						
					if( ENTERPRISE_D->change_assets(enterprise, "-"+price) )
						CHANNEL_D->channel_broadcast("ent", me->query_idname()+"花費企業資金在"+loc_short(loc)+"購買了一塊企業土地。\n", me);
					else if( me->spend_money(money_unit, price) )
					{
						ENTERPRISE_D->change_invest(enterprise, me->query_id(1), price);
						CHANNEL_D->channel_broadcast("ent", me->query_idname()+"花費個人資金在"+loc_short(loc)+"購買了一塊企業土地。\n", me);
						tell(me, "企業集團的現有資金不足以購買土地了，因此花費"+pnoun(2, me)+"的私人資金 "+money(money_unit, price)+" 購買土地。\n");
					}
					else
						return tell(me, "企業資金與"+pnoun(2, me)+"的 $"+money_unit+" 現金都不足以購買這塊土地了。\n");
					
					CITY_D->set_coor_data(loc, "owner", "ENTERPRISE/"+enterprise);
					CITY_D->set_coor_icon(loc, BWHT"□"NOR);
					
					// 設定房地產資訊
					ESTATE_D->set_land_estate("ENTERPRISE/"+enterprise, loc);
				}			
				return;
			}
			else
				return tell(me, pnoun(2, me)+"無法購買這塊地。\n");
		}
		else if( owner == me->query_id(1) )
			return tell(me, "這塊地已經是"+pnoun(2, me)+"的了。\n");
		else if( belong_to_government(owner) )
			return tell(me, "這塊地是市政府用地，無法購買。\n");
		else if( belong_to_enterprise(owner) )
			return tell(me, "這塊地已被企業集團"+ENTERPRISE_D->query_enterprise_color_id(owner[11..])+"收購，無法購買。\n");
		else
			return tell(me, "這塊地已經被 "+capitalize(owner)+" 所購買。\n");
	}
	else
		return tell(me, pnoun(2, me)+"無法在這裡購買任何物品。\n");
}
