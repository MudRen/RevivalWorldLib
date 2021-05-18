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
    �����˳ƪ��~�����O�A�L�׬O�Z���B�窫�B�y�M�B�˹��~�B����Z�˳�
�O�Q�Φ����O�i������˳ƪ��ʧ@�C

remove '���~'		�����˳ƬY�����~

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
			return tell(me, pnoun(2, me)+"�ثe���W�S������˳ơC\n");

		msg = pnoun(2, me)+"�ثe���W���˳Ʀp�U�G\n";
		
		foreach(ob in me->query_equipment_objects())
			msg += "�D"HIW"["NOR+ob->query_part_name()+HIW"] "+ob->query_idname()+"\n"NOR;
			
		return tell(me, msg+"\n");
	}
	
	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( COMBAT_D->in_fight(me) )
		return tell(me, "�԰����L�k�����˳ơC\n");

	if( arg == "all" )
	{
		foreach(ob in all_inventory(me))
			if( me->unequip(ob, ref status) )
				msg("$ME�����F�˳Ʀb�u"+ob->query_part_name()+"�v����W��"+ob->query_idname()+"�C\n", me, 0, 1);
				
		return;
	}

	if( !objectp(ob = present(arg, me)) )
		return tell(me, pnoun(2, me)+"�����W�èS�� "+arg+" �o�Ӫ��~�C\n");

	if( !me->unequip(ob, ref status) )
	{
		switch(status)
		{
			// 1: �õL�˳Ʀ�����
			// 2: �L�k�Ѱ������˳�
			
			// should not happen
			case 1: return tell(me, pnoun(2, me)+"�å��˳�"+ob->query_idname()+"�C\n"); break;
			case 2: return tell(me, pnoun(2, me)+"�L�k�����o���˳ơC\n"); break;
		}
	}
	else
		msg("$ME�����F�˳Ʀb�u"+ob->query_part_name()+"�v����W��"+ob->query_idname()+"�C\n", me, ob, 1);
}
