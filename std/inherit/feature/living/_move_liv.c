/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _move_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-1
 * Note   : 生物移動繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <type.h>
#include <daemon.h>
#include <object.h>
#include <location.h>
#include <message.h>
#include <envvar.h>
#include <login.h>
#include <buff.h>
#include <fly.h>

private nomask void process_command(string verb, string args, string input);

varargs nomask void do_look(string args)
{
	process_command("look", args, "look "+args);
}

int valid_move()
{
	object me = this_object();

	// 犯人
	if( query("prisoner") )
	{
		msg("$ME的手腳被鐵鍊銬住，無法自由行動。\n", me, 0, 1);
		return 0;
	}
	// 戰鬥中被敵人困住
	else if( !COMBAT_D->allow_escape(me) )
	{
		msg("$ME被敵人困住了，無法移動。\n", me, 0, 1);
		return 0;
	}
	// 被困住
	else if( me->is_stucking() )
	{
		tell(me, me->query_stuck_msg());
		return 0;	
	}
	//忙碌中不能移動
	else if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return 0;
	}
	else if( me->is_flying(LIMITED_FLY_TYPE) )
	{
		tell(me, pnoun(2, me)+"正在飛行途中，無法移動。\n");
		return 0;
	}
		
	return 1;
}

varargs nomask void move(mixed where)
{
	object me = this_object();
	array originalloc = copy(query_temp("location"));
	object env = environment();
	object *nearby_objects;

	if( !where ) return;

	switch(typeof(where))
	{
	case ARRAY:	
		{
			object new_env = env;

			where[CODE] = where[X]+"/"+where[Y]+"/"+where[Z]+"/"+where[PLACE]+"/"+where[NUM]+"/"+where[EXTRA];

			// leave()
			nearby_objects = present_objects(me) - ({ me });
			nearby_objects->leave(me);
			
			set("section_num", where[NUM]);
			MAP_D->leave_coordinate(originalloc, me);

			// 檢查是否要移至新的地圖物件
			if( arrayp(originalloc) )
			{
				// 移除人形圖像
				MAP_D->remove_living_icon(me, originalloc);

				if( !env || !env->is_maproom() || originalloc[NUM] != where[NUM] || originalloc[PLACE] != where[PLACE] )
					new_env = MAP_D->query_maproom(where);			
			}
			else
			{
				if( userp(me) && !(query("env/flag") & FLAG_NO_MAP) )
					startup_title_screen(me, 11);

				new_env = MAP_D->query_maproom(where);
			}

			if( objectp(env) && !env->is_maproom() )
				
				env->leave(me);
			
			if( objectp(new_env) && env != new_env )
			{
				move_object(new_env);
				set_temp("maproom", new_env);
			}

			// 必須使用 copy(), 否則座標會混亂
			set_temp("location", copy(where));
			MAP_D->enter_coordinate(where, me);
			
			if( userp(me) ) do_look();

			// init()
			nearby_objects = present_objects(me) - ({ me });
			nearby_objects->init(me);
			nearby_objects->autokill(me);

			// 設定人形圖像 
			MAP_D->set_living_icon(me, where);

			break;
		}
	case STRING:
		if( !file_exists(where) || !objectp(where = load_object(where)) )
		{
			tell(me, "這間房間已經不存在這個世界上了，回到巫師大廳...。\n");
			where = load_object(STARTROOM);
		}
	case OBJECT:
		{
			// leave()
			nearby_objects = present_objects(me) - ({ me });
			nearby_objects->leave(me);

			if( objectp(env) )
			{
				if( env->is_maproom() )
				{
					if( userp(me) )
						reset_screen(me);

					MAP_D->remove_living_icon(me, originalloc);
					delete_temp("location");

					MAP_D->leave_coordinate(originalloc, me);
				}
				else
					env->leave(me);
			}

			move_object(where);

			if( where == find_object(VOID_OB) ) return;
			
			if( userp(me) ) do_look();
			// init()
			where->init(me);

			nearby_objects = present_objects(me) - ({ me });
			nearby_objects->init(me);
			nearby_objects->autokill(me);
			break;
		}
	default:
		error(sprintf("無法找到或載入物件 %O 。\n", where));
		break;
	}
}

nomask varargs void move_to_environment(object ob)
{
	object env = environment(ob);

	if( !env ) return;

	if( env->is_maproom() )
		move( copy(query_temp("location", ob)) );
	else
		move( env );
}

// 單純移動物件至另一物件上
nomask void move_to_object(object where)
{
	if( where == this_object() || where == environment() ) return;

	move_object(where);
}

// 使跟隨者跟著移動
nomask void follower_move(mixed origin, mixed target)
{
	int moving_stamina_cost;
	string this_idname;
	object follower, *followers = copy(query_temp("follower"));

	if( !arrayp(followers) || !origin || !target ) return;

	this_idname = this_object()->query_idname();

	followers -= ({ 0 });
	
	if( !sizeof(followers) )
		delete_temp("follower");

	foreach( follower in followers )
	{
		if( !objectp(follower) ) continue;

		if( !query_temp("following", follower) )
			set_temp("following", this_object(), follower);

		if( arrayp(origin) && save_variable(copy(query_temp("location", follower))) != save_variable(origin) ) continue;
		if( objectp(origin) && origin != environment(follower) ) continue;
	
		// 是犯人
		if( query("prisoner", follower) )
		{
			msg("$ME的手腳被鐵鍊銬住，無法自由行動。\n", follower, this_object(), 1);
			continue;
		}
		
		if( !COMBAT_D->allow_escape(follower) )
		{
			msg("$ME被敵人困住了，無法移動。\n", follower, this_object(), 1);
			continue;
		}

		if( follower->is_delaying() )
		{
			tell(follower, follower->query_delay_msg());
			msg("$ME正在忙碌中，無法繼續跟隨$YOU。\n", follower, this_object(), 1);
			continue;
		}
		if( follower->is_faint() )
		{
			msg("$ME昏倒了，無法繼續跟隨$YOU。\n", follower, this_object(), 1);
			continue;	
		}

		if( follower->is_dead() )
		{
			msg("$ME死了，無法繼續跟隨$YOU。\n", follower, this_object(), 1);
			continue;	
		}

		moving_stamina_cost = -follower->query_all_buff(BUFF_MOVING_STAMINA);
		if( moving_stamina_cost > 0 && !follower->cost_stamina(moving_stamina_cost) )
		{
			msg("$ME已經沒有體力繼續前進了。\n", follower, this_object(), 1);
			continue;
		}
		
		if( follower->query_loading() > follower->query_max_loading() )
		{
			msg("$ME拖著沉重的身體前進，但終究是力不從心。\n", follower, this_object(), 1);
			follower->faint();
			continue;
		}

		msg("$ME跟隨著"+this_idname+"走了過去。\n", follower, 0, 1);
		follower->move(target);
		msg("$ME跟隨著$YOU走了過來。\n", follower, this_object(), 1, 0, ({ follower }));

		follower->follower_move(origin, target);
	}
}

private int move_or_destruct( object dest )
{
	if( this_object()->is_user_ob() )
	{
		delete("prisoner", this_object() );
		tell(this_object(), "你所在的環境"+(dest?"[ "+dest->query_name()+" ]":"")+"被摧毀了。\n一陣強光帶"+pnoun(2, this_object())+"回到了巫師大廳。\n");
		move(STARTROOM);
	}
	else if( this_object()->is_module_npc() )
		this_object()->save();
}
