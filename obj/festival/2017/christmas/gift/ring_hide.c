/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2017-12-12
 * Note   : �t�ϧ٫�
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
    set_idname("christmas ring", HIR"�C"NOR RED"��"NOR HIW"�٫�"NOR);
    set_temp("status", HIY"2017"NOR HIC" ����" NOR HIC"��");

    if( this_object()->set_shadow_database() ) return;

    set("long", "2017 RW �t�Ϭ����٫����ê��C\n");

    set("unit", "��");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);

    set(BUFF_STR, 2);
    set(BUFF_PHY, 2);
    set(BUFF_INT, 2);
    set(BUFF_AGI, 2);
    set(BUFF_CHA, 2);
    set(BUFF_STAMINA_MAX, 1);

    ::setup_equipment(EQ_FINGER, HIG"��"NOR GRN"��"NOR);
}
