/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cotton.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-24
 * Note   : �֪�
 * Update :
 *  o 2000-05-30
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("cotton", "�֪�");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "�i�Ψӻs�@��A�����~ �C\n");
        set("unit", "��");
        set("mass", 200);
        set("value", 900);
}
