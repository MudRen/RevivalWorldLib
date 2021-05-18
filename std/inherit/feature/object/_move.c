/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _move.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : ���󲾰��~����
 * Update :
 *  o 2002-08-12 Clode �����䴩�j�ƶq���ʻP��X�t�����D
 *
 -----------------------------------------
 */

#include <type.h>
#include <location.h>
#include <daemon.h>
#include <object.h>

nomask void move_to_object(object where);

varargs int move(mixed where, int amount)
{
	object me = this_object();
	int originalamount;
	array originalloc;
	object env, new_env;
	string basename;
	object *nearby_objects;

	originalloc = copy(query_temp("location"));
	originalamount = me->query_amount();
	new_env = env = environment();
	basename = base_name(me);

	//if( !clonep(me) )
	//{
		//CHANNEL_D->channel_broadcast("nch", sprintf("main ob %O move call: %O \n",me, call_stack(1)));	
	//}

	if( !where ) return 0;

	if( !undefinedp(amount) && amount <= 0 )
		return 0;

	switch(typeof(where))
	{
		// ���ʦܦa�Ϯy�ФW
		case ARRAY:	
			{
				string code;
				
				code = where[CODE] = where[X]+"/"+where[Y]+"/"+where[Z]+"/"+where[PLACE]+"/"+where[NUM]+"/"+where[EXTRA];
				
				// �ˬd�O�_�n���ܷs���a�Ϫ���
				if( !arrayp(originalloc) || originalloc[NUM] != where[NUM] || originalloc[PLACE] != where[PLACE] )
					new_env = MAP_D->query_maproom(where);

				// �D�զX���󲾰�
				if( me->no_amount() )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);

					//�Y��l���Ҭ��ͪ�
					if( objectp(env) && env->is_living() )
					{				
						// �h���ʧ@
						env->remove_action(me);
					}

					while(--amount > 0)
						new( basename )->move(where);
					
					if( env != new_env )
					{
						if( objectp(env) && !env->is_maproom() )
							env->leave(me);

						move_object(new_env);
					}
					
					MAP_D->enter_coordinate(where, me);
					
					// init()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->init(me);
					
					set_temp("location", copy(where));
				}
				// �զX������Ʋ���
				else if( undefinedp(amount) || amount == originalamount )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);
					
					// �ˬd�y�гB�O�_�w���ۦP����
					foreach( object ob in MAP_D->coordinate_inventory(where) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(originalamount);
							
						MAP_D->leave_coordinate(originalloc, me);

						destruct(me);
						return 1;
					}

					//�Y��l���Ҭ��ͪ�
					if( objectp(env) && env->is_living() )
					{	
						// �h���ʧ@
						env->remove_action(me);
					}

					if( env != new_env )
					{
						if( objectp(env) && !env->is_maproom() )
							env->leave(me);

						move_object(new_env);
					}
					
					MAP_D->enter_coordinate(where, me);

					// init()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->init(me);
					set_temp("location", copy(where));
				}
				// �Y�ǤJ�����ʼƶq�ѼƤp���`��
				else if( amount < originalamount )
				{
					object newob;

					// ��h���ʼƶq
					me->add_amount(-amount);
					
					// �ˬd�y�гB�O�_�w���ۦP����
					foreach( object ob in MAP_D->coordinate_inventory(where) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(amount);
						return 1;
					}
					
					// ���ͷs���󲾩��ؼЮy��
					newob = new(basename);

					newob->set_amount(amount);
					
					MAP_D->enter_coordinate(where, newob);
					set_temp("location", copy(where), newob);
					
					newob->move_to_object(new_env);
				}
				else
					return 0;

				break;
			}
		case STRING:
				where = load_object(where);
		case OBJECT:
			{
				new_env = where;

				// �D�զX���󲾰�
				if( me->no_amount() )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);

					//�Y����l����
					if( objectp(env) )
					{
						if( env->is_living() )
						{
							// �h���ʧ@
							env->remove_action(me);
						}
						else if( env->is_maproom() )
							delete_temp("location");
					}

					while(--amount > 0)
						new( basename )->move(where);
					
					if( env != new_env )
					{
						if( objectp(env) && !env->is_maproom() )
							env->leave(me);

						move_object(new_env);
						
						if( objectp(env) && new_env->is_npc() )
							call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), me, 1);

						new_env->init(me);
					}
					
					if( new_env->is_living() )	
						new_env->add_action(me);
					
					// init()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->init(me);
				}
				// �զX������Ʋ���
				else if( undefinedp(amount) || amount == originalamount )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);

					foreach( object ob in all_inventory(new_env) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(originalamount);
								
						MAP_D->leave_coordinate(originalloc, me);
						
						if( objectp(env) && new_env->is_npc() )
							call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), ob, originalamount);
						
						// �P������� destruct() �ۤv�|���ͪ��t���վ�, �]����l���ҭt���B�z��b�᭱
						destruct(me);

						return 1;
					}
	
					//�Y����l����
					if( objectp(env) )
					{
						if( env->is_living() )
						{
							// �h���ʧ@
							env->remove_action(me);
						}
						else if( env->is_maproom() )
							delete_temp("location");
					}
		
					if( new_env->is_living() )
						new_env->add_action(me);
					
					if( env != new_env )
					{
						if( objectp(env) )
							env->leave(me);

						move_object(new_env);
						
						if( objectp(env) && new_env->is_npc() )
							call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), me, originalamount);
						
						new_env->init(me);
					}
					
					// init()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->init(me);
				}
				else if( amount < originalamount )
				{
					object newob;
			
					// ��h���ʼƶq
					me->add_amount(-amount);

					// �ˬd���󤺳��O�_�w���ۦP����
					foreach( object ob in all_inventory(new_env) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(amount);
						
						if( objectp(env) )
							call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), ob, amount);

						return 1;
					}

					// ���ͷs���󲾩��ؼЪ���
					newob = new(basename);
						
					newob->set_amount(amount);
						
					if( new_env->is_living() )
						new_env->add_action(newob);

					newob->move_to_object(new_env);
					
					if( objectp(env) && new_env->is_npc() )
						call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), newob, amount);
				}
				else
					return 0;

				break;
			}
		default:
				error(sprintf("move() �L�k���θ��J���� %O �C\n", where));
	}

	all_inventory()->do_look("outside");
}


varargs int move_to_environment(object ob, int amount)
{
	object env;
	
	if( !objectp(ob) ) return 0;

	if( !(env = environment(ob)) ) env = ob;
	
	if( env->is_maproom() )
	{
		if( undefinedp(amount) )
			move(copy(query_temp("location", ob)));
		else
			move(copy(query_temp("location", ob)), amount);
	}
	else
	{
		if( undefinedp(amount) )
			move(env) ;
		else
			move(env, amount);
	}

	return 1;
}	
	
// ��²��ʪ���ܥt�@����W
nomask void move_to_object(object where)
{
	if( where == this_object() || where == environment() ) return;
		
	move_object(where);
}


//  �o�� apply ���G�w�g����?
private int move_or_destruct( object dest )
{

}
