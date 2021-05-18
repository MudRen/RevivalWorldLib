/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <citydata.h>
#include <fly.h>
#include <buff.h>

inherit PRODUCT_ACTION_MOD;

#define X	0
#define Y	1

// �}����
void do_fly(object me, string arg, object ob)
{
	int x, y, total_fuel, need_fuel;
	object fuel, *fuels, env = environment(me);
	string to_area, from_area;
	int to_num, from_num, *to_coor, *from_coor;
	int loctype;
	array loc, loc_temp;
	object target;
	array path;
	array map_data;
	array process = allocate(0);
			
	if( !me->valid_move() ) 
		return;

	if( !me->is_equipping_object(ob) )
		return tell(me, pnoun(2, me)+"�������˳�"+ob->query_idname()+"�C\n");

	if( !arg || sscanf(arg, "%s %d %d,%d", to_area, to_num, x, y) != 4 )
		return tell(me, pnoun(2, me)+"�Q�n���h���̡H(�ҡGlightforest 1 50,50)\n");

	to_area = lower_case(to_area);
	to_num--;
	x--;
	y--;

	if( env->is_maproom() )
	{
		if( !stringp(from_area = env->query_area()) )
			return tell(me, pnoun(2, me)+"�����b�����άO���Ϥ~����_���P�����C\n");
		else if( AREA_D->query_area_info(from_area, "need_flag") )
			return tell(me, pnoun(2, me)+"�L�k�q�o�Ӱϰ쭸������a��C\n");

		from_num = env->query_num();
	}
	else if( env->query_building_type() == "airport" && query("function", env) == "airstrip" )
	{
		from_area = env->query_city();
		from_num = env->query_city_num();
	}
	else
		return tell(me, pnoun(2, me)+"�����b�����άO���Ϥ~����_���P�����C\n");

	if( env->is_maproom() )
	{
		loc = query_temp("location", me);
		
		loctype = MAP_D->query_coor_data(loc, TYPE);
	
		if( loctype == SEA || loctype == RIVER )
			return tell(me, pnoun(2, me)+"�ثe�Ҧb��m�O���v�Ϊe�y�A�L�k�r�p�����C\n");
	}

	if( CITY_D->city_exist(to_area) )
	{
		string file = CITY_ROOM_MODULE(to_area, to_num, x, y, "airport");

		if( !objectp(target = load_module(file)) || query("function", target) != "airstrip" )
			return tell(me, "�y��"+loc_short(to_area, to_num, x, y)+"�èS���������]�D�C\n");
			
		loc = arrange_area_location(x, y, to_area, to_num);
	}
	else if( AREA_D->area_exist(to_area))
	{
		if( AREA_D->query_area_info(to_area, "need_flag") )
			return tell(me, pnoun(2, me)+"�ҳ]�w���ت��a�O�S��ϰ�A�L�k����ܸӳB�C\n");

		if( !AREA_D->valid_coordinate(x, y, to_area, to_num) )
			return tell(me, pnoun(2, me)+"������J���T���y�Ц�m(�ҡGlightforest 1 50,50)�C\n");
		
		loc = arrange_area_location(x, y, to_area, to_num);
		
		loctype = AREA_D->query_coor_data(loc, TYPE);

		if( loctype == SEA || loctype == RIVER )
			return tell(me, "�y��"+loc_short(loc)+"�O���v�Ϊe�y�A"+ob->query_idname()+"�L�k�b�ӳB�����C\n");
	}
	else
		return tell(me, pnoun(2, me)+"������J���T���y�Ц�m(�ҡGlightforest 1 50,50)�C\n");

 	fuels = filter_array(all_inventory(me), (: $1->query_module() == "liquid_fuel" :));
 	
 	total_fuel = implode(map(fuels, (: $1->query_amount() :)), (: $1 + $2 :));
 
 	from_coor = MAP_D->query_world_map_location(from_area, from_num);
 	to_coor = MAP_D->query_world_map_location(to_area, to_num);
 	
 	need_fuel = 1 + (to_int(pythagorean_thm(from_coor[X] - to_coor[X], from_coor[Y] - to_coor[Y]))/2);
 	
 	if( total_fuel < need_fuel )
 		return tell(me, pnoun(2, me)+"�ܤֻݭn "+need_fuel+" �ӲG�A�U�Ƥ~����_���e��"+loc_short(loc)+"�C\n");
 		
 	total_fuel = need_fuel;
 
 	foreach(fuel in fuels)
 	{
 		if( fuel->query_amount() >= total_fuel )
 		{
 			destruct(fuel, total_fuel);
 			break;
 		}
 		else
 		{
 			total_fuel -= fuel->query_amount();
 			destruct(fuel);
 		}
 	}
  
  path = ALGORITHM_PATH_D->search_map_path(from_coor, to_coor);
	
	me->start_fly(LIMITED_FLY_TYPE);
	
	foreach(array coor in path[1..<2])
	{
		map_data = MAP_D->query_world_map(coor[X], coor[Y]);

		loc_temp = arrange_area_location(range_random(0,99), range_random(0,99), map_data[0], map_data[1]);
		
		process += ({ 
			(: objectp($(me)) ? $(me)->move($(loc_temp)) : 0 :), 
			(: objectp($(me)) ? msg("$ME�r�p��$YOU���V"+loc_short($(loc_temp))+"���W��...�C\n", $(me), $(ob), 1) : 0 :),
			1 
		});
	}
	
 	if( arrayp(loc) )
  	{
  		process += ({ 
  			(: objectp($(me)) ? $(me)->move($(loc)) : 0 :),
  			(: objectp($(me)) ? msg("$ME�r�p��$YOU��í�a�����b"+loc_short($(loc))+"�C\n", $(me), $(ob), 1) : 0 :),
  			(: objectp($(me)) ? $(me)->stop_fly() : 0 :),
  		});
		msg("$ME�r�p"+ob->query_idname()+"���W�ѻڡA��O "+need_fuel+" �ӲG�A�U�ƫe��"+loc_short(loc)+"�C\n", me, 0, 1);
		process_function(process);
	}
	else if( objectp(target) )
	{
  		process += ({ 
  			(: objectp($(me)) ? $(me)->move($(target)) : 0 :),
  			(: objectp($(me)) ? msg("$ME�r�p��$YOU��í�a�����b"+loc_short($(loc))+"�C\n", $(me), $(ob), 1) : 0 :),
  			(: objectp($(me)) ? $(me)->stop_fly() : 0 :),
  		});
		msg("$ME�r�p"+ob->query_idname()+"���W�ѻڡA��O "+need_fuel+" �ӲG�A�U�ƫe��"+target->query_room_name()+"�C\n", me, 0, 1);
		process_function(process);
	}
	else
		error("�o�Ϳ��~�C\n");
		
	
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"fly"		: (: do_fly :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�Ƶ���",
	
	//�^��W��
	"id":		"glider",

	//���ƻݨD
	"material":	([ "tire":5, "plane_body":1, "engine":2, "steel":100, "integrated_circuit":5, "mechanical_structure":5, "man_made_fibers":30, "wire":10, "petroleum":500 ]),

	//�ޯ�ݨD
	"skill":	([ "aircraft-fac":40, "technology":40 ]),

	//�Ͳ��u�t
	"factory":	"aircraft",
]);
