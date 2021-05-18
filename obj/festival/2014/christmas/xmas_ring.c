/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2014-12-8
 * Note   : 聖誕戒指
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
    set_idname("christmas ring", HIR"耶"NOR RED"誕"NOR HIW"戒指"NOR);
    set_temp("status", HIY"2014"NOR HIC" 領"NOR CYN"導" NOR HIC"版");

    if( this_object()->set_shadow_database() ) return;

    set("long", "2014 RW 聖誕紀念戒指, 有加強領導能力的功能。\n");

    set("unit", "指");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);

    set(BUFF_LEAD_ABILITY, 5);

    ::setup_equipment(EQ_FINGER, HIG"小"NOR GRN"指"NOR);
}
