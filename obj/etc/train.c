/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : train.c
 * Author : Sinji@RevivalWorld
 * Date   : 2005-04-10
 * Note   : 火車 (利用鏈結串列)
 * Update :
 *  o 2008-01-16 Clode 重新設計以搭配火車站功能
 *  o 2005-04-13 Sinji 修正程式效率
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>
#include <inherit.h>
#include <feature.h>
#include <secure.h>
#include <lock.h>
#include <object.h>

inherit STANDARD_OBJECT;

#define TICKET		"/obj/systemproducts/train_ticket"
#define PER_MAP_TIME	3
void do_leave(object me, string arg);

// 指令動作
nosave mapping actions = 	
([
    "leave"		:	(: do_leave :),
]);

varargs int new_train(int n);

nosave object prev_train;
nosave object next_train;
nosave array path;
nosave object target_room;

int is_train_ob()
{
	return 1;
}

void set_next_train(object ob)
{
	next_train = ob;
	set("exits/next", next_train);
}

void set_prev_train(object ob)
{
	prev_train = ob;
	set("exits/prev", prev_train);
}

object query_prev_train()
{
	return prev_train;
}

object query_next_train()
{
	return next_train;
}

object query_head_train()
{
	if( objectp(prev_train) )
		return prev_train->query_head_train();

	return this_object();
}

object query_last_train()
{
	if( objectp(next_train) )
		return next_train->query_last_train();

	return this_object();
}


string long()
{
	string help = @HELP
　　工業時代的蒸汽火車。

HELP;
	return help;
}

varargs int new_train(int n)
{
	object head_train, last_train, new_train;
	int train_count;

	head_train = query_head_train();

	train_count = query("train_count", head_train);

	if( train_count >= 40 )
		return 0;

	if( train_count + n >= 40 )
		n = 40 - train_count;

	do
	{
		last_train = query_last_train();
		new_train = new(base_name(this_object()));

		if( !new_train )
			return 0;

		train_count++;

		new_train->set_idname("train " + train_count, "蒸汽火車第" + CHINESE_D->chinese_number(train_count) + "車箱");
		set("short", HIW"蒸汽"NOR WHT"火車第" + CHINESE_D->chinese_number(train_count) + "車箱", new_train);
		set("long", copy(query("long", query_head_train())), new_train);
		
		new_train->set_prev_train(last_train);

		last_train->set_next_train(new_train);
	}
	while(--n);

	set("train_count", train_count, head_train);

	set_temp("status", HIY"共 "+(train_count+1)+" 節車箱"NOR, head_train);

	if( !train_count )
		delete_temp("status", head_train);

	return 1;
}

int remove_train()
{
	object head_train, last_train, ptrain;
	int train_count;

	head_train = query_head_train();
	last_train = query_last_train();
	if( head_train == last_train ) return 0;

	ptrain = last_train->query_prev_train();
	train_count = query("train_count", head_train);

	all_inventory(last_train)->move(ptrain);

	delete("exits/next", ptrain);

	train_count--;
	set("train_count", train_count, head_train);
	set_temp("status", HIY"(車箱 " + train_count + " 節)"NOR, head_train);
	if( !train_count ) delete_temp("status", head_train);
	destruct(last_train);
	return 1;
}

void do_leave(object me, string arg)
{
	object train_env = environment(this_object());
	string train_idname = this_object()->query_idname();
	array loc;

	if( !train_env )
		train_env = environment(query_head_train());

	loc = query_temp("location", query_head_train());
	broadcast(train_env, me->query_idname() + "從" + train_idname + "上跳了下來。\n", 0, me);
	me->move(loc || train_env);
	msg("$ME從車廂之間跳了出去。\n", me, 0, 1);
	
	if( query_heart_beat() )
		me->faint();
}


void remove()
{
	object last_train;
	
	while(objectp(last_train = query_last_train()) && objectp(next_train)) 
		destruct(last_train);
} 

