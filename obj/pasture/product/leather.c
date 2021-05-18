/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : leather.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : ¥Ö­²
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("leather","¥Ö­²");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "¯ÂºØ³À¥Ö¥Ö­²¡C\n");
	set("unit", "±i");
	set("mass", 100);
	set("value", 8000);
}
