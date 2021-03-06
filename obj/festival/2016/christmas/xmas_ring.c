/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2016-12-14
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
    set_temp("status", HIY"2016"NOR HIC" 體"NOR CYN"力" NOR HIC"版");

    if( this_object()->set_shadow_database() ) return;
    set("long", "2016 RW 聖誕紀念戒指, 有增加最大體力的功能。\n");

    set("unit", "指");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);

    set(BUFF_STAMINA_MAX, 5);

    ::setup_equipment(EQ_FINGER, HIG"小"NOR GRN"指"NOR);
}
