/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : web.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-11
 * Note   : web 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
    設定自己的頭像圖片網路位址，但您必須先在該網址上放置您的頭像
圖片，也可以直接輸入網路上已經存在的各種圖片網址。
    
web avatar 圖檔網址		設定您的頭像圖片網址
web avatar -d			取消頭像設定
web homepage 網站網址		設定您的個人網站網址
web homepage -d			取消網站設定

設定完後便可以到 http://www.revivalworld.org/rw/online/who 觀看大家的資料

HELP;

private void do_command(object me, string arg)
{
	string option, value;

	if( !arg )
	{
		tell(me, pnoun(2, me)+"的頭像圖片網址："+(query("web/avatar", me)||"未設定")+"\n");
		tell(me, pnoun(2, me)+"的個人網站網址："+(query("web/homepage", me)||"未設定")+"\n");
		return;
	}
	
	if( sscanf(arg, "%s %s", option, value) != 2 )
		return tell(me, "請輸入正確的指令格式。\n");
		
	if( option == "avatar" )
	{
		if( value == "-d" )
		{
			delete("web/avatar", me);
	
			// 即時更新
			MYSQL_SCHEDULE_D->refresh_who();
			
			return tell(me, pnoun(2, me)+"將頭像圖片取消了。\n");
		}
		
		if( strsrch(value, "'") != -1 )
			return tell(me, "網址中不能擁有 ' 字元。\n");
	
		if( noansi_strlen(value) > 512 )
			return tell(me, "網址最多只能有 512 個字母喔。\n");
		
		set("web/avatar", value, me);

		
		tell(me, pnoun(2, me)+"將頭像圖片網址設定為「"+value+"」。\n"NOR);
	
		// 即時更新
		MYSQL_SCHEDULE_D->refresh_who();
	}
	else if( option == "homepage" )
	{
		if( value == "-d" )
		{
			delete("web/homepage", me);
	
			// 即時更新
			MYSQL_SCHEDULE_D->refresh_who();
			
			return tell(me, pnoun(2, me)+"將個人網站取消了。\n");
		}
		
		if( strsrch(value, "'") != -1 )
			return tell(me, "網址中不能擁有 ' 字元。\n");
	
		if( noansi_strlen(value) > 512 )
			return tell(me, "網址最多只能有 512 個字母喔。\n");
		
		set("web/homepage", value, me);

		
		tell(me, pnoun(2, me)+"將個人網站網址設定為「"+value+"」。\n"NOR);
	
		// 即時更新
		MYSQL_SCHEDULE_D->refresh_who();
	}
	else
		return tell(me, "請輸入正確的指令選項。\n");
}