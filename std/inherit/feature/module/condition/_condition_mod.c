/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _condition_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-29
 * Note   : object module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

// 回傳 Key 值
string query_key()
{
	return "condition/"+replace_string(base_name(this_object()), "/", "#");
}

// 回傳狀態英文名稱
string query_condition_id()
{
	return fetch_variable("id");
}

// 回傳狀態中文名稱
string query_condition_name()
{
	return fetch_variable("name");
}

// 回傳狀態型態名稱
string query_condition_type()
{
	return fetch_variable("type");
}

// 回傳狀態預設持續時間
int query_default_condition_time()
{
	return fetch_variable("time");
}

// 回傳狀態預設心跳時間
int query_default_condition_heart_beat()
{
	return fetch_variable("heartbeat");
}

void start_effect(object ob)
{
	msg("$ME進入「"+query_condition_name()+"」的"+query_condition_type()+"狀態。\n", ob, 0, 1);
}

// 結束狀態時的效果
void stop_effect(object ob)
{
	msg("$ME脫離「"+query_condition_name()+"」的"+query_condition_type()+"狀態。\n", ob, 0, 1);
}

// 狀態進行中的效果
void heart_beat_effect(object ob)
{

}
