/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : resource.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : �귽�Ķ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <buff.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";

mapping type_name = ([
"metal"	:	"/obj/materials/metal"->query_idname(),
"stone" :	"/obj/materials/stone"->query_idname(),
"water"	:	"/obj/materials/water"->query_idname(),
"wood"	:	"/obj/materials/wood"->query_idname(),
"fuel"	:	"/obj/materials/fuel"->query_idname(),
]);

array occupied_areas(string city)
{
	int i;
	string area, *sort_city;
	mapping area_section_influence;
	array occupied_areas = allocate(0);

	foreach(area in AREA_D->query_areas())
	{	
		for(i = 0;i<AREA_D->query_area_nums(area);++i)
		{
			area_section_influence = AREA_D->query_section_info(area, i, "area_section_influence");
			
			if( !sizeof(area_section_influence) )
				continue;	

			sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));

			if( !sizeof(sort_city) || sort_city[0] != city || area_section_influence[city]< 50000)
				continue;

			occupied_areas += ({ ({ area, i}) });
		}
	}	
	
	return occupied_areas;
}

void heart_beat(object room)
{
	string type;
	string file;
	object warehouse;
	object boss;
	object *labors = allocate(0);
	int amount;
	int total_amount;
	int users_inside;
	string owner = query("owner", room);
	array occupied_areas;
	array selected_area;
	int stamina_cur;
	int stamina_cost;
	int collect_buff;
	
	if( !objectp(boss = find_player(owner)) )
		return;

	foreach(object ob in all_inventory(room))
	{
		if( ob->is_module_npc() && query("job/type", ob) == WORKER && query("boss", ob) == owner )
			labors += ({ ob });
		else if( userp(ob) )
			users_inside = 1;
	}

	if( !sizeof(labors) ) return;

	type = query("setup/type", room);
	file = query("setup/warehouse", room);
	warehouse = load_module(file);
	
	if( !type )
	{
		if( users_inside )
			broadcast(room, "�|���]�w�Ķ������A���u�L�k�i��Ķ��@�~�C\n");
	
		return;
	}

	if( !objectp(warehouse) )
	{
		if( users_inside )
			return broadcast(room, "�|���]�w���x���ߡA���u�L�k�i��Ķ��@�~�C\n");
		
		return;
	}

	occupied_areas = occupied_areas(room->query_city());

	if( !sizeof(occupied_areas) )
	{
		if( users_inside )
			return broadcast(room, CITY_D->query_city_idname(room->query_city())+"�|��������󭥰ϡA�L�k�i��Ķ��u�@�C\n");
			
		return;
	}

	collect_buff = boss->query_all_buff(BUFF_COLLECT_ABILITY);
	
	file = "/obj/materials/"+type;
	
	foreach(object ob in labors)
	{
		stamina_cur = ob->query_stamina_cur();
		stamina_cost = (stamina_cur/75)*75;

		amount = stamina_cur / 75 * ob->query_skill_level(type) / 100;
		amount += amount * collect_buff / 100;

		if( amount > 0 )
			ob->cost_stamina(stamina_cost);
		
		if( users_inside )
		{
			if( amount > 0 )
			{
				selected_area = occupied_areas[random(sizeof(occupied_areas))];
				broadcast(room, ob->query_idname()+"��"+loc_short(selected_area[0],selected_area[1], random(100), random(100))+"�Ķ��^�� "+amount+" ��"+type_name[type]+"�C\n");
			}
			else
			{
				broadcast(room, ob->query_idname()+"�]����O�αĶ��ޯ઺�v�T�L�k�A�Ķ��귽�^�ӡC\n");
			}
		}	
		total_amount += amount;
	}

	if( total_amount <= 0 )
		return;

	input_object(warehouse->query_master(), "products", file, total_amount);
	warehouse->delay_save(1200);
}

string look_room(object room)
{
	string type = query("setup/type", room);
	string file = query("setup/warehouse", room);
	object warehouse = load_module(file);

	if( query("function", room) == "line" )
		return "���x���ߡG"+(objectp(warehouse) ? warehouse->query_room_name() : "���]�w")+"�B�Ķ������G"+ (type?type_name[type]:"���]�w")+"\n";

	return "";
}

void do_setup(object me, string arg)
{
	int x, y;
	object env = environment(me);
	string city = env->query_city();
	int num = env->query_city_num();
	string type = query("setup/type", env);
	string file = query("setup/warehouse", env);
	object warehouse = load_module(file);

	if( query("owner", env) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�Ӹ귽�Ķ������֦��̡C\n");

	if( !arg )
		return tell(me, "���x���ߡG"+(objectp(warehouse) ? warehouse->query_room_name() : "���]�w")+"\n�Ķ������G"+ (type?type_name[type]:"���]�w")+"\n");
	else if( arg == "warehouse -d" )
	{
		if( !objectp(warehouse) )
			return tell(me, "�o�̭쥻�K�S���]�w���x���ߡC\n");
			
		delete("setup/warehouse", env);
		return tell(me, "�������x���߳]�w�C\n");
		
	}
	else if( sscanf(arg, "warehouse %d,%d", x, y) == 2 )
	{
		file = CITY_ROOM_MODULE(city, num, (x-1), (y-1), "warehouse");

		if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
			return tell(me, "�y��"+loc_short(city, num, x-1, y-1)+"�èS���ܮw�C\n");

		if( query("owner", warehouse) != me->query_id(1) )
			return tell(me, warehouse->query_room_name()+"�ä��ݩ�"+pnoun(2, me)+"�C\n");
			
		set("setup/warehouse", file, env);
			
		env->save();
			
		return msg("$ME�]�w"+warehouse->query_room_name()+"�����귽�����Ϫ����x���ߡC\n", me, 0, 1);
	}
	else if( arg == "type -d" )
	{
		if( !type )
			return tell(me, "�o�̭쥻�K�S���]�w�귽�Ķ������C\n");
			
		delete("setup/type", env);
		return tell(me, "�����귽�Ķ������]�w�C\n");
	}
	else if( sscanf(arg, "type %s", type) == 1 )
	{
		if( undefinedp(type_name[type]) )
			return tell(me, "�S�� "+type+" �o�ظ귽�����C\n");
			
		set("setup/type", type, env);
		
		env->save();
	
		return msg("$ME�]�w���귽�����Ϫ��귽�Ķ������]�w���u"+type_name[type]+"�v�C\n", me, 0, 1);
	}
	else
		return tell(me, "�п�J���T�����O�榡�C\n");
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"line"	:
	([
		"short"	: NOR WHT"�@�~��"NOR,
		"help"	:
			([
"topics":
@HELP
    �귽�Ķ������ѭ��u�i�椭�j�򥻸귽�Ķ��@�~�C
HELP,

"setup":
@HELP
�]�w�ܮw��ƪ����O�A�Ϊk�p�U�G[�޲z���O]
  setup warehouse 21,33		- �]�w���귽�^�����������귽�ܮw�y�Цb 21,33
  setup warehouse -d		- �����ܮw�]�w
  setup type '�귽����'		- �]�w�Ķ��귽����(metal�Bstone�Bwater�Bwood�Bfuel)
  setup type -d			- �����Ķ��귽�����]�w
HELP,
			]),
		"heartbeat":30,	// ��ڮɶ� 1 �����
		"job": WORKER,
		"action":
			([
				"setup"		: (: do_setup :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"�귽"NOR YEL"�Ķ���"NOR

	// �}�i���ؿv�����̤֩ж�����
	,5

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,INDUSTRY_REGION

	// �}�i�����O��
	,5000000

	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,2
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,2
});
