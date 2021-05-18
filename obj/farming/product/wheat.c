/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wheat.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : 小麥
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("wheat","小麥");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","可以用來製造麵粉。\n");
        set("unit", "綑");
        set("mass", 200);
        set("value", 400);
}
