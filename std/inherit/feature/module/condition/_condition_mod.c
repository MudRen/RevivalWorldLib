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

// �^�� Key ��
string query_key()
{
	return "condition/"+replace_string(base_name(this_object()), "/", "#");
}

// �^�Ǫ��A�^��W��
string query_condition_id()
{
	return fetch_variable("id");
}

// �^�Ǫ��A����W��
string query_condition_name()
{
	return fetch_variable("name");
}

// �^�Ǫ��A���A�W��
string query_condition_type()
{
	return fetch_variable("type");
}

// �^�Ǫ��A�w�]����ɶ�
int query_default_condition_time()
{
	return fetch_variable("time");
}

// �^�Ǫ��A�w�]�߸��ɶ�
int query_default_condition_heart_beat()
{
	return fetch_variable("heartbeat");
}

void start_effect(object ob)
{
	msg("$ME�i�J�u"+query_condition_name()+"�v��"+query_condition_type()+"���A�C\n", ob, 0, 1);
}

// �������A�ɪ��ĪG
void stop_effect(object ob)
{
	msg("$ME�����u"+query_condition_name()+"�v��"+query_condition_type()+"���A�C\n", ob, 0, 1);
}

// ���A�i�椤���ĪG
void heart_beat_effect(object ob)
{

}
