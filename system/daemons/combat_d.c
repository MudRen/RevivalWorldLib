/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : combat_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-26
 * Note   : �԰����F
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

inherit __DIR__"combat_d_main.c";
		
private void create()
{
	if( clonep() )
		destruct(this_object());
	
	::create();
}

int remove()
{
	::remove();
}

string query_name()
{
	return "�԰��t��(COMBAT_D)";
}

