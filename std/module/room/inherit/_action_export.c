/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _export_action.c
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

void output_object(object env, string database, string basename, int amount);

// ���~��X���O
void do_export(object me, string arg, string database)
{
	int i, j, which = 1;
	int allow_mayor_levy = 0, amount = 1, pamount;
	object env, master, ob, newob;
	mapping objects;
	string env_city, objectname, basename, option;
	string *shelfsort, shelf;
	
	if( !userp(me) ) return;
			
	env = environment(me);
	master = env->query_master();
	env_city = env->query_city();

	if( query("owner", env) != me->query_id(1) && !wizardp(me) )
	{
		if( CITY_D->is_mayor(env_city, me) )
		{
			object user = load_user(query("owner", env));
			
			// ��Ӥ를�W�u
			if( time() - query("last_on/time", user) > 60*60*24*60 )
			{
				if( !userp(user) )
					destruct(user);
				allow_mayor_levy = 1;
			}
			else
			{
				if( !userp(user) )
					destruct(user);					
				return tell(me, pnoun(2, me)+"�L�k�b�O�H���ؿv����X���~�C\n");
			}
		}
		else
			return tell(me, pnoun(2, me)+"�L�k�b�O�H���ؿv����X���~�C\n");
	}

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��X���򪫫~�H\n");
	
	sscanf(arg, "%s %s", option, objectname);
	
	if( !objectname || (option != "all" && !(amount = to_int(option))) )
	{
		objectname = arg;
		amount = 1;
	}
	
	if( sscanf( objectname, "%s %d", arg, which ) == 2 )
	{
		if( which < 1 ) which = 1;
		objectname = arg;
	}

	objects = query(database, master);
	
	if( !sizeof(objects) )
		return tell(me, env->query_room_name()+"�̨èS�����󪫫~�C\n");

	shelfsort = keys(objects) | (query("shelfsort", master) || allocate(0));

	foreach( shelf in shelfsort )
	{
		for(i=0;i<sizeof(objects[shelf]);i+=2)
		{
			basename = objects[shelf][i];
			pamount = to_int(objects[shelf][i+1]);
			
			if( catch(ob = load_object(basename)) )
				continue;

			// �ɮפw�g����, �R�����
			if( !objectp(ob) && !file_exists(basename) )
			{
				output_object(master, database, basename, -3);
				continue;
			}

			j++;
	
			if( j == to_int(objectname) || objectname == "all" || (ob->query_id(1) == lower_case(objectname) && !(--which)) )
			{
				if( option == "all" || objectname == "all" )
					amount = pamount;
				else if( amount > pamount )
					return tell(me, "�o�̨èS���o��h"+(query("unit", ob)||"��")+ob->query_idname()+"�C\n");
				else if( amount < 1 )
					return tell(me, "�п�J���T����X�ƶq�C\n");
				
				if( allow_mayor_levy && !query("material", ob) )
					return tell(me, pnoun(2, me)+"�L�k��X"+ob->query_idname()+"�C\n");

				newob = new(basename);
				
				if( !newob->no_amount() )
					newob->set_amount(amount);
				else if( amount > 500 )
					amount = 500;

				msg("$ME�N"+QUANTITY_D->obj(ob, amount)+"��"+env->query_room_name()+"��X�C\n",me,0,1);

				if( !me->available_get(newob, amount) )
				{
					if( amount == 1 )
					{
						tell(me, pnoun(2, me)+"���W�����~�Ӧh�Τӭ��L�k�A����h�F��F�A�u�n����b�a�W�C\n");
						newob->move_to_environment(me);
					}
					else
					{
						tell(me, pnoun(2, me)+"���W�����~�Ӧh�Τӭ��L�k�A����h�F��F�C\n");
						destruct(newob);
						master->delay_save(300);
						return;
					}
				}
				else
					newob->move(me, amount);
	
				output_object(master, database, basename, amount);
				
				if( objectname != "all" )
				{
					master->delay_save(300);
					return;
				}
			}
		}
	}
	
	if( objectname != "all" )
		return tell(me, "�o�̨èS�� "+objectname+" �o�ذӫ~�C\n");
	
	master->delay_save(300);
}
