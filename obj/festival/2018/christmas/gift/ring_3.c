/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2018-12-08
 * Note   : 竧较з
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
    set_idname("christmas ring", HIR"瑿"NOR RED"较"NOR HIW"з"NOR);
    set_temp("status", HIY"2018"NOR HIW" 龙瓣缝" NOR HIC"");

    if( this_object()->set_shadow_database() ) return;

    set("long", "2018 RW 竧较├з\n");

    set("unit", "");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);
    /*    
#define BUFF_STAMINA_MAX                "buff/abi/stamina/max"          // 砰程
#define BUFF_HEALTH_MAX                 "buff/abi/health/max"           // ネ㏑程
#define BUFF_ENERGY_MAX                 "buff/abi/energy/max"           // 弘程
    */
    set(BUFF_ENERGY_MAX, 3); // 弘程

    ::setup_equipment(EQ_FINGER, HIG"も"NOR GRN""NOR);
}
