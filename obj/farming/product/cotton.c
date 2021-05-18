/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cotton.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-24
 * Note   : 棉花
 * Update :
 *  o 2000-05-30
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("cotton", "棉花");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "可用來製作衣服等物品 。\n");
        set("unit", "株");
        set("mass", 200);
        set("value", 900);
}
