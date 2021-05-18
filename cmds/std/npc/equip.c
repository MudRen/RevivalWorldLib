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
    裝備物品的指令，無論是武器、衣物、座騎、裝飾品、團體武裝都
是利用此指令進行裝備動作。

equip '物品'		- 裝備某項物品
equip -s 2		- 將目前身上裝備之所有物品設定為 2 號套裝
equip -d 2		- 刪除 2 號套裝設定
equip -l 2		- 查詢 2 號套裝設定
equip 2			- 將身上裝備換裝為 2 號套裝
HELP;

private void do_command(object me, string arg)
{
	int status;
	object ob;
	int equip_from_environment;
	int num;
	
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
		return tell(me, "戰鬥中無法裝備物品。\n");
		
	if( sscanf(arg, "-s %d", num) == 1 )
	{
		if( num < 1 || num > 5 )
			return tell(me, "最多只能設定 5 組套裝。\n");
		
		set("equipment_set/"+num, map(me->query_equipment_objects(), (: base_name($1) :)), me);
		me->save();

		return tell(me, pnoun(2, me)+"將目前的裝備設定為第 "+num+" 號套裝。\n");
	}
	else if( sscanf(arg, "-d %d", num) == 1 )
	{
		if( !query("equipment_set/"+num, me) )
			return tell(me, pnoun(2, me)+"原本並沒有設定第 "+num+" 號套裝。\n");
		
		delete("equipment_set/"+num, me);
		me->save();
		
		return tell(me, pnoun(2, me)+"將第 "+num+" 號套裝設定刪除。\n");
	}
	else if( sscanf(arg, "-l %d", num) == 1 )
	{
		string msg;
		string *equipment_set = query("equipment_set/"+num, me);
		
		if( !arrayp(equipment_set) )
			return tell(me, pnoun(2, me)+"原本並沒有設定第 "+num+" 號套裝。\n");
		
		msg = pnoun(2, me)+"所設定的 "+num+" 號套裝如下：\n";
		
		foreach(string basename in equipment_set)
		{
			ob = load_object(basename);
			msg += "．"HIW"["NOR+ob->query_part_name()+HIW"] "+ob->query_idname()+"\n"NOR;
		}
		
		return tell(me, msg);
	}
	else if( sscanf(arg, "%d", num) == 1 )
	{
		int index;
		string *equipment_set = query("equipment_set/"+num, me);
		
		if( !arrayp(equipment_set) )
			return tell(me, pnoun(2, me)+"並沒有設定第 "+num+" 號套裝。\n");
		
		foreach(ob in all_inventory(me))
		{
			if( me->is_equipping_object(ob) )
			{
				me->unequip(ob, ref status);
				
				//	msg("$ME卸除了裝備在「"+ob->query_part_name()+"」部位上的"+ob->query_idname()+"。\n", me, 0, 1);
			}
		}

		foreach(ob in all_inventory(me))
		{
			index = member_array(base_name(ob), equipment_set);
			
			if( index == -1 ) continue;
	
			equipment_set = equipment_set[0..index-1] + equipment_set[index+1..];
	
			if( me->equip(ob, ref status) )
				msg("$ME將"+ob->query_idname()+"裝備在「"+ob->query_part_name()+"」的部位上。\n", me, 0, 1);
			else
			{
				switch(status)
				{
					// 1: 此物件不是裝備
					// 2: 無法裝備在此物件上
					// 3: 不知此物件該裝備在何處
					// 4: 已經有同種類的裝備
					// 5: 已經裝備在其他的部位上
					
					case 1:	tell(me, ob->query_idname()+"無法用來裝備。\n"); break;
					case 2: tell(me, pnoun(2, me)+"似乎沒有足夠的能力來裝備這個物品。\n"); break;
					case 3: tell(me, pnoun(2, me)+"不瞭解"+ob->query_idname()+"該裝備在何處。\n"); break;
					case 4: tell(me, ob->query_idname()+"必須裝備在「"+ob->query_part_name()+"」部位上，但與"+pnoun(2, me)+"身上某個裝備中的物品發生衝突。\n"); break;
					case 5: tell(me, ob->query_idname()+"已經裝備在其他的部位上了。\n"); break;
					default: error("裝備物品發生錯誤。\n"); break;
				}
			}
		}
		
		return;
	}
	
	if( arg == "all" )
	{
		foreach(ob in all_inventory(me))
		{
			if( me->is_equipping_object(ob) )
				tell(me, pnoun(2, me)+"已經將"+ob->query_idname()+"裝備在「"+ob->query_part_name()+"」的部位上了。\n");

			if( me->equip(ob, ref status) )
				msg("$ME將"+ob->query_idname()+"裝備在「"+ob->query_part_name()+"」的部位上。\n", me, 0, 1);
		}
				
		return;
	}

	if( !objectp(ob = present(arg, me)) )
	{
		if( objectp(ob = present(arg, environment(me))) && !ob->is_living() )
		{
			equip_from_environment = 1;
		}
		else
			return tell(me, "這附近並沒有 "+arg+" 這個物品。\n");
	}

	if( me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"已經將"+ob->query_idname()+"裝備在「"+ob->query_part_name()+"」的部位上了。\n");

	if( !me->equip(ob, ref status) )
	{
		switch(status)
		{
			// 1: 此物件不是裝備
			// 2: 無法裝備在此物件上
			// 3: 不知此物件該裝備在何處
			// 4: 已經有同種類的裝備
			// 5: 已經裝備在其他的部位上
			
			case 1:	return tell(me, ob->query_idname()+"無法用來裝備。\n"); break;
			case 2: return tell(me, pnoun(2, me)+"似乎沒有足夠的能力來裝備這個物品。\n"); break;
			case 3: return tell(me, pnoun(2, me)+"不瞭解"+ob->query_idname()+"該裝備在何處。\n"); break;
			case 4: return tell(me, ob->query_idname()+"必須裝備在「"+ob->query_part_name()+"」部位上，但與"+pnoun(2, me)+"身上某個裝備中的物品發生衝突。\n"); break;
			case 5: return tell(me, ob->query_idname()+"已經裝備在其他的部位上了。\n"); break;
			default: error("裝備物品發生錯誤。\n"); break;
		}
	}
	else
	{
		msg("$ME將"+ob->query_idname()+"裝備在「"+ob->query_part_name()+"」的部位上。\n", me, 0, 1);
		
		if( equip_from_environment )
			ob->move(me, 1);
	}
}
