/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_cloth.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : �C�Ϥj��
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
    set_idname("christmas cloth", HIR"�C"NOR RED"��"NOR HIW"�j��"NOR);
    set_temp("status", HIY"2008"NOR HIC" ��"NOR CYN"��" NOR HIC"��");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "���ͪ��@�� 2008 �C�Ϭ����~�C\n");
    set("unit", "��");
    set("mass", 1000);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 15);
    set(BUFF_STR, 3);
    set(BUFF_AGI, 3);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_BREASTPLATE, HIY"��"NOR YEL"��"NOR);
}

