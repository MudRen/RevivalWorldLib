/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : train_ticket.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-18
 * Note   : 火車車票
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("train ticket", "火車車票");
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "　一張可以用來前往世界各地的火車票。");
	set("unit", "張");
	set("mass", 10);
	set("value", 100000);
	set("badsell", 1);
}
