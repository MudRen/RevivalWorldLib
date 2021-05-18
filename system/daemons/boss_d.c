/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : boss_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-10
 * Note   : 管理特殊 BOSS 的系統
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

	CHANNEL_D->channel_broadcast("news", HIY"海外傳回急報："+AREA_D->query_area_name(area)+HIY"的海洋或河流航線驚見憤怒的海神"+poseidon->query_idname()+HIY"出現"NOR);
	CHANNEL_D->channel_broadcast("news", HIY"據悉已有 "+query_temp("killed_boats", poseidon)+" 艘船隻遭到憤怒的海神擊沉，導致世界貿易航運大亂，逐漸引發世界經濟蕭條"NOR);
	
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
		CHANNEL_D->channel_broadcast("news", HIY"憤怒的海神"+poseidon->query_idname()+HIY"持續攻擊路過船隻，據悉已有 "+addn_temp("killed_boats", range_random(14,37), poseidon)+" 艘船隻在"+loc_short(loc)+HIY"遭到擊沉，世界經濟持續恐慌"NOR);	
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
	return "BOSS 管理系統(BOSS_D)";
}
