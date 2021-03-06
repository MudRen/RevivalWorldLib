/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : exchange.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-17
 * Note   : exchange 指令
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
        標準 exchange 指令。
HELP;

private void do_command(object me, string arg)
{
	mapping exchange_data = EXCHANGE_D->query_exchange_data();
	
	string msg = HIW"各主要城市匯率對照表如下：\n"NOR;
	
	msg += NOR WHT"──────────────────────────────────\n"NOR;
	
	foreach( string money_unit, float exchange in exchange_data )
	{
		msg += sprintf("%-30s %.6f\n", CITY_D->query_city_idname(MONEY_D->money_unit_to_city(money_unit)), exchange);
	}
	msg += NOR WHT"──────────────────────────────────\n"NOR;
	
	return tell(me, msg+"\n", CMDMSG);
}
