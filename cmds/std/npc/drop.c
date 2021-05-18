/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : drop.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-07
 * Note   : drop ���O
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
#include <map.h>

inherit COMMAND;

string help = @HELP
�N���~��b�a�W

drop [�ƶq|all] [���~] [�Ƨ�]	- �N�Y�ƶq�αƧǪ��Y���~��b�a�W

HELP;

private void do_command(object me, string arg)
{
	string msg="";
	string option;
	int amount;
	object ob;
	array loc = query_temp("location", me);
	int is_destruct_env;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڥ�U����F��H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n��U����F��H\n");
	}

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( MAP_D->query_map_system(loc) && (MAP_D->query_map_system(loc))->query_coor_data(loc, TYPE) == RIVER ) 
		is_destruct_env = 1;

	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		if( option == "all" ) ;
		// �p�G option �ä��O�Ʀr���
		else if( !big_number_check(option) )
		{
			arg = option + " " + arg;
			amount = 1;	
		}	
		else if( to_int(option) < 1 ) 
			amount = 1;
		else
			amount = to_int(option);
	}
	else amount = 1;
	
	if( lower_case(arg) == "all" )
	{
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			foreach(ob in all_ob)
			{
				amount = ob->query_amount();
				
				if( ob->is_keeping() || query("flag/no_drop", ob) ) continue;
			
				msg += HIG"�D"NOR+QUANTITY_D->obj(ob, amount, 1)+"\n";

				if( is_destruct_env )
				{
					msg(QUANTITY_D->obj(ob, amount, 1)+"��������F�C\n", me, ob, 1);
					destruct(ob, amount);
					continue;
				}

				if( !ob->move_to_environment(me, amount) )
				{
					msg("$ME�⨭�W����᪺�F�����b�a�W�F�C\n"+msg+"\n"NOR, me, 0, 1);
					msg("$ME�Q��"+QUANTITY_D->obj(ob, amount)+"��b�a�W�A���a�W�F���b�Ӧh�S�a���F�I\n", me, 0, 1);
					me->delay_save(300);
					return;
				}
			}
			me->delay_save(300);
			return msg("$ME�⨭�W����᪺�F�����b�a�W�F�C\n"+msg+"\n"NOR, me, 0, 1);
		}
		
		if( me->is_npc() )
			return me->do_command("say �ڨ��W�S���F��i�H��F�աI\n");
		else
			return tell(me, pnoun(2, me)+"���W�S���F��i�H��F�աI\n");
	}
	
	if( objectp(ob = present(arg, me)) )
	{
		if( option == "all" )
			amount = ob->query_amount();

		if( amount > ob->query_amount() )
		{
			if( me->is_npc() )
				return me->do_command("say �ڨ��W�u�� "+ob->query_amount()+" "+(query("unit",ob)||"��")+ob->query_idname()+"�C\n");
			else
				return tell(me, pnoun(2, me)+"���W�u�� "+ob->query_amount()+" "+(query("unit",ob)||"��")+ob->query_idname()+"�C\n");
		}
		
		if( ob->is_keeping() )
		{
			if( me->is_npc() )
				return me->do_command("say �������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
			else
				return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
		}
		
		if( query("flag/no_drop", ob) )
			return tell(me, ob->query_idname()+"�����\���C\n");
		
		msg = "$ME��"+QUANTITY_D->obj(ob, amount)+"��b�a�W�C\n";

		if( is_destruct_env )
		{
			msg(msg, me, 0, 1);
			msg(QUANTITY_D->obj(ob, amount, 1)+"�Q$ME��������F�C\n", me, ob, 1);
			destruct(ob, amount);
			return;
		}
		
		if( query_temp("endurance", ob) < 0 )
		{
			msg(msg, me, 0, 1);
			msg("�l�a��"+QUANTITY_D->obj(ob, amount)+"�H�۷L���Ƭ��Фg...�C\n", me, ob, 1);
			destruct(ob, amount);
			return;
		}

		if( !ob->move_to_environment(me, amount) )
			msg = "$ME�Q��"+QUANTITY_D->obj(ob, amount)+"��b�a�W�A���a�W�F���b�Ӧh�S�a���F�I\n";
		else
			me->delay_save(300);

		msg(msg, me, 0, 1);

		return;
	}

	if( me->is_npc() )
		return me->do_command("say �ڨ��W�S�� "+arg+" �o�󪫫~�C\n");
	else
		return tell(me, pnoun(2, me)+"���W�S�� "+arg+" �o�󪫫~�C\n");
}
