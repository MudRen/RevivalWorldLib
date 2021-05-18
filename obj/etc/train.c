/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : train.c
 * Author : Sinji@RevivalWorld
 * Date   : 2005-04-10
 * Note   : ���� (�Q���쵲��C)
 * Update :
 *  o 2008-01-16 Clode ���s�]�p�H�f�t�������\��
 *  o 2005-04-13 Sinji �ץ��{���Ĳv
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

// ���O�ʧ@
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
�@�@�u�~�ɥN���]�T�����C

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

		new_train->set_idname("train " + train_count, "�]�T������" + CHINESE_D->chinese_number(train_count) + "���c");
		set("short", HIW"�]�T"NOR WHT"������" + CHINESE_D->chinese_number(train_count) + "���c", new_train);
		set("long", copy(query("long", query_head_train())), new_train);
		
		new_train->set_prev_train(last_train);

		last_train->set_next_train(new_train);
	}
	while(--n);

	set("train_count", train_count, head_train);

	set_temp("status", HIY"�@ "+(train_count+1)+" �`���c"NOR, head_train);

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
	set_temp("status", HIY"(���c " + train_count + " �`)"NOR, head_train);
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
	broadcast(train_env, me->query_idname() + "�q" + train_idname + "�W���F�U�ӡC\n", 0, me);
	me->move(loc || train_env);
	msg("$ME�q���[�������F�X�h�C\n", me, 0, 1);
	
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

	set_idname("train", CITY_D->query_city_name(target->query_city(), target->query_city_num())+NOR HIW"�]�T"NOR WHT"����"NOR);
	set("flag/no_amount", 1);
	set("short", HIW"�]�T"NOR WHT"���������Y"NOR);
	set("long", "�@�@�C���ت��a�G"+CITY_D->query_city_name(target->query_city(), target->query_city_num())+target->query_room_name()+"\n\n�@�@�o�O�@�C�]�T�����A�����W���A�ȥͥ��b�ˤ��a�۩I�ȫȡA���~�s�諸������\n�_�a���L���ǡC���ɸg�L���O�c�ت����ϡA���ɸg�L���O���R������A���ɸg�L��\n�O�������G�D�C�K�D���L��ɪ������A�ȫȪ��ߤ]�H���}��_�ӡC");
	set("unit", "�x");
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
		msg("�u�ʹʡ�T��T���v�A$ME������I�S�ӥh...�C\n", this_object(), 0, 1);

	this_object()->move(path[0]);
	msg("�u�ʹʡ�T��T���v�A$ME�q����I�S�Ө�...�C\n", this_object(), 0, 1);

	set_heart_beat(PER_MAP_TIME);

	foreach(object train in query_all_trains())
	{
		if( sizeof(all_inventory(train)) )
			broadcast(train, HIC"�s"NOR CYN"��"NOR"�G�U��ȫȱz�n�A�w��f�����Z�C���A���Z�C���w�}�l�e��"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"�A�w�p��F�ɶ��G"+(sizeof(path)*PER_MAP_TIME)+" ���C\n");
	}
}

void heart_beat()
{
	path = path[1..];

	// ��F�ت��a
	if( !sizeof(path) )
	{
		object train, inv;
		
		if( !objectp(target_room) )
		{
			foreach(train in query_all_trains())
			foreach(inv in all_inventory(train))
			{
				inv->move(WIZ_HALL);
				msg("�������ت��a�w�g�����A$ME�୼�ܦ��a�U���C\n", inv, 0, 1);
			}
		}
		else
		{
			foreach(train in query_all_trains())
			{
				if( sizeof(all_inventory(train)) )
				{
					broadcast(train, HIC"�s"NOR CYN"��"NOR"�G�U��ȫȱz�n�A�����w��F"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"�A�ФŧѰO�z���H������C\n");
				
					foreach(inv in all_inventory(train))
					{
						inv->move(target_room);
						msg("$ME���U�����A�Ө�"+target_room->query_room_name()+"�C\n", inv, 0, 1);
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
			broadcast(train, HIC"�s"NOR CYN"��"NOR"�G�U��ȫȱz�n�A�����A�L "+sizeof(path)*PER_MAP_TIME+" ��Y�N��F"+CITY_D->query_city_name(target_room->query_city(), target_room->query_city_num())+target_room->query_room_name()+"�C\n");	
	}
	
	msg("�u�ʹʡ�T��T���v�A$ME������I�S�ӥh...�C\n", this_object(), 0, 1);
	this_object()->move(path[0]);
	msg("�u�ʹʡ�T��T���v�A$ME�q����I�S�Ө�...�C\n", this_object(), 0, 1);
}
