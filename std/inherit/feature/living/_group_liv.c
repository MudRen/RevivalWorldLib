/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _group_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-01-11
 * Note   : �ն��\��
 * Update :
 *  o 2003-00-00 
 -----------------------------------------
 */

#include <group.h>

private nosave object group_leader;
private nosave string group_name;
private nosave mapping group = allocate_mapping(0);

//-----------------------------
// �M�z������
//-----------------------------
void rehash_group_data()
{
	map_delete(group, 0);
}

//-----------------------------
// �[�J����
//-----------------------------
int add_group_member(object ob)
{
	rehash_group_data();

	ob->set_group_leader(this_object());
	
	if( userp(ob) )
		group[ob] = GROUP_PLAYER;
	else
		group[ob] = GROUP_LABOR;
}

//-----------------------------
// �إ߷s����
//-----------------------------
void create_new_group(string name)
{
	rehash_group_data();

	group_name = name;

	group[this_object()] = GROUP_LEADER;
	
	group_leader = this_object();
}

//-----------------------------
// �q�������
//-----------------------------
int remove_group_member(object ob)
{
	rehash_group_data();

	map_delete(group, ob);

	ob->clear_group_data();
}

//-----------------------------
// �]�w����
//-----------------------------
void set_group_leader(object leader)
{
	group_leader = leader;
}

//-----------------------------
// �]�w������
//-----------------------------
void become_group_leader(object original_leader)
{
	group = copy(original_leader->query_group_data());
	group_name = copy(original_leader->query_group_name());
	
	group[this_object()] = GROUP_LEADER;
	group[original_leader] = GROUP_PLAYER;

	original_leader->clear_group_data();

	group_leader = this_object();
	original_leader->set_group_leader(this_object());
}

//-----------------------------
// ������������
//-----------------------------
void clear_group_data()
{
	group_leader = 0;
	group_name = 0;
	group = allocate_mapping(0);
}


//-----------------------------
// ���o��������
//-----------------------------
object query_group_leader()
{
	return group_leader;
}

//-----------------------------
// ���o����W��
//-----------------------------
string query_group_name()
{
	if( !objectp(group_leader) )
		return 0;

	if( group_leader == this_object() )
		return group_name;
	else
		return group_leader->query_group_name();
}

//-----------------------------
// �]�w����W��
//-----------------------------
void set_group_name(string name)
{
	group_name = name;
}

//-----------------------------
// �d�߬Y�H�O�_�������
//-----------------------------
int is_in_my_group(object member)
{
	if( member == this_object() )
		return 1;

	if( !objectp(group_leader) )
		return 0;
	
	if( group_leader == this_object() )
	{
		rehash_group_data();
		return !undefinedp(group[member]);
	}
	else
		return group_leader->is_in_my_group(member);
}



//-----------------------------
// �d�߬O�_�������
//-----------------------------
int is_group_leader()
{
	return this_object() == group_leader;
}

//-----------------------------
// ���o������
//-----------------------------
mapping query_group_data()
{
	rehash_group_data();

	return copy(group);
}

//-----------------------------
// �]�w������
//-----------------------------
mapping set_group_data(mapping data)
{
	group = copy(data);
}

//-----------------------------
// ���o����W��
//-----------------------------
object *query_group_members()
{
	if( !objectp(group_leader) )
		return 0;

	if( group_leader == this_object() )
	{
		rehash_group_data();
		
		return keys(group);
	}
	else
		return group_leader->query_group_members();
}

//-----------------------------
// �Ѵ���Ӷ���
//-----------------------------
void dismiss_group()
{
	group_name = 0;

	foreach(object member, mixed data in group)
	{
		if( !objectp(member) || member == this_object() )
			continue;
		
		member->clear_group_data();
	}

	clear_group_data();
}

