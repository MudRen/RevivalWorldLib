/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-12-25
 * Note   : ¸t½Ï§Ù«ü
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
    set_idname("christmas ring", HIR"­C"NOR RED"½Ï"NOR HIW"§Ù«ü"NOR);
    set_temp("status", HIY"2008"NOR HIC" ¹w"NOR CYN"°â" NOR HIC"ª©");

    if( this_object()->set_shadow_database() ) return;

    set("long", "2008 RW ¸t½Ï§Ù«ü¡C\n");

    set("unit", "«ü");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);
    set(BUFF_STAMINA_REGEN, 5);

    ::setup_equipment(EQ_FINGER, HIG"¸}"NOR GRN"«ü"NOR);
}
