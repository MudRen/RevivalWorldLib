/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : close.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-04
 * Note   : 建築物關閉指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <map.h>
#include <location.h>
#include <building.h>

inherit COMMAND;

string help = @HELP
關閉土地功能、建築物、或拆除上層房間的指令。

close		- 關閉您目前所在地的建築物或土地功能(必須站在建築物門口或該土地上)
close north	- 關閉北邊的建築物或土地功能
close south	- 關閉南邊的建築物或土地功能
close east	- 關閉西邊的建築物或土地功能
close west	- 關閉東邊的建築物或土地功能

close up	- 拆除上層的房間

HELP;

#define SELL_PERCENT	20

#define BUILDING_TYPE		2

private void confirm_close(object me, array loc, string owner, string enterprise, string arg)
{
	int percent;
	int value;
	mapping building_table;
	mapping estdata;
	string moneyunit = MONEY_D->city_to_money_unit(loc[CITY]);
	
	if( lower_case(arg) != "y" )
		tell(me, "取消關閉建築物。\n");
	else 
	{
		building_table = BUILDING_D->query_building_table();
		estdata = ESTATE_D->query_loc_estate(loc);
		
		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;

		if( estdata["roomtable"] )
			value = ESTATE_D->query_estate_value(loc, 1) * percent / 100;

		ESTATE_D->unlink_estate(loc);
		
		CITY_D->save_city(loc[CITY], loc[NUM]);

		if( belong_to_enterprise(owner) )
		{
			if( estdata["roomtable"] )
			{	
				CHANNEL_D->channel_broadcast("news", "企業集團「"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"」關閉了在"+CITY_D->query_city_idname(loc[CITY], loc[NUM])+"的一棟"+building_table[estdata["type"]][BUILDING_SHORT]+"。\n");
				tell(me, pnoun(2, me)+"以 "+percent+"% 價錢回收關閉了這處房地產，拿回 "HIY+money(moneyunit, value)+NOR" 元繳回企業資金。\n");
				ENTERPRISE_D->change_assets(enterprise, value);
			}
			else
				tell(me, pnoun(2, me)+"關閉了此處的建築物或景觀。\n");
		}
		else if( belong_to_individual(owner) )
		{
			if( estdata["roomtable"] )
			{
				CHANNEL_D->channel_broadcast("city", me->query_idname()+"關閉了在"+loc_short(loc)+NOR"附近的"+building_table[estdata["type"]][BUILDING_SHORT]+"。\n", me);
				tell(me, pnoun(2, me)+"以 "+percent+"% 價錢回收關閉了這處房地產，拿回 "HIY+money(moneyunit, value)+NOR" 元。\n");
				me->earn_money(moneyunit, value);
			}
			else
				tell(me, pnoun(2, me)+"關閉了此處的建築物或景觀。\n");
		}
		else
			tell(me, pnoun(2, me)+"關閉了此處的建築物或景觀。\n");
	}
	
	me->finish_input();
}

private void confirm_close_up(object me, object env, object up_room, string moneyunit, int value, string arg)
{
	object first_floor_room;
	object connect_room;
	object master = up_room->query_master();
	string exit, exit2;
	mixed direction, direction2;
	string up_room_basename = base_name(up_room);

	if( arg != "yes" && arg != "y" )
	{
		tell(me, "取消拆除工作。\n");
		return me->finish_input();
	}
	
	if( !objectp(first_floor_room = load_object(query("firstfloor", up_room))) )
		error("樓層資料發生錯誤。");
		
	addn("floor", -1, first_floor_room);
	first_floor_room->save();
	
	up_room->remove_master();
	master->remove_slave(up_room_basename);
	master->save();

	foreach(exit, direction in query("exits", up_room))
	{
		if( !stringp(direction) || !file_exists(direction) ) continue;
			
		connect_room = load_object(direction);
		
		if( mapp(query("exits", connect_room)) )
		foreach(exit2, direction2 in query("exits", connect_room))
		{
			if( direction2 == up_room_basename )
				delete("exits/"+exit2, connect_room);
				
			connect_room->save();
		}
	}
	
	destruct(up_room);
	rm(up_room_basename+__SAVE_EXTENSION__);

	me->earn_money(moneyunit, value);
	tell(me, pnoun(2, me)+"將上層房間拆除掉了，拿回 "HIY+money(moneyunit, value)+NOR" 元。\n");
	me->finish_input();
	
	if( !wizardp(me) )
		TOP_D->update_top("building", base_name(first_floor_room), env->query_floor(), query("owner", env), first_floor_room->query_room_name(1), env->query_city());
}


