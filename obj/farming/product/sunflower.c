/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sunflower.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 向日葵
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("sunflower","向日葵");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","向日葵花語：愛慕•花輝•忠誠•尊敬•顯赫。\n");
        set("unit", "朵");
        set("mass", 200);
        set("value", 600);
}
