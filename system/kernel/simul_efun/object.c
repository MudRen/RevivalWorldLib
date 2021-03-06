/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : object.c
 * Author : Clode@RevivalWorld
 * Date	  : 2001-7-2
 * Note	  : included by	Simul_Efun Object
 * Update :
 *  o 2002-01-05 Cookys	加上註解

 -----------------------------------------
 */

#include <daemon.h>
#include <location.h>


/* 傳回跟 ob 在同一環境或同一座標內之物件陣列, 包括 ob 本身 */
/* 此 simul_efun 對於 mud 效率有很大的影響, 若有必要, 需改寫成真正的 efun */
object *present_objects(object ob)
{
	// env 為目前物件所在的空間
	object env = environment(ob);
	
	// 沒有環境！？
	if( !env ) return allocate(0);
	
	// 只有在 map_d	的地圖系統才需要判斷
	if( env->is_maproom() )
		return MAP_D->coordinate_inventory(query_temp("location", ob));
	
	// 如果在 map_d	系統以外直接傳回 efun:all_inventory 的值
	return all_inventory(env);
}


int sort_objects_sortfunc(object ob1, object ob2)
{
	if( ob1->is_board() ) return 1;
	if( ob2->is_board() ) return -1;
	
	if( ob1->is_dead() ) return 1;
	if( ob2->is_dead() ) return -1;
	
	if( ob1->is_user_ob() ) return	1;
	if( ob2->is_user_ob() ) return	-1;
	
	if( ob1->is_npc() ) return 1;
	if( ob2->is_npc() ) return -1;
	
	if( ob1->is_equipping() ) return 1;
	if( ob2->is_equipping() ) return -1;
	
	if( ob1->is_keeping() ) return 1;
	if( ob2->is_keeping() ) return -1;
	
	return -1;
}

/*
	用於遊戲內容物件排序
	物件排序順序為 NPC -> ITEM -> PLAYER ->	BOARD
*/
object *sort_objects(object *obarr)
{
	object *retarr = allocate(0);
	array uniarr;
	
	if( !sizeof(obarr) )	
		return retarr;
	
	// 進行分類	
	uniarr = unique_array(obarr, 
	    (:	
		$1->is_dead() 		? 1 : 
		userp($1) 		? 2 : 
		$1->is_npc() 		? 3 : 
		$1->is_board() 		? 4 : 
		$1->is_equipping() 	? 5 : 
		$1->is_keeping() 	? 6 : 
		7
	    :)
	);
	
	// 對分類排序
	uniarr = sort_array(uniarr, (: sort_objects_sortfunc($1[0], $2[0]) :));
	
	foreach( object* unique	in uniarr )
	retarr += sort_array(unique, (: -strcmp($1->query_id(1)||base_name($1), $2->query_id(1)||base_name($2)) :));
	
	return retarr;
}


/* 根據	str 找尋物件 */
//note:	object str 刪除不用 (就是:「str」也可以是一個物件，則「str」是尋找，而非呼叫函式 id()。這個)
// str 是欲尋找的字串 ,	若有ob則尋找 ob	內含物
varargs	object present(	string str, object ob )
{
	object *inv;
	
	if( !str || !str[0] ) return 0;
	
	// 有沒有 ob 傳入，有則檢查是否位於特殊	map_d system
	if( objectp(ob)	)
	{
		// 位於	map_d 中，用特定的 present_object simul_efun.
		if( ob->is_maproom() )
			inv = sort_objects(present_objects(this_player()));
		else
			inv = sort_objects(all_inventory(ob));
	}
	else
		// 沒有	ob 傳入，自動指派上一個物件
	{
		object obj = previous_object(-1)[<1];
		object env = environment(obj);
		inv = sort_objects(all_inventory(obj));
		
		// 對於上個物件而言，有兩部分物件，一個是所在區的物件，另一個是身上物件
		if( objectp(env) )
		{
			// 是否在 map_d	中
			if( env->is_maproom() )
				inv += sort_objects(present_objects(obj));
			else
				inv += sort_objects(all_inventory(env));
		}
	}
	
	return object_parse(str, inv);
}


