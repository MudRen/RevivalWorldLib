/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _import_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>

int over_capacity(object env, string database, int amount, int capacity);
void input_object(object env, string database, string basename, int amount);

// ���~��J���O
void do_import(object me, string arg, string database, int capacity)
{
	int amount;
	string option;
	string basename, env_city;
	object ob, env, master, *obs;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��J���򪫫~�H\n");

	env = environment(me);
	master = env->query_master();

	if( !userp(me) ) return;

	if( !wizardp(me) && query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�L�k�b�O�H���ؿv����J���~�C\n");

	env_city = env->query_city();
	
	if( arg == "all" )
		obs = filter_array(all_inventory(me)+all_inventory(env), (: !$1->is_keeping() && !query("flag/no_import", $1) && count(query("value", $1),">",0) :));		
	else
	{
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
	
		if( !objectp(ob = present(arg, me) || present(arg, env)) )
			return tell(me, "�o����èS�� "+arg+" �o�˪F��C\n");
	
		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
		
		if( query("flag/no_import", ob) )
			return tell(me, ob->query_idname()+"�����\��J�C\n");

		if( query_temp("decorated", ob) )
			return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"�����C�C\n");

		if( option == "all" )
			amount = ob->query_amount();

		if( amount > ob->query_amount() )
			return tell(me, "�o����èS������h"+(query("unit", ob)||"��")+ob->query_idname()+"�C\n");
	
		if( ob->is_living() )
			return tell(me, pnoun(2, me)+"�L�k��J�@�ӥͪ��I\n");
			
		obs = ({ ob });
	}
	
	if( !sizeof(obs) )
		return tell(me, "�S������i�H��J�����~�C\n");
	
	foreach( ob in obs )
	{
		basename = base_name(ob);
		
		if( arg == "all" )
			amount = ob->query_amount();

		if( count(query("value", ob), "<", 1) )
		{
			tell(me, ob->query_idname()+"�S�����ȡA�L�k��J�C\n");
			continue;
		}

		if( query_temp("decorated", ob) )
		{
			tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"�����C�C\n");
			continue;
		}

		if( !undefinedp(query_temp("endurance", ob)) && query_temp("endurance", ob) <= 0 )
		{
			tell(me, ob->query_idname()+"�w�g�l�a�F�A�L�k��J�C\n");
			continue;
		}

		// �W�L�e�q
		if( over_capacity(master, database, amount, capacity) )
		{
			master->delay_save(300);
			return tell(me, env->query_room_name()+"�L�k�A�e�Ǩ���h���~�F�C\n");
		}
		
		input_object(master, database, basename, amount);
		
		msg("$ME�N"+QUANTITY_D->obj(ob, amount)+"��z���J"+env->query_room_name()+"�C\n"NOR, me, 0, 1);

		destruct(ob, amount);
	}
	
	master->delay_save(300);
}
