/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _mapnpc.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-05-13
 * Note   : �a�Ϩt�ΤW�� NPC �w��P����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <location.h>
#include <daemon.h>
#include <nature.h>
#include <nature_data.h>
#include <map.h>

mapping corresponding =
([
	"north"		:	"south",
	"south"		:	"north",
	"east"		:	"west",
	"west"		:	"east",
	"northwest"	:	"southeast",
	"northeast"	:	"southwest",
	"southwest"	:	"northeast",
	"southeast"	:	"northwest",
	"up"		:	"down",
	"down"		:	"up",
]);

mapping weather_go =
([
    SUNNY		:	"���Y�j���a",
    CLOUDY		:	"���H�۩M��",
    RAINY		:	"�}����۫B��",
    WINDY		:	"���й���",
    SNOWY		:	"�a�۴����ճ�",
    SHOWER		:	"�q�Y��}��O�O�a",
    TYPHOON		:	"�����@��k��a",
    FOEHN		:	"���ۼ���"
]);

void startup_heart_beat()
{
	set_heart_beat(1);
}

varargs void load_mapnpc(array ref npc, string map_id, int map_num, int no_default_npc)
{
	int i, amount;
	int loc_check;
	string filename;
	float bonus;
	array loc;
	object ob;

	if( undefinedp(no_default_npc) )
		npc += load_object(__DIR__"all_area.c")->query_all_area_npc();

	for(i=0;i<sizeof(npc);i++)
	{
		npc[i]["objects"] = allocate(0);
		
		if( npc[i]["move_speed"] == -1 )
			npc[i]["move_speed_count"] = -1;
		else
			npc[i]["move_speed_count"] = random(npc[i]["move_speed"]);

		loc_check = 0;
		do
		{
			loc = arrange_location(
				range_random(npc[i]["x"][0], npc[i]["x"][1]), 
				range_random(npc[i]["y"][0], npc[i]["y"][1]), 
				0, 
				map_id, 
				map_num, 
				0
			);
			
			if( ++loc_check > 100 ) break;

		} while ( AREA_D->query_coor_data(loc, FLAGS) & NO_MOVE );
		
		foreach(filename, amount in npc[i]["group"])
		{
			bonus = 1. + (pow(amount, 3.5)/100.);

			while(amount--)
			{
				ob = new(filename);
				ob->move(loc);
				broadcast(loc, HIY+ob->query_idname()+HIY"��M�q����«�F�X�ӡI\n"NOR);
				set_temp("mapnpc", 1, ob);
				npc[i]["objects"] += ({ ob });
			}
		}
	}
	
	call_out( (: startup_heart_beat :), random(10) );
}

int unload_mapnpc(array ref npc)
{
	int i;
	object ob;

	for(i=0;i<sizeof(npc);i++)
	{
		if( arrayp(npc[i]["objects"]) )
		foreach(ob in npc[i]["objects"])
		{
			if( objectp(ob) )
			{
				ob->do_command("die �t�Χ�s");
				destruct(ob);
			}
		}
				
		npc[i]["objects"] = allocate(0);
	}
}

