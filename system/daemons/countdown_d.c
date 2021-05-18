/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : countdown_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-12-31
 * Note   : 倒數精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>

#define TARGET_TIME	1199116800
#define TARGET_NAME	NOR CYN"西元"HIC" 2008 "NOR CYN"年"NOR

void create()
{
	set_heart_beat(1);
}

void heart_beat()
{
	int now = time();
	
	if( now > TARGET_TIME )
	{
		set_heart_beat(0);
		return;
	}
	
	// 時間到達
	if( now == TARGET_TIME )
	{
		string msg = "";
		
		msg += "\n";
		msg += HIM"～～～～～"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"～～～～～\n\a"NOR;
		msg += HIM"　　～～～～～"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"～～～～～\n\a"NOR;
		msg += HIM"　　　　～～～～～"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"～～～～～\n\a"NOR;
		msg += HIM"　　　　　　～～～～～"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"～～～～～\n\a"NOR;
		msg += HIM"　　　　　　　　～～～～～"HIC"2008"HIG" HAPPY"HIY" NEW"HIR" YEAR"HIM"～～～～～\n\a"NOR;
		msg += "\n";
		shout(msg);
	}
	// 每秒倒數
	else if( TARGET_TIME - now <= 10 )
	{
		shout("\n距離"+TARGET_NAME+"倒數 "HIR+(TARGET_TIME-now)+NOR" 秒。\a\n\n");
	}
	// 每 5 秒倒數
	else if( TARGET_TIME - now <= 60 )
	{
		if( !(now % 5 ) )
			shout("\n距離"+TARGET_NAME+"只剩 "HIY+(TARGET_TIME-now)+NOR" 秒。\a\n\n");
	}
	// 每分鐘倒數
	else if( TARGET_TIME - now <= 30*60 )
	{
		if( !(now % 60 ) )
			shout("\n距離"+TARGET_NAME+"還有 "+(TARGET_TIME-now)/60+" 分鐘。\n\n");
	}
	// 每 5 分鐘倒數
	else if( TARGET_TIME - now <= 60*60 )
	{
		if( !(now % (5*60) ) )
			shout("\n距離"+TARGET_NAME+"還有 "+(TARGET_TIME-now)/60+" 分鐘。\n\n");
	}
	// 每小時倒數 
	else if( TARGET_TIME - now <= 24*60*60 )
	{
		if( !(now % (60*60)) )
			shout("\n距離"+TARGET_NAME+"還有 "+(TARGET_TIME-now)/(60*60)+" 小時。\n\n");
	}
	// 每天倒數
	else
	{
		if( !(now % (24*60*60)) )
			shout("\n距離"+TARGET_NAME+"還有 "+(TARGET_TIME-now)/(24*60*60)+" 天。\n\n");
	}
}

int query_lack_time()
{
	return TARGET_TIME - time();
}

string query_name()
{
	return "倒數系統(COUNTDOWN_D)";
}
