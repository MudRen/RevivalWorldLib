/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : equip.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <equipment.h>

inherit COMMAND;

string help = @HELP
    卸除裝備物品的指令，無論是武器、衣物、座騎、裝飾品、團體武裝都
是利用此指令進行卸除裝備的動作。

remove '物品'		卸除裝備某項物品

HELP;

private void do_command(object me, string arg)
{
	int status;
	object ob;

	if( !arg )
	{
		string msg;
		object *equipments = me->query_equipment_objects();
		
		if( !sizeof(equipments) )
			return tell(me, pnoun(2, me)+"目前身上沒有任何裝備。\n");

		msg = pnoun(2, me)+"目前身上的裝備如下：\n";
		
		foreach(ob in me->query_equipment_objects())
			msg += "．"HIW"["NOR+ob->query_part_name()+HIW"] "+ob->query_idname()+"\n"NOR;
			
		return tell(me, msg+"\n");
	}
	
	//忙碌中不能下指令
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( COMBAT_D->in_fight(me) )
		return tell(me, "戰鬥中無法卸除裝備。\n");

	if( arg == "all" )
	{
		foreach(ob in all_inventory(me))
			if( me->unequip(ob, ref status) )
				msg("$ME卸除了裝備在「"+ob->query_part_name()+"」部位上的"+ob->query_idname()+"。\n", me, 0, 1);
				
		return;
	}

	if( !objectp(ob = present(arg, me)) )
		return tell(me, pnoun(2, me)+"的身上並沒有 "+arg+" 這個物品。\n");

	if( !me->unequip(ob, ref status) )
	{
		switch(status)
		{
			// 1: 並無裝備此物件
			// 2: 無法解除此項裝備
			
			// should not happen
			case 1: return tell(me, pnoun(2, me)+"並未裝備"+ob->query_idname()+"。\n"); break;
			case 2: return tell(me, pnoun(2, me)+"無法卸除這項裝備。\n"); break;
		}
	}
	else
		msg("$ME卸除了裝備在「"+ob->query_part_name()+"」部位上的"+ob->query_idname()+"。\n", me, ob, 1);
}
