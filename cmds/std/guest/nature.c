/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nature.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-12
 * Note   : nature 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <time.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <nature.h>
#include <nature_data.h>

inherit COMMAND;

string help = @HELP
	標準 nature 指令。
HELP;

private void command(object me, string arg)
{
	string msg;
	mapping nature = NATURE_D->query_nature();
	
	if( !is_command() ) return;

	msg = HIW"目前的季節為「"+TIME_D->season_period(TIME_D->query_gametime_array()[MON])+"天」\n，各主要城市自然現象如下：\n"NOR;
	msg += "城市/區域                天氣  溫度  水氣  風速  持續時間\n";
	msg += "───────────────────────────────────\n";
	
	foreach( object env, mapping data in nature )
		msg += sprintf("%-25s%-6s%-6d%-6d%-6d%-10d\n", CITY_D->query_city_id(query("city", env), query("num", env))||file_name(env), data[CURRENT_WEATHER][WEATHER_CHINESE], data[TEMPERATURE], data[HUMIDITY], data[WIND], data[NEXTTIME_ARRANGE] );

	msg += "───────────────────────────────────\n";
	me->more(msg);
}








