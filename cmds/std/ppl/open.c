/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : open.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-08-24
 * Note   : 建築物開張指令，重要指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <location.h>
#include <map.h>
#include <message.h>
#include <roommodule.h>

inherit COMMAND;

string help = @HELP
    當你將牆壁組合成一個密合的地區時，便可以使用此指令來開張所需要的建築物
指令使用時必須要站在建築物的門上，無論是直門或橫門都可以。目前開放的建築物
有：小型商店、住宅... 等。

    在建築物內部時則可利用 open up 指令來往上加蓋樓層，每加蓋一層便可多出
一間房間，同時建築物愈高帶來的人氣與商機也愈高，並帶動城市繁榮與消費能力。

    在加蓋樓層完畢後可以利用以下指令來開通樓層間的通道，每開闢一個通道需花
費 10,000 的資金
      
    open north		- 開闢往北邊的通道
    open south		- 開闢往南邊的通道
    open west		- 開闢往西邊的通道
    open east		- 開闢往東邊的通道
      
相對於開張指令，也有所謂的關閉指令。

相關指令: blist, close
HELP;

#define WALL_TABLE	0
#define ROOM_TABLE	1

#define OPEN_DOOR_COST	10000

#define CORRESPONDING_DIRECTION	([ "north":"south", "south":"north", "west":"east", "east":"west" ])

