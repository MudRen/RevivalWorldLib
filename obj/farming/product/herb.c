/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : herb.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : �į�
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("herb","�į�");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�i�H�Ω�s�ġC\n");
        set("unit", "��");
        set("mass", 200);
        set("value", 500);
}
