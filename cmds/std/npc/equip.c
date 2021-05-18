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
    �˳ƪ��~�����O�A�L�׬O�Z���B�窫�B�y�M�B�˹��~�B����Z�˳�
�O�Q�Φ����O�i��˳ưʧ@�C

equip '���~'		- �˳ƬY�����~
equip -s 2		- �N�ثe���W�˳Ƥ��Ҧ����~�]�w�� 2 ���M��
equip -d 2		- �R�� 2 ���M�˳]�w
equip -l 2		- �d�� 2 ���M�˳]�w
equip 2			- �N���W�˳ƴ��ˬ� 2 ���M��
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
		return tell(me, "�԰����L�k�˳ƪ��~�C\n");
		
	if( sscanf(arg, "-s %d", num) == 1 )
	{
		if( num < 1 || num > 5 )
			return tell(me, "�̦h�u��]�w 5 �ծM�ˡC\n");
		
		set("equipment_set/"+num, map(me->query_equipment_objects(), (: base_name($1) :)), me);
		me->save();

		return tell(me, pnoun(2, me)+"�N�ثe���˳Ƴ]�w���� "+num+" ���M�ˡC\n");
	}
	else if( sscanf(arg, "-d %d", num) == 1 )
	{
		if( !query("equipment_set/"+num, me) )
			return tell(me, pnoun(2, me)+"�쥻�èS���]�w�� "+num+" ���M�ˡC\n");
		
		delete("equipment_set/"+num, me);
		me->save();
		
		return tell(me, pnoun(2, me)+"�N�� "+num+" ���M�˳]�w�R���C\n");
	}
	else if( sscanf(arg, "-l %d", num) == 1 )
	{
		string msg;
		string *equipment_set = query("equipment_set/"+num, me);
		
		if( !arrayp(equipment_set) )
			return tell(me, pnoun(2, me)+"�쥻�èS���]�w�� "+num+" ���M�ˡC\n");
		
		msg = pnoun(2, me)+"�ҳ]�w�� "+num+" ���M�˦p�U�G\n";
		
		foreach(string basename in equipment_set)
		{
			ob = load_object(basename);
			msg += "�D"HIW"["NOR+ob->query_part_name()+HIW"] "+ob->query_idname()+"\n"NOR;
		}
		
		return tell(me, msg);
	}
	else if( sscanf(arg, "%d", num) == 1 )
	{
		int index;
		string *equipment_set = query("equipment_set/"+num, me);
		
		if( !arrayp(equipment_set) )
			return tell(me, pnoun(2, me)+"�èS���]�w�� "+num+" ���M�ˡC\n");
		
		foreach(ob in all_inventory(me))
		{
			if( me->is_equipping_object(ob) )
			{
				me->unequip(ob, ref status);
				
				//	msg("$ME�����F�˳Ʀb�u"+ob->query_part_name()+"�v����W��"+ob->query_idname()+"�C\n", me, 0, 1);
			}
		}

		foreach(ob in all_inventory(me))
		{
			index = member_array(base_name(ob), equipment_set);
			
			if( index == -1 ) continue;
	
			equipment_set = equipment_set[0..index-1] + equipment_set[index+1..];
	
			if( me->equip(ob, ref status) )
				msg("$ME�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�C\n", me, 0, 1);
			else
			{
				switch(status)
				{
					// 1: �����󤣬O�˳�
					// 2: �L�k�˳Ʀb������W
					// 3: ����������Ӹ˳Ʀb��B
					// 4: �w�g���P�������˳�
					// 5: �w�g�˳Ʀb��L������W
					
					case 1:	tell(me, ob->query_idname()+"�L�k�ΨӸ˳ơC\n"); break;
					case 2: tell(me, pnoun(2, me)+"���G�S����������O�Ӹ˳Ƴo�Ӫ��~�C\n"); break;
					case 3: tell(me, pnoun(2, me)+"���A��"+ob->query_idname()+"�Ӹ˳Ʀb��B�C\n"); break;
					case 4: tell(me, ob->query_idname()+"�����˳Ʀb�u"+ob->query_part_name()+"�v����W�A���P"+pnoun(2, me)+"���W�Y�Ӹ˳Ƥ������~�o�ͽĬ�C\n"); break;
					case 5: tell(me, ob->query_idname()+"�w�g�˳Ʀb��L������W�F�C\n"); break;
					default: error("�˳ƪ��~�o�Ϳ��~�C\n"); break;
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
				tell(me, pnoun(2, me)+"�w�g�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�F�C\n");

			if( me->equip(ob, ref status) )
				msg("$ME�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�C\n", me, 0, 1);
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
			return tell(me, "�o����èS�� "+arg+" �o�Ӫ��~�C\n");
	}

	if( me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"�w�g�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�F�C\n");

	if( !me->equip(ob, ref status) )
	{
		switch(status)
		{
			// 1: �����󤣬O�˳�
			// 2: �L�k�˳Ʀb������W
			// 3: ����������Ӹ˳Ʀb��B
			// 4: �w�g���P�������˳�
			// 5: �w�g�˳Ʀb��L������W
			
			case 1:	return tell(me, ob->query_idname()+"�L�k�ΨӸ˳ơC\n"); break;
			case 2: return tell(me, pnoun(2, me)+"���G�S����������O�Ӹ˳Ƴo�Ӫ��~�C\n"); break;
			case 3: return tell(me, pnoun(2, me)+"���A��"+ob->query_idname()+"�Ӹ˳Ʀb��B�C\n"); break;
			case 4: return tell(me, ob->query_idname()+"�����˳Ʀb�u"+ob->query_part_name()+"�v����W�A���P"+pnoun(2, me)+"���W�Y�Ӹ˳Ƥ������~�o�ͽĬ�C\n"); break;
			case 5: return tell(me, ob->query_idname()+"�w�g�˳Ʀb��L������W�F�C\n"); break;
			default: error("�˳ƪ��~�o�Ϳ��~�C\n"); break;
		}
	}
	else
	{
		msg("$ME�N"+ob->query_idname()+"�˳Ʀb�u"+ob->query_part_name()+"�v������W�C\n", me, 0, 1);
		
		if( equip_from_environment )
			ob->move(me, 1);
	}
}
