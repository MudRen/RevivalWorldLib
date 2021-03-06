/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _room_action_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-08-24
 * Note   : room action module
 * Update :
 *
 *
 -----------------------------------------
 */

#include <daemon.h>

void create()
{
	//int length;
	string this_basename = base_name(this_object())+".c";
	
	// 暴力更新所有使用此模組之房間 action...
	objects( (: $1->query_module_file() == $(this_basename) :) )->enable_action();
}

array query_building_info()
{
	return fetch_variable("building_info");
}

mapping query_action_info()
{
	return fetch_variable("action_info");
}
