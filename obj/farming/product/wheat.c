/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wheat.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : �p��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <inherit.h>

inherit STANDARD_OBJECT;

void create()
{
        set_idname("wheat","�p��");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long","�i�H�Ψӻs�y�ѯ��C\n");
        set("unit", "�i");
        set("mass", 200);
        set("value", 400);
}
