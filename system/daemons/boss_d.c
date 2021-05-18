/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : boss_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-10
 * Note   : �޲z�S�� BOSS ���t��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>
#include <location.h>
#include <map.h>

#define POSEIDON_PATH	"/obj/npc/boss/poseidon"

int poseidon_exists()
{
	object poseidon = find_object(POSEIDON_PATH);
	
	return objectp(poseidon) && objectp(environment(poseidon));
}

void create_poseidon()
{
	int num;
	array loc;
	object env;
	string *areas;
	string area;
	object poseidon = load_object(POSEIDON_PATH);
	int loctype;
	
	if( !objectp(poseidon) ) return;
	
	if( objectp(env = environment(poseidon)) && env->is_maproom() )
	{
		loc = query_temp("location", poseidon);
		
		if( AREA_D->is_area_location(loc) )
		 	return;
	}
			
	areas = AREA_D->query_areas();
	
	do
	{
		area = areas[random(sizeof(areas))];
	}
	while(AREA_D->query_area_info(area, "need_flag"));
	
	num = random(AREA_D->query_area_nums(area));
	
	do
	{
		loc = arrange_area_location(range_random(0, 99), range_random(0, 99), area, num);
		loctype = AREA_D->query_coor_data(loc, TYPE);
	}
	while(loctype != SEA && loctype != RIVER);
	
	set_temp("killed_boats", range_random(14,37), poseidon);

	CHANNEL_D->channel_broadcast("news", HIY"���~�Ǧ^����G"+AREA_D->query_area_name(area)+HIY"�����v�Ϊe�y��u�娣���㪺����"+poseidon->query_idname()+HIY"�X�{"NOR);
	CHANNEL_D->channel_broadcast("news", HIY"�ڱx�w�� "+query_temp("killed_boats", poseidon)+" ����D�켫�㪺�������I�A�ɭP�@�ɶT����B�j�áA�v���޵o�@�ɸg�ٿ���"NOR);
	
	poseidon->move(loc);
}

void notify_poseidon()
{
	array loc;
	object poseidon;
	
	if( !poseidon_exists() ) return;
		
	poseidon = find_object(POSEIDON_PATH);
	
	loc = query_temp("location", poseidon);
	
	if( AREA_D->is_area_location(loc) )
		CHANNEL_D->channel_broadcast("news", HIY"���㪺����"+poseidon->query_idname()+HIY"����������L��A�ڱx�w�� "+addn_temp("killed_boats", range_random(14,37), poseidon)+" ����b"+loc_short(loc)+HIY"�D�����I�A�@�ɸg�٫��򮣷W"NOR);	
}

// called by TIME_D
void process_per_hour()
{
	if( poseidon_exists() )
		notify_poseidon();
	else if( !random(18) )
		call_out( (: create_poseidon :), random(3600) );
}

void create()
{
}

int remove() { }

string query_name()
{
	return "BOSS �޲z�t��(BOSS_D)";
}
