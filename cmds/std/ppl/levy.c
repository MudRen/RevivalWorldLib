/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : levy.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-04
 * Note   : 徵用土地
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
#include <location.h>

inherit COMMAND;

string help = @HELP
    市長或官員可使用此指令來強制徵收土地
levy here	- 徵收目前所站位置之土地，若土地為某建築物的一部份，將會關
                  閉整棟建築並讓原本的土地擁有者喪失此建築物的所有土地
levy north	- 徵收北邊的建築物或土地
levy south	- 徵收南邊的建築物或土地
levy east	- 徵收西邊的建築物或土地
levy west	- 徵收東邊的建築物或土地
levy -d		- 釋放已徵收的政府土地

一個月內曾上線的市民	- 只能徵收其戶外的地產(例如空地、農田、圍牆、景觀)
超過一個月未上線的市民	- 可徵收其所有種類的房地產
HELP;

private void confirm_levy(object me, array loc, string owner, string arg)
{
	if( arg != "y" && arg != "yes" )
		return me->finish_input();

	ESTATE_D->remove_estate(loc);
	
	CITY_D->set_coor_data(loc, "owner", "GOVERNMENT/"+loc[CITY]);
	CITY_D->set_coor_icon(loc, BMAG"．"NOR);

	ESTATE_D->set_land_estate("GOVERNMENT/"+loc[CITY], loc);
	
	if( owner )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"強制徵收了 "HIY+capitalize(owner)+NOR" 在 "+loc_short(loc)+" 的土地，供市府使用。\n", me);
	
	return msg("$ME強制徵收了這塊土地，以供市府使用。\n", me, 0, 1);
}

// 徵求給市長自己使用(暫不開放)
private void confirm_levy_to_mayor(object me, array loc, string owner, string arg)
{
	if( arg != "y" && arg != "yes" )
		return me->finish_input();
	
	if( !ESTATE_D->transfer_estate(owner, me->query_id(1), loc) )
		return tell(me, "房地產轉移發生錯誤，請通知巫師處理。\n");
	
	if( owner )
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"強制徵收了 "HIY+capitalize(owner)+NOR" 在 "+loc_short(loc)+" 的土地，由市長管理。\n", me);
	
	return msg("$ME強制徵收了這塊土地，由市長管理。\n", me, 0, 1);
}

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);
	object ownerob;

	if( !env || !env->is_maproom() ) 
		return tell(me, pnoun(2, me)+"無法徵收這塊土地。\n");
	
	loc = query_temp("location", me);

	if( arg == "north" )
		loc = arrange_city_location(loc[X], loc[Y]-1, loc[CITY], loc[NUM]);
	else if( arg == "south" )
		loc = arrange_city_location(loc[X], loc[Y]+1, loc[CITY], loc[NUM]);
	else if( arg == "west" )
		loc = arrange_city_location(loc[X]-1, loc[Y], loc[CITY], loc[NUM]);
	else if( arg == "east" )
		loc = arrange_city_location(loc[X]+1, loc[Y], loc[CITY], loc[NUM]);

	if( !CITY_D->valid_coordinate(loc[X], loc[Y], loc[CITY], loc[NUM]) )
		return tell(me, "那個方向的座標錯誤。\n");

	if( !wizardp(me) && !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"不是這座城市的市長或官員。\n");
		
	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( arg == "-d" )
	{
		if( !belong_to_government(owner) )
			return tell(me, "這塊地原本就不是市府用地。\n");
		
		if( CITY_D->query_coor_data(loc, TYPE) == ROAD )
			return tell(me, pnoun(2, me)+"必須先拆除道路。\n");
			
		if( CITY_D->query_coor_data(loc, TYPE) == BRIDGE )
			return tell(me, pnoun(2, me)+"必須先拆除橋樑。\n");

		if( sizeof(ESTATE_D->query_loc_estate(loc)["roomtable"]) )
			return tell(me, pnoun(2, me)+"必須先關閉這裡的建築物。\n");
			
		CITY_D->delete_coor_data(loc);

		ESTATE_D->remove_estate(loc);

		//CHANNEL_D->channel_broadcast("city", me->query_idname()+"釋出 "+loc_short(loc)+" 的政府土地。\n", me);
	
		return msg("$ME釋出了這塊政府土地，以供市民購買。\n", me, 0, 1);
	}


	if( owner )
	{
		if( belong_to_government(owner) )
			return tell(me, "這塊地已經是市府用地。\n");

		if( belong_to_enterprise(owner) )
			return tell(me, "無法徵收企業土地。\n");

		if( !CITY_D->is_mayor(loc[CITY], me) && !wizardp(me) )
			return tell(me, "這塊土地為 "+owner+" 所擁有，只有市長能夠徵收私人土地。\n");

		if( !sizeof(ESTATE_D->query_loc_estate(loc)["roomtable"]) )
		{
			tell(me, "是否確定無償強制徵收 "+capitalize(owner)+" 擁有的這塊價值 "HIY+money(MONEY_D->city_to_money_unit(loc[CITY]), ESTATE_D->query_estate_value(loc))+NOR" 元的土地？(yes/no)");
			input_to( (: confirm_levy, me, loc, owner :) );
			return;
		}
	
		if( !wizardp(me) && find_player(owner) && userp(find_player(owner)) )
			return tell(me, pnoun(2, me)+"無法強迫徵收線上玩家的房地產。\n");
			
		ownerob = load_user(owner);

		// 一個月未上線可徵收房地產
		if( !wizardp(me) && time() - query("last_on/time", ownerob) < 30*24*60*60 )
		{
			tell(me, ownerob->query_idname()+"一個月內曾經上線，不允許徵收房地產。\n");
			destruct(ownerob);
			return;
		}
		
		tell(me, "是否確定無償強制徵收"+ownerob->query_idname()+"這塊價值 "HIY+money(MONEY_D->city_to_money_unit(loc[CITY]), ESTATE_D->query_estate_value(loc))+NOR" 元的土地？(yes/no)");
		
		destruct(ownerob);
		
		input_to( (: confirm_levy, me, loc, owner :) );
			
		return;
	}
	
	if( !CITY_D->query_coor_data(loc, "region") )
		return tell(me, pnoun(2, me)+"只能徵收已設定好規劃區之土地。\n");

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
		case MOUNTAIN:
		case FOREST:
		case RIVER:
			return tell(me, "這塊地是自然地形，無法徵收。\n");
			break;
	}

	confirm_levy(me, loc, owner, "yes");
}
