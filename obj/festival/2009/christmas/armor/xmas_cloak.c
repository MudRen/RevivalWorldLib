/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_cloak.c
 * Author : Msr@RevivalWorld
 * Date   : 2009-12-07
 * Note   : �C�ϩܭ�
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
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
    set_idname("christmas cloak", HIR"�C"NOR RED"��"NOR HIW"�ܭ�"NOR);
    set_temp("status", HIY"2009"NOR HIC" �i"NOR CYN"�h" NOR HIC"��");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "���ͪ��@�� 2009 �C�Ϭ����~�C\n");
    set("unit", "��");
    set("mass", 500);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 5);
    set(BUFF_STR, 1);
    set(BUFF_AGI, 2);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_CLOAK, HIY"��"NOR YEL"��"NOR);
}

