/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moonrabbit_gloves.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : 耶誕手套
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
    set_idname("christmas gloves", HIR"耶"NOR RED"誕"NOR HIW"手套"NOR);
    set_temp("status", HIY"2008"NOR HIC" 紀"NOR CYN"念" NOR HIC"版");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "重生的世界 2008 耶誕紀念品。\n");
    set("unit", "雙");
    set("mass", 500);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 5);
    set(BUFF_STR, 1);
    set(BUFF_AGI, 2);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_GLOVES, HIY"手"NOR YEL"套"NOR);
}

