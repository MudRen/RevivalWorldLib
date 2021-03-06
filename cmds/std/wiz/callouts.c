/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : callouts.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : callouts 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
 
#include <feature.h>
#include <ansi.h>

inherit COMMAND;

string help = @HELP
	標準 callouts 指令
HELP;

private void command(object me, string arg)
{
	array info;
	string *msg, oarg;

	if( !is_command() ) return;
	
	msg  = ({ sprintf("%6s %-40s %-10s %-40s\n", "延遲", "物件", "參數", "呼叫函數") });
	msg += ({ "  ─────────────────────────────────────────\n" });
	info = call_out_info();
	info = sort_array(info, (: ($1[2] < $2[2] ? -1:1) :));
	foreach(mixed cout in info) 
	{
		if( sizeof(cout) > 3 )
			oarg = cout[3];
		else
			oarg = "無";

		msg += ({ sprintf("%6d %-40O %-10s %s\n", cout[2], cout[0], oarg, cout[1]) });
	}
	
	if( !sizeof(info) ) 
		msg += ({ "目前沒有程序呼叫中。\n" });
	
	msg += ({ "  ─────────────────────────────────────────\n" });
	msg += ({ NOR WHT" 函式呼叫─ "NOR"目前有 " + sizeof(info) + " 個程序延遲呼叫中。\n" });
	
	me->more(implode(msg, ""));
}