void create(object target)
{
	if( !objectp(target) )
	{
		call_out( (: destruct(this_object()) :), 1);
		return;
	}

	set_idname("train", CITY_D->query_city_name(target->query_city(), target->query_city_num())+NOR HIW"蒸汽"NOR WHT"火車"NOR);
	set("flag/no_amount", 1);
	set("short", HIW"蒸汽"NOR WHT"火車火車頭"NOR);
	set("long", "　　列車目的地："+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"\n\n　　這是一列蒸汽火車，火車上的服務生正在親切地招呼旅客，窗外廣闊的美景不\n斷地擦過身旁。有時經過的是繁華的市區，有時經過的是寧靜的平原，有時經過的\n是長長的隧道。鐵道似無止盡的延伸，旅客的心也隨之開闊起來。");
	set("unit", "台");
	set("mass", -1);

	target_room = target;

	new_train(1);
}


object *query_all_trains()
{
	object *all_trains = allocate(0);
	object train = this_object();
	
	do
	{
		all_trains |= ({ train });
		
	} while(objectp(train = train->query_next_train()));
	
	return all_trains;
}

void start()
{
	object from = environment();

	path = allocate(0);

	foreach(array loc in ALGORITHM_PATH_D->search_map_path(
		MAP_D->query_world_map_location(from->query_city(), from->query_city_num()),
		MAP_D->query_world_map_location(target_room->query_city(), target_room->query_city_num())
	))
	{
		loc = MAP_D->query_world_map(loc[0], loc[1]);
		
		loc = arrange_area_location(random(100), random(100), loc[0], loc[1]);
		
		path += ({ loc });
	}

	if( !sizeof(path) )
	{
		call_out( (: destruct(this_object()) :), 1);
		all_inventory(this_object())->move(environment(this_object()));
		return;
	}

	if( sizeof(all_inventory(from)) )
		msg("「嘟嘟～汽鏘汽鏘～」，$ME往遠方呼嘯而去...。\n", this_object(), 0, 1);

	this_object()->move(path[0]);
	msg("「嘟嘟～汽鏘汽鏘～」，$ME從遠方呼嘯而來...。\n", this_object(), 0, 1);

	set_heart_beat(PER_MAP_TIME);

	foreach(object train in query_all_trains())
	{
		if( sizeof(all_inventory(train)) )
			broadcast(train, HIC"廣"NOR CYN"播"NOR"：各位旅客您好，歡迎搭乘本班列車，本班列車已開始前往"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"，預計到達時間："+(sizeof(path)*PER_MAP_TIME)+" 秒後。\n");
	}
}

void heart_beat()
{
	path = path[1..];

	// 到達目的地
	if( !sizeof(path) )
	{
		object train, inv;
		
		if( !objectp(target_room) )
		{
			foreach(train in query_all_trains())
			foreach(inv in all_inventory(train))
			{
				inv->move(WIZ_HALL);
				msg("火車的目的地已經消失，$ME轉乘至此地下車。\n", inv, 0, 1);
			}
		}
		else
		{
			foreach(train in query_all_trains())
			{
				if( sizeof(all_inventory(train)) )
				{
					broadcast(train, HIC"廣"NOR CYN"播"NOR"：各位旅客您好，火車已到達"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"，請勿忘記您的隨身行李。\n");
				
					foreach(inv in all_inventory(train))
					{
						inv->move(target_room);
						msg("$ME走下火車，來到"+target_room->query_room_name()+"。\n", inv, 0, 1);
					}
				}
			}
		}
		destruct(this_object());
		return;
	}

	if( sizeof(path) == 2 )
	{
		foreach(object train in query_all_trains())
		if( sizeof(all_inventory(train)) )
			broadcast(train, HIC"廣"NOR CYN"播"NOR"：各位旅客您好，火車再過 "+sizeof(path)*PER_MAP_TIME+" 秒即將到達"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"。\n");	
	}
	
	msg("「嘟嘟～汽鏘汽鏘～」，$ME往遠方呼嘯而去...。\n", this_object(), 0, 1);
	this_object()->move(path[0]);
	msg("「嘟嘟～汽鏘汽鏘～」，$ME從遠方呼嘯而來...。\n", this_object(), 0, 1);
}
