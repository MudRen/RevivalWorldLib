/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : keep.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-12
 * Note   : keep 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <object.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
這個指令用來保留身上重要的物品，以防不小心被賣出或者是丟棄。

keep '物品'		- 啟動身上某個物品的保留狀態
keep -d '物品'    	- 取消某個物品的保留狀態
keep all            	- 啟動身上所有東西的保留狀態
keep -d all            	- 取消身上所有東西的保留狀態
HELP;

private void do_command(object me, string arg)
{
	int cancel;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say 老闆想要將我身上哪一個物品設定為保留狀態？");
		else
			return tell(me, pnoun(2, me)+"想要將哪一個物品設定為保留狀態？\n");
	}
	
	if( sscanf(arg, "-d %s", arg) )
		cancel = 1;
	
	if( arg == "all" )
	{
		if( !cancel )
		{
			foreach( ob in all_inventory(me) )
				ob->set_keep();

			if( me->is_npc() )
				return me->do_command("say 啟動所有身上物品的保留狀態。");
			else
				return tell(me, "啟動所有身上物品的保留狀態。\n");
		}
		else
		{
			foreach( ob in all_inventory(me) )
			{
				if( ob->is_keeping() == 2 || ob->is_equipping() ) 
					continue;
					
				ob->delete_keep();
			}
		
			if( me->is_npc() )
				return me->do_command("say 取消所有身上物品的保留狀態。");
			else
				return tell(me, "取消所有身上物品的保留狀態。\n");
		}
	}
	else if( objectp(ob = present(arg, me)) )
	{
		if( !cancel )
		{
			ob->set_keep();
			
			if( me->is_npc() )
				return me->do_command("say 啟動"+ob->query_idname()+"之保留狀態。");
			else
				return tell(me, "啟動"+ob->query_idname()+"之保留狀態。\n");
		}
		else
		{
			if( ob->is_keeping() == 2 )
			{
				if( me->is_npc() )
					return me->do_command("say 無法取消"+ob->query_idname()+"之保留狀態。");
				else
					return tell(me, "無法取消"+ob->query_idname()+"之保留狀態。\n");
			}
			
			if( ob->is_equipping() )
			{
				if( me->is_npc() )
					return me->do_command("say 必須先卸下"+ob->query_idname()+"的裝備狀態。");
				else
					return tell(me, pnoun(2, me)+"必須先卸下"+ob->query_idname()+"的裝備狀態。\n");
			}

			ob->delete_keep();
			
			if( me->is_npc() )
				return me->do_command("say 取消"+ob->query_idname()+"之保留狀態。");
			else	
				return tell(me, "取消"+ob->query_idname()+"之保留狀態。\n");
		}
	}
	else
	{
		if( me->is_npc() )
			return me->do_command("say 我身上並沒有 "+arg+" 這件物品。");
		else	
			return tell(me, pnoun(2, me)+"身上並沒有 "+arg+" 這件物品。\n");
	}
}
