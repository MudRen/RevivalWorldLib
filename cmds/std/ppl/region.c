/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : region.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-06
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
    市長或官員可使用此指令來規劃城市分區
    	region 農	- 將土地規劃成農業區
    	region 工	- 將土地規劃成工業區
    	region 商	- 將土地規劃成商業區
    	region -d	- 取消土地規劃
HELP;

#define REGION_COST	500000

private void do_command(object me, string arg)
{
	string owner;
	array loc;
	object env = environment(me);

	if( !arg ) 
		return tell(me, "請輸入正常的規劃區名稱(農, 工, 商)。\n");

	if( !env || !env->is_maproom() ) 
		return tell(me, pnoun(2, me)+"無法規劃這塊土地。\n");
	
	loc = query_temp("location", me);

	if( !CITY_D->city_exist(loc[CITY]) )
		return tell(me, "這塊地圖上是不能進行規劃區的畫分的。\n");

	if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
		return tell(me, pnoun(2, me)+"不是這座城市的市長或官員。\n");

	owner = CITY_D->query_coor_data(loc, "owner");
	
	if( owner )
	{
		if( belong_to_government(owner) )
			return tell(me, "這塊地已經是政府徵收的土地，無法重新規劃。\n");
		else if( belong_to_enterprise(owner) )
			return tell(me, "這塊地已經是 "+owner[11..]+" 的土地，無法重新規劃。\n");
		return tell(me, "這塊地已經是 "+owner+" 的土地，無法重新規劃。\n");
	}

	switch( CITY_D->query_coor_data(loc, TYPE) )
	{
		case MOUNTAIN:
		case FOREST:
		case RIVER:
			return tell(me, "這塊地是自然地形，無法進行規劃區畫分。\n");
	}

	switch( arg )
	{
		case "農":
			if( CITY_D->query_coor_data(loc, "region") == AGRICULTURE_REGION )
				return tell(me, "這塊土地原本就是農業區。\n");
			
			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "市府資金只剩下 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"，不足以重新規劃土地了！\n");
		
			
			CITY_D->set_coor_data(loc, "region", AGRICULTURE_REGION);
			CITY_D->set_coor_data(loc, VALUE, "100000");
			CITY_D->set_coor_icon(loc, BGRN"．"NOR);
			
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"將 "+loc_short(loc)+" 的土地規劃為"HIG HBGRN"農業區"NOR"。\n", me);
			msg("$ME支付了市政府資產 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" 將此地規劃為農業區。\n", me, 0, 1);
			
			break;
		case "工":
			if( CITY_D->query_coor_data(loc, "region") == INDUSTRY_REGION )
				return tell(me, "這塊土地原本就是工業區。\n");
			
			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "市府資金只剩下 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"，不足以重新規劃土地了！\n");
		
			CITY_D->set_coor_data(loc, "region", INDUSTRY_REGION);
			CITY_D->set_coor_data(loc, VALUE, "200000");
			CITY_D->set_coor_icon(loc, BYEL"．"NOR);
			
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"將 "+loc_short(loc)+" 的土地規劃為"HIY HBYEL"工業區"NOR"。\n", me);
			msg("$ME支付了市政府資產 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" 將此地規劃為工業區。\n", me, 0, 1);
			break;
			
		case "商":
			if( CITY_D->query_coor_data(loc, "region") == COMMERCE_REGION )
				return tell(me, "這塊土地原本就是商業區。\n");

			if( !CITY_D->change_assets(loc[CITY], -REGION_COST) )
				return tell(me, "市府資金只剩下 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+CITY_D->query_assets(loc[CITY])+"，不足以重新規劃土地了！\n");
		
			CITY_D->set_coor_data(loc, "region", COMMERCE_REGION);
			CITY_D->set_coor_data(loc, VALUE, "300000");
			CITY_D->set_coor_icon(loc, BCYN"．"NOR);
			
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"將 "+loc_short(loc)+" 的土地規劃為"HIC HBCYN"商業區"NOR"。\n", me);
			msg("$ME支付了市政府資產 $"+MONEY_D->city_to_money_unit(loc[CITY])+" "+REGION_COST+" 將此地規劃為商業區。\n", me, 0, 1);
			break;
		
		case "-d":
			if( !CITY_D->query_coor_data(loc, "region") )
				return tell(me, "這塊地原本就沒有規劃。\n");
			
			CITY_D->delete_coor_data(loc, "region");
			CITY_D->delete_coor_data(loc, 6);
			CITY_D->delete_coor_icon(loc);
			
			//CHANNEL_D->channel_broadcast("city", me->query_idname()+"取消了 "+loc_short(loc)+" 原有的土地規劃。\n", me);
			msg("$ME取消了此地原有的土地規劃。\n", me, 0, 1);
			break;
			
		default:
			return tell(me, "請輸入正常的規劃區名稱(農, 工, 商)。\n");
	}
}
