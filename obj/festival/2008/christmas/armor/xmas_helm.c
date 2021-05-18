/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_helm.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : 聖誕老人帽
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
    set_idname("christmas helm", HIR"耶"NOR RED"誕"NOR HIW"老人帽"NOR);
    set_temp("status", HIY"2008"NOR HIC" 紀"NOR CYN"念" NOR HIC"版");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "重生的世界 2008 耶誕紀念品。\n");
    set("unit", "頂");
    set("mass", 500);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 5);
    set(BUFF_STR, 1);
    set(BUFF_AGI, 2);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_HEAD, HIY"頭"NOR YEL"盔"NOR);
}

