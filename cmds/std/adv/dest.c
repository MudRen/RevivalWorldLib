/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dest.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-5
 * Note   : dest 指令
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
	標準 dest 指令。
HELP;

private void command(object me, string arg)
{
	int dest_all, amount, total_amount;
	string str, ob_idname, location, option;
	object ob, env;
	
	if( !is_command() ) return;

	if( !arg ) return tell(me, "指令格式：dest <物件名||檔案名>。\n");
	
	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		// 如果 option 並不是數字單位
		if( to_float(option) == 0. )
		{	
			if( lower_case(option) == "all" )
				dest_all = 1;
			else
			{
				arg = option + " " + arg;
				amount = 1;
			}	
		}	
		else if( to_int(option) < 1 )
			amount = 1;
		else
			amount = to_int(option);
	}
	else amount = 1;
	
	ob = present(arg, me) || present(arg, environment(me));

	if( !objectp(ob) )
	{
		str = resolve_path(me, arg);
		ob = find_object(str) || find_object(str+".c") || find_object(str+".o");
	}

	if( !objectp(ob) )
		return tell(me, "沒有 "+arg+" 這個物件或檔案。\n");
	
	ob_idname = ob->query_idname()||file_name(ob);
	
	if( userp(ob) )
		return tell(me, pnoun(2, me)+"無法摧毀一個使用者物件。\n");
	
	total_amount = ob->query_amount();
	
	env = environment(ob);
	
	if( env == me )
		location = "身上";
	else if( env == environment(me) )
		location = "地上";
	else
		location = "記憶體中";
	
	if( dest_all || total_amount <= amount )
	{	
		if( destruct(ob) )
			return msg("$ME使出"HIW"百"NOR WHT"裂"HIW"掌"NOR"將"+location+"全部的「"+ob_idname+"」撕成了碎片。\n", me, 0, 1);
			
		return tell(me, "摧毀物件 "+arg+" 失敗。\n");
	}
	else
	{
		msg("$ME使出"HIW"百"NOR WHT"裂"HIW"掌"NOR"將"+location+"的 "+NUMBER_D->number_symbol(amount)+" "+(query("unit",ob)||"個")+"「"+ob_idname+"」撕成了碎片。\n", me, 0, 1);

		ob->set_amount(total_amount - amount);
		return;
	}
}