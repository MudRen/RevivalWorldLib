/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : citydata_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-10
 * Note   : �������F�D��, �B�z�Ҧ�������Ʈw
 * Update :
 *  o 2002-09-17 Clode ���s�]�p������Ƶ��c�P�x�s��Ƶ{��
 *  o 2002-09-17 Clode �קﳡ�� code �����e����O
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <citydata.h>

private mapping cities;

mapping query_data()
{
	return cities;
}

/* ���m�Ҧ���� */
void restore_all_data()
{
	string city, roomfile;
	mixed num;

	cities = allocate_mapping(0);

	/* ��l��Ū���Ҧ�������T */
	foreach( city in get_dir(CITY_ROOT) )
	{
		if( file_size(CITY_PATH(city)) != -2 ) continue;

		if(!read_file(CITY_INFO(city))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s �gŪ����r��Ȭ� 0 ... ���L�C", CITY_INFO(city)));
			continue;
		}

		cities[city] = allocate_mapping(0);

		if(!(cities[city][INFO] = restore_variable(read_file(CITY_INFO(city))))) {
			CHANNEL_D->channel_broadcast("sys", sprintf("%s �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C", CITY_INFO(city)));
		}
	
		
		// Ū���Ҧ��l�������
		foreach( num in get_dir(CITY_PATH(city)) )
		{
			if( !sscanf(num, "%d", num) ) continue;

			CHANNEL_D->channel_broadcast("sys", "�����G��l�ƫ��� "+city+" ���� "+num+" ���Ϧa�ϸ�Ʈw");

			cities[city][num] 	= allocate_mapping(0);

			if(!read_file(CITY_NUM_DATA(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "DATA �gŪ����r��Ȭ� 0 ... ���L�C");
			}
			else if(!(cities[city][num][DATA] = restore_variable(read_file(CITY_NUM_DATA(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "DATA �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C");
			}

			if(!read_file(CITY_NUM_MAP(city, num))) {
				CHANNEL_D->channel_broadcast("sys", "MAP �gŪ����r��Ȭ� 0 ... ���L�C");
			}
			else if(!(cities[city][num][MAP] = restore_variable(read_file(CITY_NUM_MAP(city, num))))) {
				CHANNEL_D->channel_broadcast("sys", "MAP �Ҧs��ƥH restore_variable Ū�X���G�� 0 ... ���L�C");
			}
			else
				cities[city][num][RMAP] = copy(cities[city][num][MAP]);

			cities[city][num][CROOM] = load_cityroom(city, num);

			// ���J�Ҧ� Module Room
			foreach( roomfile in get_dir(CITY_NUM_ROOM(city, num)) )
			{
				reset_eval_cost();

				catch(load_object(CITY_NUM_ROOM(city, num)+roomfile[0..<3]));
			}
		}
	}

	CITY_D->link_data();
}

int remove()
{
	return CITY_D->save_all();
}

void create()
{
	if( clonep() )
		destruct(this_object());

	restore_all_data();
}

string query_name()
{
	return "������Ʈw(CITYDATA_D)";
}
