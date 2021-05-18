/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : train_ticket.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : ��������
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("train ticket", "��������");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�@�i�i�H�Ψӫe���@�ɦU�a���������C");
	set("unit", "�i");
	set("mass", 10);
	set("value", 100000);
	set("badsell", 1);
}