void mapnpc_heart_beat(array ref npc, string map_id, int map_num)
{
	int i, j, size;
	object ob;
	array loc;
	array firstloc;
	string *valid_direction;
	string direction;
	array nextloc;
	string filename;
	mapping current_objects_count;
	int amount;
	int loc_check;

	size = sizeof(npc);

	for(i=0;i<size;i++)
	{
		// �M�����s�b������
		npc[i]["objects"] -= ({ 0 });

		// ��Ĥ@�Ӫ���ӧP�_�ثe�y��	
		if( sizeof(npc[i]["objects"]) )
			loc = query_temp("location", npc[i]["objects"][0]);
		else
			loc = 0;

		current_objects_count = allocate_mapping(0);
		// �p��{���ƶq, ��X�ثe�y��
		foreach(ob in npc[i]["objects"])
			current_objects_count[base_name(ob)]++;

		
		// �ˬd��ӹζ�������O�_�٦s�b
		foreach(filename, amount in npc[i]["group"])
		{
			// ��ڼƶq�֩�]�w�ƶq(�����i�歫��)
			if( current_objects_count[filename] < amount )
			{
				if( undefinedp(npc[i]["reborn_filename"]) )
					npc[i]["reborn_filename"] = allocate_mapping(0);
					
				if( undefinedp(npc[i]["reborn_filename"][filename]) )
					npc[i]["reborn_filename"][filename] = allocate(0);
			
				// �Y�ثe�ƧǤ������͸�Ƥ����ʤ֪������ -> �s�W���͸��
				if( sizeof(npc[i]["reborn_filename"][filename]) < amount - current_objects_count[filename] )
					npc[i]["reborn_filename"][filename] += allocate(amount - current_objects_count[filename], npc[i]["reborn_time"]);

				// ���˥ثe���͸�� -> �˼Ƨ����i�歫��
				for(j=0;j<sizeof(npc[i]["reborn_filename"][filename]);j++)
				{
					// �˼Ƨ���, �i�歫��
					if( !--npc[i]["reborn_filename"][filename][j] )
					{
						loc_check = 0;
						// �S���y��, �إ߷s�y��
						if( !arrayp(loc) )
						{
							do
							{
								loc = arrange_location(
									range_random(npc[i]["x"][0], npc[i]["x"][1]), 
									range_random(npc[i]["y"][0], npc[i]["y"][1]), 
									0, 
									map_id, 
									map_num, 
									0
								);
								
								if( ++loc_check > 100 ) break;

							} while ( AREA_D->query_coor_data(loc, FLAGS) & NO_MOVE );
						}
						
						ob = new(filename);
						ob->move(loc);
						broadcast(loc, HIY+ob->query_idname()+HIY"��M�q����«�F�X�ӡI\n"NOR);
						set_temp("mapnpc", 1, ob);
						npc[i]["objects"] += ({ ob });
					}
				}
				npc[i]["reborn_filename"][filename] -= ({ 0 });
				
				if( !sizeof(npc[i]["reborn_filename"][filename]) )
					map_delete(npc[i]["reborn_filename"], filename);
			}
		}

		// �Y�q�q���٨S����, �N���L		
		if( !sizeof(npc[i]["objects"]) ) continue;

		// �i�沾�ʳB�z
		if( npc[i]["move_speed_count"] != -1 && --npc[i]["move_speed_count"] <= 0 )
		{
			npc[i]["move_speed_count"] = npc[i]["move_speed"];
			
			// �����ʽd�򪺸ܴN�i�沾��
			if( npc[i]["x"][0] != npc[i]["x"][1] || npc[i]["y"][0] != npc[i]["y"][1] )
			{
				firstloc = copy(loc);

				foreach(ob in npc[i]["objects"])
				{
					loc = query_temp("location", ob);
			
					// �Y�O�Ĥ@���άO����X�����y�лP�Ĥ@�����P��
					if( ob == npc[i]["objects"][0] || firstloc[X] != loc[X] || firstloc[Y] != loc[Y] )
					{
						valid_direction = allocate(0);
						
						if( loc[X] > npc[i]["x"][0] ) valid_direction += ({ "west" });
						if( loc[X] < npc[i]["x"][1] ) valid_direction += ({ "east" });
						if( loc[Y] > npc[i]["y"][0] ) valid_direction += ({ "north" });
						if( loc[Y] < npc[i]["y"][1] ) valid_direction += ({ "south" });
		
						direction = valid_direction[random(sizeof(valid_direction))];
						
						switch(direction)
						{
							case "west":
								nextloc = arrange_location(loc[X]-1, loc[Y], 0, map_id, map_num, 0);
								break;
							case "east":
								nextloc = arrange_location(loc[X]+1, loc[Y], 0, map_id, map_num, 0);
								break;
							case "north":
								nextloc = arrange_location(loc[X], loc[Y]-1, 0, map_id, map_num, 0);
								break;
							case "south":
								nextloc = arrange_location(loc[X], loc[Y]+1, 0, map_id, map_num, 0);
								break;
						}
					}

					if( COMBAT_D->in_fight(ob) || ob->is_faint() || ob->is_dead() || AREA_D->query_coor_data(nextloc, FLAGS) & NO_MOVE )
						continue;

					broadcast(loc, ob->query_idname()+"��"+CHINESE_D->to_chinese(direction)+"���}�C\n", 0, ob);
					ob->move(nextloc);
					broadcast(nextloc, ob->query_idname()+weather_go[NATURE_D->query_nature(AREA_D->query_maproom(loc))[NATURE_WEATHER][WEATHER_ID]]+"�A��"+CHINESE_D->to_chinese(corresponding[direction])+"���F�L�ӡC\n", 0, ob);
					
					ob->follower_move(loc, nextloc);	
				}
			}
		}
	}	
}