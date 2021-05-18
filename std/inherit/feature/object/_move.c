/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _move.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-6
 * Note   : 物件移動繼承檔
 * Update :
 *  o 2002-08-12 Clode 直接支援大數量移動與整合負重問題
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
		// 移動至地圖座標上
		case ARRAY:	
			{
				string code;
				
				code = where[CODE] = where[X]+"/"+where[Y]+"/"+where[Z]+"/"+where[PLACE]+"/"+where[NUM]+"/"+where[EXTRA];
				
				// 檢查是否要移至新的地圖物件
				if( !arrayp(originalloc) || originalloc[NUM] != where[NUM] || originalloc[PLACE] != where[PLACE] )
					new_env = MAP_D->query_maproom(where);

				// 非組合物件移動
				if( me->no_amount() )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);

					//若原始環境為生物
					if( objectp(env) && env->is_living() )
					{				
						// 去除動作
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
				// 組合物件全數移動
				else if( undefinedp(amount) || amount == originalamount )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);
					
					// 檢查座標處是否已有相同物件
					foreach( object ob in MAP_D->coordinate_inventory(where) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(originalamount);
							
						MAP_D->leave_coordinate(originalloc, me);

						destruct(me);
						return 1;
					}

					//若原始環境為生物
					if( objectp(env) && env->is_living() )
					{	
						// 去除動作
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
				// 若傳入之移動數量參數小於總數
				else if( amount < originalamount )
				{
					object newob;

					// 減去移動數量
					me->add_amount(-amount);
					
					// 檢查座標處是否已有相同物件
					foreach( object ob in MAP_D->coordinate_inventory(where) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(amount);
						return 1;
					}
					
					// 產生新物件移往目標座標
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

				// 非組合物件移動
				if( me->no_amount() )
				{
					// leave()
					nearby_objects = present_objects(me) - ({ me });
					nearby_objects->leave(me);

					MAP_D->leave_coordinate(originalloc, me);

					//若有原始環境
					if( objectp(env) )
					{
						if( env->is_living() )
						{
							// 去除動作
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
				// 組合物件全數移動
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
						
						// 銷毀物件時 destruct() 自己會做生物負重調整, 因此原始環境負重處理放在後面
						destruct(me);

						return 1;
					}
	
					//若有原始環境
					if( objectp(env) )
					{
						if( env->is_living() )
						{
							// 去除動作
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
			
					// 減去移動數量
					me->add_amount(-amount);

					// 檢查物件內部是否已有相同物件
					foreach( object ob in all_inventory(new_env) )
					if( base_name(ob) == basename )
					{
						ob->add_amount(amount);
						
						if( objectp(env) )
							call_out((: call_other :), 0, new_env, "reply_get_object", this_player(), ob, amount);

						return 1;
					}

					// 產生新物件移往目標物件
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
				error(sprintf("move() 無法找到或載入物件 %O 。\n", where));
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
	
// 單純移動物件至另一物件上
nomask void move_to_object(object where)
{
	if( where == this_object() || where == environment() ) return;
		
	move_object(where);
}


//  這個 apply 似乎已經失效?
private int move_or_destruct( object dest )
{

}
