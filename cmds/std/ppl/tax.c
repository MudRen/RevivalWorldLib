/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tax.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-09-04
 * Note   : tax 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
    查詢稅收狀況

tax			- 查詢自己的稅收狀況
tax '玩家ID'    	- 查詢某位市民的稅收狀況(市長官員指令)
tax city '城市ID'	- 查詢整座城市的稅收狀況(市長官員指令)
    
HELP;

private void do_command(object me, string arg)
{
	int estatetax;
	string city, money_unit, msg, id, query_city;
	object player;
	int money;

	city = query("city", me);

	if( !city )
		return tell(me, pnoun(2, me)+"未加入任何城市，無稅收資料。\n");

	if( !arg )
		id = me->query_id(1);
	else if( sscanf(arg, "city %s", query_city) == 1 )
	{
		int total_money;
		
		money_unit = MONEY_D->city_to_money_unit(query_city);
		estatetax = CITY_D->query_city_info(query_city, "estatetax");

		if( !wizardp(me) && !CITY_D->is_mayor_or_officer(query_city, me) )
			return tell(me, "只有市長或官員可以查詢這座城市的稅收情形。\n");
		
		msg = CITY_D->query_city_idname(query_city)+"的所有稅收狀況如下：\n";
		msg += "─────────────────────────────────\n";
		msg += "名稱                   稅收倍率         稅收\n";
		msg += "─────────────────────────────────\n";

		foreach(string citizen in CITY_D->query_citizens(query_city))
		{
			money = TAX_D->query_player_tax(citizen, estatetax);
			
			player = load_user(citizen);
			
			if( !undefinedp(query("taxrate", player)) )
				money *= copy(query("taxrate", player));

			total_money += money;

			msg += sprintf("%-30s%-10d"HIY"%s"NOR"\n", player->query_idname(), undefinedp(query("taxrate", player))?1:query("taxrate", player), money(money_unit, money));
			
			if( !userp(player) )
				destruct(player);
		}

		msg += "─────────────────────────────────\n";
		msg += sprintf("%-40s"HIY"%s"NOR"\n", CITY_D->query_city_idname(city)+"稅收總收入", money(money_unit, total_money));

		return me->more(msg);
	}
	else if( wizardp(me) )
		id = arg;
	else if( CITY_D->is_mayor_or_officer(city, me) )
	{
		if( !CITY_D->is_citizen(arg, city) )
			return tell(me, capitalize(arg)+" 不是本城市的市民。\n");
		
		id = arg;
	}
	else
		return tell(me, pnoun(2, me)+"沒有權利查詢其他人的稅收資料。\n");
	
	if( !objectp(player = load_user(id)) )
		return tell(me, "沒有 "+id+" 這位玩家。\n");

	city = query("city", player);

	money_unit = MONEY_D->city_to_money_unit(city);
	estatetax = CITY_D->query_city_info(city, "estatetax");
	money = TAX_D->query_player_tax(id, estatetax);

	if( !undefinedp(query("taxrate", player)) )
		money *= copy(query("taxrate", player));
		
	msg = (player == me ? pnoun(2, me):player->query_idname())+"的稅收資料如下：\n";	
	msg += sprintf("稅收項目      土地+房屋數 x 稅率   x 稅收倍率   =   稅收總額\n"
	NOR WHT"─────────────────────────────────\n"
	NOR HIY"地產稅        %-11d x %-6d x %-10d =   %s\n"
	NOR WHT"─────────────────────────────────\n\n"NOR
	, ESTATE_D->query_total_estate_floors(id)
	, estatetax
	, undefinedp(query("taxrate", player)) ? 1 : query("taxrate", player)
	, money(money_unit, money));
	
	if( !userp(player) )
		destruct(player);

	tell(me, msg);
}