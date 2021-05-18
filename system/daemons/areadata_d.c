/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : areadata_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-18
 * Note   : �ϰ���F�D��, �B�z�Ҧ��ϰ��Ʈw
 * Update :
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <areadata.h>

private mapping areas;

mapping query_data()
{
	return areas;
}

/* ���m�Ҧ���� */
void restore_all_data()
{
	string area;
	mixed num;

	areas = allocate_mapping(0);

	/* ��l��Ū���Ҧ��ϰ��T */
	foreach( area in get_dir(AREA_ROOT) )
	{
		if( file_size(AREA_PATH(area)) != -2 ) continue;

		if(!read_file(AREA_INFO(area)))
		{
			CHANNEL_D->channel_broadcast("sys", sprintf("%s �gŪ����r��Ȭ� 0 ... ���L�C", AREA_INFO(area)));
			continue;
		}

		areas[area] = allocate_mapping(0);

		areas[area][INFO] = restore_variable(read_file(AREA_INFO(area)));

		// Ū���Ҧ��l�ϰ���
		foreach( num in get_dir(AREA_PATH(area)) )
		{
			if( !sscanf(num, "%d", num) ) continue;
			
			reset_eval_cost();

			areas[area][num] = allocate_mapping(0);

			CHANNEL_D->channel_broadcast("sys", "�ϰ�G��l�ưϰ� "+area+" ���� "+num+" �Ϧa�ϸ�Ʈw");

			areas[area][num][DATA] = restore_variable(read_file(AREA_NUM_DATA(area, num)));
			areas[area][num][MAP] = restore_variable(read_file(AREA_NUM_MAP(area, num)));
			areas[area][num][RMAP] = copy(areas[area][num][MAP]);
			areas[area][num][AROOM]= load_arearoom(area, num);
		}
	}

	AREA_D->link_data();
}

int remove()
{
	return AREA_D->save_all();
}

void create()
{
	if( clonep() )
		destruct(this_object());

	restore_all_data();
}

string query_name()
{
	return "���ϸ�Ʈw(AREADATA_D)";
}
