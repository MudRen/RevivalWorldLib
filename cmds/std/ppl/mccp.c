/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pplcmd_example.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-00-00
 * Note   : 玩家指令範例
 * Update :
 *  o 2005-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
指令說明:
    檢查玩家的程式是否有啟動 MCCP(Mud Client Compression Protocol)功能
此功能可以加速網路傳輸速度
指令簡介:

指令格式:

相關文件:
   
相關指令:

HELP;

#define MCCP_SUPPORT	1

private void do_command(object me, string arg)
{
#ifdef MCCP_SUPPORT

	if( wizardp(me) && arg == "-l")
	{
		string m="\n";
		int j=1,i=0;
		foreach(object ob in users()) 
			m+=sprintf("%3d.%-30s%3s\n", j++, ob->query_idname(), compressedp(ob)?(i++,HIG"ON"NOR):HIR"OFF"NOR); 
		tell(me,m+"\n\nMCCP COUNT:"+HIG+ i +NOR+"\n");
	} else{
		tell(me,"你的 MCCP 狀態為：" + (compressedp(me)?(HIG"ON"NOR):HIR"OFF"NOR) +"\n");
	}
	tell(me,"現在共 " +HIC+ sizeof(filter(users(),(: compressedp($1) :))) + NOR+"/"+ sizeof(users()) +" 個使用者開啟 MCCP。\n");
#else
	tell(me, "目前 Driver 不支援此功能。\n");

#endif
}	