void confirm_build_up(object me, object env, string owner, string nfile, string ofile, string bfile, int newfloor, int cost, string yn)
{
	string zone;
	object room, broom;
	string moneyunit;
	array loc = env->query_location();

	if( lower_case(yn) != "y" )
	{
		tell(me, pnoun(2, me)+"放棄往上加蓋樓層。\n");
		me->finish_input();
		return;
	}

	if( CITY_D->is_city_location(loc) )
	{
		zone = env->query_city();
		moneyunit = MONEY_D->city_to_money_unit(zone);
	
		if( belong_to_government(owner) )
		{
			if( count(CITY_D->query_assets(zone), "<", 0) )
			{
				tell(me, "政府資金已經不夠加蓋樓層了。\n");
				return me->finish_input();
			}

			CITY_D->change_assets(zone, "-"+cost);
		}
		else if( belong_to_enterprise(owner) )
		{
			moneyunit = MONEY_D->query_default_money_unit();
			
			if( !ENTERPRISE_D->change_assets(query("enterprise", me), "-"+cost) )
			{
				tell(me, "企業資金已經不夠加蓋樓層了。\n");
				return me->finish_input();
			}
		}
		else if( !me->spend_money(moneyunit, cost) )
		{
			tell(me, pnoun(2,me)+"身上的 $"+moneyunit+" 錢不夠了！！\n");
			return me->finish_input();
		}
	}
	else if( AREA_D->is_area_location(loc) )
	{
		moneyunit = MONEY_D->query_default_money_unit();
		if( !me->spend_money(moneyunit, cost) )
		{
			tell(me, pnoun(2,me)+"身上的 $"+moneyunit+" 錢不夠了！！\n");
			return me->finish_input();
		}
	}
	else error("發生錯誤。\n");

	room = load_object(nfile);
	broom = load_object(bfile);
	
	addn("floor", 1, broom);
	broom->save();
	
	set("exits/down", ofile, room);
	set("firstfloor", bfile, room);
	set("owner", owner, room);
	set("short", copy(query("short", broom)), room);
	set("long", copy(query("long", broom)), room);

	room->save();

	set("exits/up", nfile, env);
	env->save();
	

	if( CITY_D->is_city_location(loc) )
	{
		if( !(newfloor%5) )	
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"花費 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "HIW+newfloor+NOR" 樓。", me);
		
		if( !(newfloor%20) )
			CHANNEL_D->channel_broadcast("news", me->query_idname()+"在"+CITY_D->query_city_name(zone)+"之"+query("short", broom)+"建築突破 "HIW+newfloor+NOR" 樓，成為當地 "HIY+(newfloor/20)+NOR" 級地標。");
	
		//set("title/50",   HIC"城"NOR CYN"市"HIC"巨"NOR CYN"擘"NOR);
	//set("title/100",  HIW"城"NOR WHT"市"HIW"主"NOR WHT"宰"NOR);
	
		if( newfloor == 300 )
			me->add_title( HIW"帝"NOR WHT"國"HIW"主"NOR WHT"宰"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 240 )
			me->add_title( HIC"帝"NOR CYN"國"HIC"巨"NOR CYN"擘"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 210 )
			me->add_title( HIW"帝"NOR WHT"國"HIW"大"NOR WHT"廈"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 160 )
			me->add_title( HIR"普"NOR RED"利"NOR HIR"茲"NOR RED"克"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 120 )
			me->add_title( HIR"建築界"NOR RED"霸主"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 100 )
			me->add_title( HIR"首席"NOR RED"建築師"NOR, "建築樓層達 "+newfloor+" 層以上" );
		else if( newfloor == 50 )
			me->add_title( HIR"建築界"NOR RED"新秀"NOR, "建築樓層達 "+newfloor+" 層以上" );
			
		if( belong_to_government(owner) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"動用市府資產 "HIY+money(moneyunit, cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "+newfloor+" 樓。", me);
		else if( belong_to_enterprise(owner) )
			CHANNEL_D->channel_broadcast("ent", me->query_idname()+"動用企業資金 "HIY+money(moneyunit, cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "+newfloor+" 樓。", me);
		else
			tell(me, pnoun(2, me)+"花費 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "+newfloor+" 樓。\n");
	
		if( !wizardp(me) )
			TOP_D->update_top("building", base_name(broom), newfloor, owner, broom->query_room_name(1), zone);
	}
	else if( AREA_D->is_area_location(loc) )
	{
		CHANNEL_D->channel_broadcast("city", me->query_idname()+"花費 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "HIW+newfloor+NOR" 樓。", me);

		tell(me, pnoun(2, me)+"花費 "HIY"$"+moneyunit+" "+NUMBER_D->number_symbol(cost)+NOR" 將"+query("short", broom)+"往上加蓋至第 "+newfloor+" 樓。\n");
	}
	else error("發生錯誤。\n");

	me->finish_input();
}

void open_building(object me, string arg)
{
	int num;
	string owner, city, area, enterprise, moneyunit;
	int cost;
	mapping table;
	array loc = query_temp("location",me);
	array building_info;

	if( !arrayp(loc) || !environment(me)->is_maproom() )
		return tell(me, pnoun(2, me)+"必須在地圖上開張建築物。\n");
		
	arg = lower_case(arg);

	if( !BUILDING_D->has_building(arg) )
		return tell(me, "沒有 "+arg+" 這種建築物種類，查詢建築物列表請輸入 blist。\n");

	building_info = BUILDING_D->query_building(arg);

	if( building_info[ROOMMODULE_TESTING] && !wizardp(me) )
		return tell(me, "此建築物仍在封閉測試中，尚未開放建造。\n");
		
	if( CITY_D->is_city_location(loc) )
	{
		city = loc[CITY];
		num = loc[NUM];

		owner = CITY_D->query_coor_data(loc, "owner");

		moneyunit = MONEY_D->city_to_money_unit(city);
		
		if( !owner )
			return tell(me, "這塊土地不屬於任何人，"+pnoun(2, me)+"無法在此開張建築物。\n");
		
		// 政府土地
		if( belong_to_government(owner) )
		{
			if( !wizardp(me) )
			{
				if( !CITY_D->is_mayor_or_officer(loc[CITY], me) )
					return tell(me, pnoun(2, me)+"不是這座城市的市長或官員，無法使用政府土地建造建築物。\n");
			
				if( !(building_info[ROOMMODULE_BUILDINGTYPE] & GOVERNMENT) )
					return tell(me, "政府土地上只能建造政府建築。\n");
			}
		}
		// 企業土地
		else if( belong_to_enterprise(owner) )
		{
			enterprise = query("enterprise", me);
			moneyunit = MONEY_D->query_default_money_unit();
	
			if( !ENTERPRISE_D->is_member(enterprise, me->query_id(1)) )
				return tell(me, pnoun(2, me)+"不是這個企業集團的成員，無法使用企業土地建造建築物。\n");
			
			if( enterprise != owner[11..] )
				return tell(me, "這塊土地並不屬於"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"企業集團。\n");
				
			if( !(building_info[ROOMMODULE_BUILDINGTYPE] & ENTERPRISE) )
				return tell(me, "企業土地上只能建造企業建築。\n");
		}
		// 個人土地
		else if( belong_to_individual(owner) )
		{
			if( owner != me->query_id(1) )
				return tell(me, "這塊土地是屬於 "+owner+" 的，並不是"+pnoun(2, me)+"的土地。\n");
			
			if( !(building_info[ROOMMODULE_BUILDINGTYPE] & INDIVIDUAL) )
				return tell(me, "個人土地上只能建造個人建築。\n");
		}
		
		// 建造圍牆
		if( arg == "fence" )
		{
			int land_type = CITY_D->query_coor_data(loc, TYPE);
			string estate_type = ESTATE_D->query_loc_estate(loc)["type"];
			
			if( land_type != WALL && land_type != DOOR )
				return tell(me, pnoun(2, me)+"必須站在牆璧或門上來啟動圍牆功能。\n");
	
			if( estate_type == "fence" )
				return tell(me, "這裡已經是圍牆了。\n");
	
			if( estate_type != "land" )
				return tell(me, "這裡不能啟動圍牆功能。\n");			
			
			if( CITY_D->query_coor_data(loc, TYPE) == WALL )
				CITY_D->set_coor_data(loc, FLAGS, NO_MOVE);
	
			table = allocate_mapping(0);
			table["type"] = "fence";
			table["regtime"] = time();
			table["walltable"] = ({ save_variable(loc) });
			table["roomtable"] = allocate(0);
			
			CITY_D->set_coor_icon(loc, WHT+remove_ansi(CITY_D->query_coor_icon(loc))+NOR);
	
			ESTATE_D->set_estate(owner, table, "fence", city, num);
			
			msg("$ME在此地啟動了圍牆功能。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"在"+loc_short(loc)+NOR"建造了「"HIM"圍牆"NOR"」", me);
			return;
		}
		
		if( building_info[ROOMMODULE_MAXLIMIT] > 0 && ESTATE_D->query_owner_amount(owner, arg) >= building_info[ROOMMODULE_MAXLIMIT] )
			return tell(me, "此種建築每個政府/企業/玩家不得建造超過 "+building_info[ROOMMODULE_MAXLIMIT]+" 棟。\n");
		
		if( CITY_D->query_city_info(city, "age") < building_info[ROOMMODULE_AGE] )
			return tell(me, "目前這座城市的文明時代為「"+CITY_D->query_age_name(city)+"」，必須升級到「"+CITY_D->query_age_name(building_info[ROOMMODULE_AGE])+"」之後才能建造此類建築。\n");
	
		// 超過每座城市的建築數量限制
		if( building_info[ROOMMODULE_MAXBUILDINGLIMIT] && ESTATE_D->query_zone_amount(loc[CITY], loc[NUM], arg) >= building_info[ROOMMODULE_MAXBUILDINGLIMIT] )
			return tell(me, "這座城市的"+building_info[ROOMMODULE_SHORT]+"建築物數量已經到達 "+building_info[ROOMMODULE_MAXBUILDINGLIMIT]+" 棟的限制，無法再增加了。\n");

		// 規劃區檢查
		if( !(CITY_D->query_coor_data(loc, "region") & building_info[ROOMMODULE_REGION]) )
			return tell(me, building_info[ROOMMODULE_SHORT]+"不能建造在此種土地規劃區上。\n");
	
		// 開張地點檢查
		if( CITY_D->query_coor_data(loc, TYPE) != DOOR )
			return tell(me, pnoun(2, me)+"必須站在大門口來進行建築物的開張儀式。\n");
	}
	else if( AREA_D->is_area_location(loc) )
	{
		area = loc[AREA];
		num = loc[NUM];

		moneyunit = MONEY_D->query_default_money_unit();
		
		if( !(building_info[ROOMMODULE_REGION] & AREA_REGION) )
			return tell(me, building_info[ROOMMODULE_SHORT]+"不能建造在郊區。\n");
	}
	else
		error("發生錯誤。\n");
	
	// 牆壁邏輯
	if( !mapp(table = BUILDING_D->analyze_building_logic(loc)) )
		return tell(me, "此棟建築物的建造結構有問題，無法開張，請將錯誤部份重新建造。\n");

	foreach( string coor, mixed data in MAP_D->query_coor_range(loc, ROOM, 1) )
		if( data )
			return tell(me, pnoun(2, me)+"必須先關閉這棟建築物才能重新開張。\n");

	if( sizeof(table["roomtable"]) < building_info[ROOMMODULE_ROOMLIMIT] )
		return tell(me, building_info[ROOMMODULE_SHORT]+"最少需要"+CHINESE_D->chinese_number(building_info[ROOMMODULE_ROOMLIMIT])+"間房間，這棟建築只有"+CHINESE_D->chinese_number(sizeof(table["roomtable"]))+"間房間。\n");

	if( building_info[ROOMMODULE_OPENCOST] < 0 )
		return tell(me, "無法開張這種建築物。\n");

	// 計算開張花費
	cost = to_int(count(building_info[ROOMMODULE_OPENCOST], "*", sizeof(table["roomtable"])));

	if( CITY_D->is_city_location(loc) )
	{
		if( belong_to_government(owner) )
		{
			if( count(CITY_D->query_assets(city), "<", 0) && cost > 0 && arg != "cityhall")
				return tell(me, "政府資金已經不夠建造建築物了。\n");

			CHANNEL_D->channel_broadcast("news", me->query_idname()+"動用市府資產 "+HIY+money(moneyunit, cost)+NOR" 在"+CITY_D->query_city_idname(city, num)+"的"+CITY_D->position(loc[X], loc[Y])+"附近建造了一棟"+building_info[ROOMMODULE_SHORT]+"。\n");
			CITY_D->change_assets(city, "-"+cost);
		}
		else if( belong_to_enterprise(owner) )
		{
			if( ENTERPRISE_D->change_assets(enterprise, "-"+cost) )
				CHANNEL_D->channel_broadcast("news", "企業集團「"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"」花費企業資產 "+HIY+money(moneyunit, cost)+NOR" 在"+CITY_D->query_city_idname(city, num)+"建造了一棟"+building_info[ROOMMODULE_SHORT]+"。\n");		
			else if( me->spend_money(moneyunit, cost) )
			{
				ENTERPRISE_D->change_invest(enterprise, me->query_id(1), cost);
				CHANNEL_D->channel_broadcast("news", "企業集團「"+ENTERPRISE_D->query_enterprise_color_id(enterprise)+"」花費個人資金 "+HIY+money(moneyunit, cost)+NOR" 在"+CITY_D->query_city_idname(city, num)+"建造了一棟"+building_info[ROOMMODULE_SHORT]+"。\n");
				tell(me, "企業集團的現有資金不足以開張建築了，因此花費"+pnoun(2, me)+"的私人資金 "+money(moneyunit, cost)+" 進行開張。\n");
			}
			else
				return tell(me, "企業資產或"+pnoun(2, me)+"的私人現金都已經不足以支付 "+HIY+money(moneyunit, cost)+NOR+"。\n");
		}
		else if( me->spend_money(moneyunit , cost) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"花費 "+HIY+money(moneyunit, cost)+NOR" 在"+CITY_D->query_city_name(city, num)+loc_short(loc)+"附近建造了一棟"+building_info[ROOMMODULE_SHORT]+"。\n", me);
		else
			return tell(me, "開張"+building_info[ROOMMODULE_SHORT]+"需要基本裝潢與開張儀式費用"+HIY+money(moneyunit, cost)+NOR"。\n");
			
		BUILDING_D->materialize_city_building(me, owner, table, city, num, arg);
		
		CITY_D->save_city(city, num);
	}
	else
	{
		owner = me->query_id(1);

		if( me->spend_money(moneyunit , cost) )
			CHANNEL_D->channel_broadcast("city", me->query_idname()+"花費 "+HIY+money(moneyunit, cost)+NOR" 在"+AREA_D->query_area_name(area, num)+loc_short(loc)+"附近建造了一棟"+building_info[ROOMMODULE_SHORT]+"。\n", me);
		else
			return tell(me, "開張"+building_info[ROOMMODULE_SHORT]+"需要基本裝潢與開張儀式費用"+HIY+money(moneyunit, cost)+NOR"。\n");
			
		BUILDING_D->materialize_area_building(me, owner, table, area, num, arg);
		
		AREA_D->save_area(area, num);
	}
}

void open_up(object me)
{
	int x, y, z, newfloor;
	object env = environment(me);
	array loc = env->query_location();
	string owner, fhead, file = base_name(env), nfile;
	string type, bfile, money_unit;
	int cost;

	owner = query("owner", env);

	if( env->is_maproom() )
		return tell(me, pnoun(2, me)+"必須在建築物內部往上加蓋。\n");
	
	if( belong_to_government(owner) )
	{
		if( !CITY_D->is_mayor_or_officer(env->query_city(), me) )
			return tell(me, pnoun(2, me)+"不是這座城市的市長或官員。\n");
	}
	else if( belong_to_enterprise(owner) )
	{
		if( query("enterprise", me) != owner[11..] )
			return tell(me, pnoun(2, me)+"不是本企業集團的成員。\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這棟建築物的擁有者。\n");

	if( query("exits/up", env) )
		return tell(me, "上一層的樓層已經加蓋完成。\n");

	
	if( CITY_D->is_city_location(loc) )
		money_unit = MONEY_D->city_to_money_unit(env->query_city());
	else if( AREA_D->is_area_location(loc) )
		money_unit = MONEY_D->query_default_money_unit();
	else
		error("發生錯誤。\n");

	if( sscanf(file, "%s/room/%d_%d_%d_%s", fhead, x, y, z, type) == 5 || sscanf(file, "%s/room/%d_%d_%s", fhead, x, y, type) == 4 )
	{
		nfile = fhead+"/room/"+x+"_"+y+"_"+(z+1)+"_"+type;
		bfile = fhead+"/room/"+x+"_"+y+"_"+type;
		
		newfloor = z+2;
		
		if( newfloor > BUILDING_D->query_building_table()[type][ROOMMODULE_MAXFLOOR] )
			return tell(me, "此種建築物最多只能加蓋至此樓。\n");

		if( newfloor <= 100 )
		{	
			if( me->query_skill_level("architectonic") < newfloor )
				return tell(me, pnoun(2, me)+"的建築技術不足以再往上加蓋樓層。\n");
		}
		else if( newfloor <= 200 )
		{
			if( me->query_skill_level("architectonic") < 100 )
				return tell(me, pnoun(2, me)+"必須先將建築技術提升到最高等級。\n");
			
			if( me->query_skill_level("architectonic-adv")+100 < newfloor )
				return tell(me, pnoun(2, me)+"的巨型建築技能等級不足以再往上加蓋樓層。\n");
		}
		else
		{
			if( me->query_skill_level("architectonic-adv") < 100 )
				return tell(me, pnoun(2, me)+"必須先將巨型建築提升到最高等級。\n");
			
			if( me->query_skill_level("architectonic-high")+200 < newfloor )
				return tell(me, pnoun(2, me)+"的偉大建築技能等級不足以再往上加蓋樓層。\n");	
		}
		
		cost = BUILDING_D->query_floor_value(newfloor);
			
		tell(me, "加蓋第 "+newfloor+" 樓層需要資金 "HIY"$"+money_unit+" "+NUMBER_D->number_symbol(cost)+HIG"\n是否確定加蓋？(Y/N)"NOR);
		input_to((: confirm_build_up, me, env, owner, nfile, file, bfile, newfloor, cost:));
		
		return;
	}
	
	tell(me, "這棟建築物無法往上加蓋樓層。\n");
}

void open_door(object me, string arg)
{
	int x, y, z;
	object nextroom, env = environment(me);
	string type, fhead, file = base_name(env);
	string owner, money_unit = MONEY_D->city_to_money_unit(env->query_city());

	owner = query("owner", env);

	if( env->is_maproom() )
		return tell(me, pnoun(2, me)+"必須在建築物內部往上加蓋。\n");
	
	if( belong_to_enterprise(owner) )
	{
		if( query("enterprise", me) != owner[11..] )
			return tell(me, pnoun(2, me)+"不是本企業集團的成員。\n");
	}
	else if( owner != me->query_id(1) )
		return tell(me, pnoun(2, me)+"不是這棟建築物的擁有者。\n");

	if( query("exits/"+arg, env) )
		return tell(me, "那個方向的通道已經打開。\n");

	if( sscanf(file, "%s/room/%d_%d_%d_%s", fhead, x, y, z, type) != 5 )
		return tell(me, pnoun(2, me)+"只能在 2 樓以上的房間開闢通道。\n");
		
	if( arg == "south" ) y++;
	else if( arg == "north" ) y--;
	else if( arg == "west" ) x--;
	else if( arg == "east" ) x++;
	
	if( !objectp(nextroom = find_object(fhead+"/room/"+x+"_"+y+"_"+z+"_"+type)) )
		return tell(me, "那個方向的牆壁後面並沒有房間。\n");

	if( belong_to_enterprise(owner) )
	{
		if( !ENTERPRISE_D->change_assets(query("enterprise", me), "-"+OPEN_DOOR_COST) )
			return tell(me, "企業資金已經不夠開闢通道了。\n");
	}
	else if( !me->spend_money(money_unit, OPEN_DOOR_COST) )
		return tell(me, pnoun(2,me)+"身上的 $"+money_unit+" 錢不夠了！！\n");

	set("exits/"+arg, base_name(nextroom), env);
	set("exits/"+CORRESPONDING_DIRECTION[arg], file, nextroom);
		
	msg("$ME花費 $"+money_unit+" 10,000 將連接"+query("short", env)+env->query_coor_short()+"與"+query("short", nextroom)+nextroom->query_coor_short()+"的通道打開。\n", me, 0, 1);
	
	env->save();
	nextroom->save();
}

private void do_command(object me, string arg)
{
	if( !arg || !arg[0] ) 
		return tell(me, "請選擇"+pnoun(2, me)+"想要使用的建築物種類，查詢建築物列表輸入 blist。\n");

	arg = lower_case(arg);
	
	if( arg == "up" )
		open_up(me);
	else if( !undefinedp(CORRESPONDING_DIRECTION[arg]) )
		open_door(me, arg);
	else 
		open_building(me, arg);
}
