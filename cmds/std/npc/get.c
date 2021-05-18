/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : get.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-10-10
 * Note   : get ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
�N���~�߬B�_��

get [�ƶq|all] [���~] [�Ƨ�]	- �N�Y�ƶq�αƧǪ��Y���~�߬B�_��
get all from [�Y��]		- �N�Y���~�W���Ҧ��F��߬B�_��
get autoloot			- �ҰʻP�����۰ʾ߬B���骺�\��

HELP;

private void do_command(object me, string arg)
{
	int i, mass, from_environment=1;
	object ob, target, env;
	string from, msg="";
	string option;
	int amount;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ھߤ���F��H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�ߤ���F��H\n");
	}

	if( arg == "autoloot" )
	{
		if( !query("auto/loot", me) )
		{
			set("auto/loot", 1, me);
			tell(me, "�Ұʦ۰ʾ߬B���骺�\��C\n");
		}
		else
		{
			delete("auto/loot", me);
			tell(me, "�����۰ʾ߬B���骺�\��C\n");
		}
		me->save();
		return;
	}

	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	env = environment(me);

	if( sscanf(arg, "%s from %s", arg, from) == 2 )
	{
		if( !objectp(target = present(from)) )
			return tell(me, "�o�̨S�� "+from+" �o�ӤH�Ϊ��~�C\n");
		else
			from_environment = 0;
	}
	else target = env;

	if( objectp(target) && target->is_living() )
	{
		if( target->is_module_npc() )
		{
			if( query("boss", target) != me->query_id(1) )
				return tell(me, pnoun(2, me)+"�S���v�Q�q"+target->query_idname()+"���W�߬B���󪫫~�C\n");
		}
		else if( !target->is_dead() && !wizardp(me) ) 
			return tell(me, pnoun(2, me)+"�S���v�Q�q"+target->query_idname()+"���W�߬B���󪫫~�C\n");
	}

	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		if( option == "all") ;

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

	if( objectp(ob = present(arg, target)) )
	{
		if( me == ob )
			return tell(me, pnoun(2, me)+"�p�G�u���i�H��ۤv���_�ӡA�ЦܧŮv�j�U����Q�U�������C\n");

		if( me == target )
			return tell(me, ob->query_idname()+"�w�g�b"+pnoun(2, me)+"���W�F�I�I�C\n");

		if( userp(target) )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ�"+QUANTITY_D->obj(ob, amount)+"�������I\n", me, target, 1);

		if( ob->is_living() )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ��I�O�I��$YOU�����_�ӡI�I\n", me, ob,1);

		if( option != "all" && amount > ob->query_amount() )
		{
			if( me->is_npc() )
				return me->do_command("say "+"�o�̪�"+ob->query_idname()+"�S������h�ӡC\n");
			else
				return tell(me, "�o�̪�"+ob->query_idname()+"�S������h�ӡC\n");
		}

		if( query("mass", ob) == -1 )
		{
			msg("$ME�H��N"+QUANTITY_D->obj(ob, amount)+"�@���|�_�A�u��$ME�����a���F�@�y�u�ګ��I�n���ӭ��F...�v�C\n",me,0,1);
			return me->faint();
		}

		if( query_temp("decorated", ob) )
			return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"�����C�C\n");
		
		if( query_temp("activate", ob) )
			return tell(me, pnoun(2, me)+"�L�k�߰_"+ob->query_idname()+"�C\n");

		if( option == "all" )
		{
			amount = (me->query_max_loading() - me->query_loading()) / (query("mass", ob)||1);

			if( amount > ob->query_amount() )
				amount = ob->query_amount();

			if( amount == 0 )
				return tell(me, pnoun(2, me)+"������"+ob->query_idname()+"�F�C\n");
		}

		if( objectp(target) && target->is_living() )
		{
			if( ob->is_keeping() || ob->is_equipping() )
				return tell(me, pnoun(2, me)+"�L�k�q"+target->query_idname()+"�W�߰_"+ob->query_idname()+"�C\n");
		}

		if( target->is_dead() && !target->is_module_npc() && !wizardp(me) )
		{
			CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");

			if( !sizeof(all_inventory(target)) )
				target->corpse_disappear();			
		}

		if( me->available_get(ob, amount) )
		{
			msg("$ME��"+QUANTITY_D->obj(ob, amount)+(from_environment?"":"�q"+target->query_idname()+"��")+"�ߤF�_�ӡC\n", me,0,1);
			ob->before_get(amount);
			ob->move(me, amount);
			me->delay_save(300);
			return;
		}
		else
			return msg("$ME��"+QUANTITY_D->obj(ob, amount)+(from_environment?"":"�q"+target->query_idname()+"��")+"���F�_�ӡA�uť��j�L�@�n�ذաA�j�L�����_�F�I�S������C\n",me,0,1);
	}

	if( lower_case(arg) == "all" )
	{
		object *all_ob;

		if( target->is_living() )
			all_ob = all_inventory(target);
		else
			all_ob = filter_array(present_objects(me), (: !$1->is_living() && query("mass",$1) >= 0 :));

		if( userp(target) && (!wizardp(me) || SECURE_D->level_num(me) < SECURE_D->level_num(target)) )
			return msg("$ME�b$YOU���W�F�N��N�A�յۧ�$YOU�Ҧ����F����������I\n",me,target,1);

		if( sizeof(all_ob) )
		{
			int too_heavy, loading, max_loading, special;

			max_loading = me->query_max_loading();

			foreach(ob in all_ob)
			{
				mass = query("mass",ob);
				special = query("special", ob);

				if( mass == -1 || ob->is_keeping() || ob->is_equipping() || query_temp("decorated", ob) || query_temp("activate", ob)) continue;

				amount = ob->query_amount();

				loading = max_loading - me->query_loading();

				if( objectp(target) )
					target->unequip(ob, ref i);

				if( loading < amount * mass )
				{
					if( loading < mass || !me->available_get(ob, loading/mass) ) continue;

					msg += HIR"�D"NOR+QUANTITY_D->obj(ob, loading/mass, 1)+"\n";

					if( target->is_living() && target->is_dead() && !target->is_module_npc() && !wizardp(me) )
					{
						CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");

						if( special )
						{
							if( me->is_module_npc() )
							{
								string boss = query("boss", me);
									
								if( stringp(boss) && objectp(find_player(boss)) )
									CHANNEL_D->channel_broadcast("combat", find_player(boss)->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");
							}
							else
								CHANNEL_D->channel_broadcast("combat", me->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");
						}
					}
					
					ob->before_get(loading/mass);	
					ob->move(me, loading/mass);

					too_heavy = 1;
				}
				else
				{
					if( !me->available_get(ob, amount) ) continue;

					msg += HIR"�D"NOR+trim(QUANTITY_D->obj(ob, amount))+"\n";

					if( target->is_dead() && !target->is_module_npc() && !wizardp(me) )
					{
						CHANNEL_D->channel_broadcast("nch", me->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");

						if( special )
						{
							if( me->is_module_npc() )
							{
								string boss = query("boss", me);
									
								if( stringp(boss) && objectp(find_player(boss)) )
									CHANNEL_D->channel_broadcast("combat", find_player(boss)->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");
							}
							else
								CHANNEL_D->channel_broadcast("combat", me->query_idname()+"�q"+target->query_idname()+"���W�߰_�F"+ob->query_idname()+"�C");
						}

						if( !sizeof(all_inventory(target)) )
							target->corpse_disappear();
					}
					ob->before_get(amount);
					ob->move(me , amount);
				}
			}

			if( msg == "" )
			{
				if( me->is_npc() )
					return me->do_command("say �ھߤ��_�ӥ��󪺪F��C\n");
				else
					return tell(me, pnoun(2, me)+"�ߤ��_�ӥ��󪺪F��C\n");
			}

			me->delay_save(300);
			msg("$ME��"+(from_environment?"�a�W�߱o�ʪ��F�賣�ߤF�_��":"$YOU���W�ள���F��q�q���F�X��")+"�C\n"+msg+"\n", me, target,1);

			if( too_heavy ) tell(me, "�ѤU���F��ӭ������ʤF�C\n\n");

			return;
		}

		tell(me, (from_environment?"�o�Ӧa��":target->query_idname())+"�S�����󪫫~�i�H�߭C...�C\n");

		if( objectp(target) &&  target->is_dead() && !target->is_module_npc() && !sizeof(all_inventory(target)) )
			target->corpse_disappear();

		return;
	}

	if( me->is_npc() )
		return me->do_command("say �o�̨S�� "+arg+" �o�󪫫~�C\n");
	else
		return tell(me, "�o�̨S�� "+arg+" �o�󪫫~�C\n");
}
