/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : uptime.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-2
 * Note   : uptime 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <mudlib.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
查詢啟動時間

這個指令可以查詢重生的世界距離啟動的時間

相關指令:
HELP;

private void do_command(object me, string arg)
{
	tell(me, MUD_FULL_NAME"\n啟動日期：" + TIME_D->time_description("公元", TIME_D->analyse_time(time()-uptime())) + "。\n", CMDMSG);
	tell(me, "運作時間："+CHINESE_D->chinese_period(uptime())+"。\n", CMDMSG);
}