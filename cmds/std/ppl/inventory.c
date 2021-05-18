/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : inventory.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-11-6
 * Note   : inventory ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <buff.h>

inherit COMMAND;

string help = @HELP
    �o�ӫ��O�i�C�X���W�Ҧ���a�������~�A�ѩ�o�O�D�`�`�Ϊ����O�A�ҥH�b�ܦh
Mud �̳��|������²�g�C�o�̤]���ҥ~�A�u�n�� i �N����o���O�ۦP�����G�X�{�C

HELP;

#define MAX_SHOW_SIZE	150

string loading_buff_description(object ob)
{
	int buff = ob->query_all_buff(BUFF_LOADING_MAX);
	
	if( buff > 0 )
		return sprintf(HIY"%.1f"NOR"("CYN"+"HIC"%d"NOR")", ob->query_max_loading()/1000., buff);
	else if( buff < 0 )
		return sprintf(HIY"%.1f"NOR"("RED"-"HIR"%d"NOR")", ob->query_max_loading()/1000., -buff);
	else
		return sprintf(HIY"%.1f"NOR, ob->query_max_loading()/1000.);
}

string slot_buff_description(object ob)
{
	int buff = ob->query_all_buff(BUFF_SLOT_MAX);
	
	if( buff > 0 )
		return sprintf(HIY"%d"NOR"("CYN"+"HIC"%d"NOR")", ob->query_inventory_slot(), buff);
	else if( buff < 0 )
		return sprintf(HIY"%d"NOR"("RED"-"HIR"%d"NOR")", ob->query_inventory_slot(), -buff);
	else
		return sprintf(HIY"%d"NOR, ob->query_inventory_slot());
}

private void show_inventory(object ob, object me, int show_filename)
{
	string *msg = allocate(0);

	object *obs = all_inventory(ob);
	
	if( !sizeof(obs) )
		msg = ({ sprintf("%s���W�èS����a���󪫫~("NOR GRN"%d"NOR"/"HIG"%d"NOR" �Ŷ��B"NOR GRN"%.1f"NOR"/"HIG"%.1f"NOR" �t��)�C", (ob==me?pnoun(2, ob):ob->query_idname()), sizeof(obs), ob->query_inventory_slot(), ob->query_loading()/1000.,  ob->query_max_loading()/1000.) });
	
	else
	{

		if( sizeof(obs) > MAX_SHOW_SIZE )
			msg += ({ HIG"\n    ���W�@�� "+sizeof(obs)+" �󪫫~�A�ƶq�Ӧh�@�ɬݤ��M��..."NOR });
		else
		foreach( object obj in sort_objects(obs)[0..MAX_SHOW_SIZE-1] )
			msg += ({ sprintf(" %s%s ",  obj->is_keeping() ? HIR"#"NOR : " ", obj->is_equipping() ? HIG"#"NOR : " " ) + (obj->is_equipping() ? (HIW"["NOR+obj->query_part_name()+HIW"] "NOR):"")+trim(obj->short(1))+ (show_filename ? " -> "NOR WHT+file_name(obj)+NOR:"") });	

		msg = ({ sprintf("%s���W��a�ۥH�U���~�G("NOR YEL"%d"NOR"/"HIY"%s"NOR" �Ŷ��B"NOR YEL"%.1f"NOR"/%s �t��)",
			(ob==me?pnoun(2, ob):ob->query_idname()),
			sizeof(obs), 
			slot_buff_description(ob),
			ob->query_loading()/1000.,
			loading_buff_description(ob)
		) }) + msg
		;
	}
	
	me->more(implode(msg, "\n"));
}

void do_command(object me, string arg)
{
	int show_filename;
	object target;
	
	if( !arg )
		return show_inventory(me, me, wizardp(me));

	if( wizardp(me) )
	{
		target = find_player(arg) || present(arg);
		show_filename = 1;
	}
	else
		target = present(arg);
	
	if( objectp(target) )
	{
		if( target->is_user_ob() || target->is_npc() )
		{
			if( query("boss", target) == me->query_id(1) || wizardp(me) )
				show_inventory(target, me, show_filename);
			else return tell(me, pnoun(2, me)+"�ä��O"+target->query_idname()+"�����D�C\n");
		}
		else return tell(me, pnoun(2, me)+"�S��k�o�˰��C\n");
	}
	else return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");
}