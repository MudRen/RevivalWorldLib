/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dellink.c
 * Author : Cookys@RevivalWorld
 * Date   : 2002-09-15
 * Note   : 建立與修改檔案標頭檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <ansi.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
        標準 dellink 指令
HELP;


private void command(object me, string arg)
{
	object env=environment(me);
	
	if( !is_command() ) return;

        if( !arg && !env->is_maproom() )
	        return tell(me, "請輸入要連結的出口名稱或方向。 \n", CMDMSG);
	        
	if( env->is_maproom() )
	{
		if( undefinedp(MAP_D->query_special_exit(query_temp("location",me))) )
			return tell(me, "此處並沒有特殊連結出口。\n", CMDMSG);
			
		MAP_D->del_special_exit(query_temp("location",me));
		return tell(me, "特殊出口已刪除。\n", CMDMSG);
	} else {
	
		if( undefinedp(query("exits",env )[arg] ) )
			return tell(me, HIC + arg + NOR +" 這個出口不存在。\n", CMDMSG);	
		
		delete("exits/"+arg,env);
		return tell(me, HIC+arg+NOR+"出口已刪除。\n", CMDMSG);
	}
	
}