/* 檢查物件 ob 是否為指令物件 */
varargs	int is_command(object ob)
{
	// 如果沒有傳入	ob, 自動取上一個呼叫物件
	if( !objectp(ob) ) ob =	previous_object();
	// 檢查是否位於指令目錄，並且沒被 clone	過
	return file_name(ob)[0..5] == "/cmds/" && !clonep(ob);
}
/* 檢查物件 ob 是否為精靈物件 */
varargs	int is_daemon(object ob)
{
	// 如果沒有傳入	ob, 自動取上一個呼叫物件
	if( !objectp(ob) ) ob =	previous_object();
	// 檢查是否位於精靈目錄，並且沒被 clone	過
	return file_name(ob)[0..15] == "/system/daemons/" && !clonep(ob);
}

/* 摧毀傳入的 ob , ob 可為 str or object type */
varargs	int destruct(mixed ob, int amount)
{
	object env, *obarr;

	if( stringp(ob)	) 
		ob = find_object(ob);
	
	if( objectp(ob)	)
		obarr =	({ ob });
	else if( arrayp(ob) )
		obarr =	ob;
	else
		return 0;
	
	foreach( ob in obarr )
	{
		if( !objectp(ob) ) continue;
		
		/* only for debug */
		/*
		if( !clonep(ob) )
		{
			CHANNEL_D->channel_broadcast("nch", sprintf("主物件 %s 遭 %O 摧毀",  base_name(ob), previous_object()));
			log_file("root_object", sprintf("主物件 %s 遭 %O 摧毀",  base_name(ob), previous_object()));
		}
		*/
		
		if( ob->query_database() ) 
		{
			// 摧毀數量少於總數
			if( !undefinedp(amount) && amount > 0 && amount < ob->query_amount() )
			{
				ob->add_amount(-amount);
				continue;
			}
			// 全部摧毀(移除環境設定)
			else if( objectp(env = environment(ob)) )
			{
				if( env->is_maproom() )
				{
					if( ob->is_living() )
						MAP_D->remove_living_icon(ob, query_temp("location", ob));
					
					MAP_D->leave_coordinate(query_temp("location", ob), ob);
				}
				else if( env->is_living() )
					env->remove_action(ob);
			}
		}
		
		// 若此物件正被裝備中, 強迫卸除裝備
		if( ob->is_equipping() )
		{
			int status;
			object *equippers = ob->query_equipping();
			
			if( sizeof(equippers) )
			{
				foreach(object equipper in equippers)
				if( objectp(equipper) )
					equipper->unequip(ob, ref status);	
			}
		}
		
		catch(ob->remove());
		
		// 呼叫	efun 摧毀物件
		efun::destruct(ob);
	}
	return 1;
}

object load_user(string	id)
{
	object user;
	
	if( !stringp(id) )
		return 0;
	
	if( objectp(user = find_player(id)) )
		return user;
	
	if( !objectp(user = load_object(user_file_name(id))) )
		return 0;
	
	if( user->restore() )
		return user;
	
	destruct(user);
	return 0;
}

int user_exists(string id)
{
	object user;
	object online = find_player(id);
	
	if( !stringp(id) ) return 0;
	
	user = load_user(id);
	
	if( !user ) return 0;
	
	if( !online )
		destruct(user);
	
	return 1;
}

object load_module(mixed filename)
{
	if( !filename ) return 0;
	
	if( arrayp(filename) )
	{
		foreach( string room in this_object()->city_roomfiles(filename) )
		{
			if( sscanf(room, "/city/%*s/%*d/room/%*d_%*d_%*d_%*s") == 6 )
				continue;
			
			return load_object(room[0..<3]);
		}			
		return 0;
	}			
	
	if( !stringp(filename) || !file_exists(filename) ) return 0;
	
	return load_object(filename);
}

int same_environment(object ob1, object ob2)
{
	return objectp(ob1) && objectp(ob2) && member_array(ob2, present_objects(ob1)) != -1;
}

