/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : algorithm_path_d.c
 * Author : Clode@Revival World
 * Date   : 2008-01-17
 * Note   : 路徑搜尋演算法
 *
 -----------------------------------------
 */
//#pragma optimize

#include <daemon.h>

#define COST	0
#define MOTHER	1

//-----------------------------------------------------
// A* 路徑演算法
//-----------------------------------------------------
array search_path(array input, array sta_pos, array end_pos)
{
	int lowest_cost;
	int cost;

	array cur_pos;
	string str_cur_pos;
	array tmp_pos;
	string str_tmp_pos;

	array data;
	array result = allocate(0);

	mapping openlist = allocate_mapping(0);
	mapping closelist = allocate_mapping(0);

	// 起始節點
	
	cur_pos = sta_pos;
	str_cur_pos = save_variable(cur_pos);
	openlist[str_cur_pos] = ({ 0, 0 });

	while(sizeof(openlist))
	{
		// 取出 openlist 中成本最低節點
		lowest_cost = 0;
		foreach(str_tmp_pos, data in openlist)
		{
			if( data[COST] == 0 )
				continue;

			if( lowest_cost == 0 || data[COST] < lowest_cost )
			{
				lowest_cost = data[COST];
				cur_pos = restore_variable(str_tmp_pos);
				str_cur_pos = str_tmp_pos;
			}
		}
		closelist[str_cur_pos] = copy(openlist[str_cur_pos]);

		if( str_cur_pos != save_variable(end_pos) )
		{
			// 搜尋相鄰節點
			foreach(tmp_pos in input)
			{
				str_tmp_pos = save_variable(tmp_pos);
				
				if( pythagorean_thm(tmp_pos[0]-cur_pos[0], tmp_pos[1]-cur_pos[1]) <= 1 && undefinedp(openlist[str_tmp_pos]) && undefinedp(closelist[str_tmp_pos]) )
				{
					cost = 1 + openlist[str_cur_pos][COST] + to_int( pythagorean_thm(tmp_pos[0]-end_pos[0], tmp_pos[1]-end_pos[1]) );
					
					openlist[str_tmp_pos] = ({ cost, str_cur_pos });
				}
			}

			map_delete(openlist, str_cur_pos);
		}
		else
			break;
	}

	str_tmp_pos = save_variable(end_pos);
	
	if( !undefinedp(closelist[str_tmp_pos]) )
	{
		result = ({ end_pos });
	
		do
		{
			str_tmp_pos = closelist[str_tmp_pos][MOTHER];
	
			if( !str_tmp_pos ) break;
	
			result = ({ restore_variable(str_tmp_pos) }) + result;
	
		} while ( !undefinedp(closelist[str_tmp_pos]) );
	}

	return result;
}


array search_map_path(array sta_pos, array end_pos)
{
	array input = allocate(0);

	foreach(int x, mapping data in MAP_D->query_world_map())
	foreach(int y, array a in data)
		input |= ({ ({x, y}) });
		
	return search_path(input, sta_pos, end_pos);
}

string query_name()
{
	return "路徑搜尋演算法系統(ALGORITHM_PATH_D)";
}
