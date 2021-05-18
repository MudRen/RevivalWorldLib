/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_bracers.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : 瑿较臔得
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
    set_idname("christmas bracer", HIR"瑿"NOR RED"较"NOR HIW"臔得"NOR);
    set_temp("status", HIY"2008"NOR HIC" "NOR CYN"├" NOR HIC"");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "ネ 2008 瑿较├珇\n");
    set("unit", "捌");
    set("mass", 500);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 5);
    set(BUFF_STR, 1);
    set(BUFF_AGI, 2);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_BRACERS, HIY"臔"NOR YEL"得"NOR);
}