private void do_command(object me, string arg)
{
	string owner, enterprise;
	array loc = copy(query_temp("location",me));
	mapping estdata;
	object env = environment(me);

	if( arg == "up" )
	{
		object up_room;
		int value; 
		string moneyunit = env->query_money_unit();
		int percent;

		if( !wizardp(me) && query("owner", env) != me->query_id(1) )
			return tell(me, pnoun(2, me)+"不是這棟建築物的擁有者。\n");
		
		if( !query("exits/up", env) )
			return tell(me, "這裡樓上沒有房間。\n");
		
		if( !objectp(up_room = load_object(query("exits/up", env))) )
			error("無法載入上層房間。");
			
		if( query("exits/up", up_room) )
			return tell(me, "上層的房間並不是頂樓，"+pnoun(2, me)+"必須從最頂樓一層一層拆下來。\n");
		
		value = BUILDING_D->query_floor_value(up_room->query_floor());
				
		percent = SELL_PERCENT + me->query_skill_level("estaterebate")/2;
		value = value * percent / 100;
		
		tell(me, "拆除上層房間可回收 "+percent+"% 的金額共 "HIY+money(moneyunit, value)+NOR"，確定拆除上面這一層房間嗎(y/n)？");
		
		input_to((: confirm_close_up, me, env, up_room, moneyunit, value :));
		
		return;
	}
	
        if( !arrayp(loc) || !env || !env->is_maproom() )
	{
		if( query("owner", env) != me->query_id(1) )
			return tell(me, pnoun(2, me)+"無法關閉這個建築物。\n");
		else
			return tell(me, pnoun(2, me)+"必須到建築物門口進行關閉的動作。\n");
	}
	
	if( arg == "north" )
		loc = arrange_city_location(loc[X], loc[Y]-1, loc[CITY], loc[NUM]);
	else if( arg == "south" )
		loc = arrange_city_location(loc[X], loc[Y]+1, loc[CITY], loc[NUM]);
	else if( arg == "west" )
		loc = arrange_city_location(loc[X]-1, loc[Y], loc[CITY], loc[NUM]);
	else if( arg == "east" )
		loc = arrange_city_location(loc[X]+1, loc[Y], loc[CITY], loc[NUM]);
	
	if( !MAP_D->valid_coordinate(loc) )
		return tell(me, "那個方向的座標錯誤。\n");

	if( CITY_D->is_city_location(loc) )
	{
		owner = CITY_D->query_coor_data(loc, "owner");
	
		if( !owner )
			return tell(me, "這塊土地不屬於"+pnoun(2, me)+"。\n");
		else if( belong_to_government(owner) )
		{
			if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
				return tell(me, "這塊地是市政府資產，"+pnoun(2, me)+"沒有權利關閉這裡的建築物或景觀。\n");
		}
		else if( belong_to_enterprise(owner) )
		{
			enterprise = query("enterprise", me);
			
			if( enterprise != owner[11..] )
				return tell(me, pnoun(2, me)+"並不是"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"企業集團的成員，沒有權利關閉這裡的建築物或景觀。\n");
		}
		else if( owner != me->query_id(1) )
			return tell(me, "這塊地是 "+capitalize(owner)+" 的私人土地，"+pnoun(2, me)+"沒有權利關閉這裡的建築物或景觀。\n");
	}

	estdata = ESTATE_D->query_loc_estate(loc);

	if( !mapp(estdata) )
		return tell(me, "這塊土地上並沒建築物或景觀可以關閉。\n");
		
	if( estdata["type"] == "land" )
		return tell(me, "荒地不需要關閉。\n");

	tell(me, HIR"注意："HIY"建築物關閉後，將會清除所有內容資料與土地等級。\n"NOR HIR"請確認是否有連鎖主店在其中，存貨資料皆會被一併刪除。\n"NOR"確定是否關閉此建築物？(y/n):");
	input_to( (: confirm_close, me, loc, owner, enterprise :) );
}
