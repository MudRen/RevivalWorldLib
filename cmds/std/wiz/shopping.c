/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : shopping.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-30
 * Note   : shopping 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
	標準 shopping 指令。
HELP;

private void command(object me, string arg)
{
	int num, time = time();
	string city, msg;
	
	if( !is_command() ) return;
	
	msg = HIW"城市              總共金額      目前金額      預期金額      下次更新  比例\n"NOR;
	msg +=NOR WHT"────────────────────────────────────────\n"NOR;


	foreach(city in CITY_D->query_cities() )
	{
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			msg += sprintf("%-18s%-14d%-14d%-14d%-10d%d\n",
				CITY_D->query_city_id(city, num),
				SHOPPING_D->query_shopping_info(city, num, "totalmoney"),
				SHOPPING_D->query_shopping_info(city, num, "money"),
				SHOPPING_D->query_shopping_info(city, num, "predictmoney"),
				3600 - (time - SHOPPING_D->query_refresh_time()),
				SHOPPING_D->shopping_cost_ratio(city, num),
				);
		}
	}
	msg +=NOR WHT"────────────────────────────────────────\n"NOR;
	tell(me, msg, CMDMSG);
}