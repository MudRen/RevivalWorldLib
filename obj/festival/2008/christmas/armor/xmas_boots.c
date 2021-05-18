/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_boots.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : ­C½Ï¾c
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
    set_idname("christmas boots", HIR"­C"NOR RED"½Ï"NOR HIW"¨¾·Æ¾c"NOR);
    set_temp("status", HIY"2008"NOR HIC" ¬ö"NOR CYN"©À" NOR HIC"ª©");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "­«¥Íªº¥@¬É 2008 ­C½Ï¬ö©À«~¡C\n");
    set("unit", "Âù");
    set("mass", 500);
    set("value", 10000);
    set("flag/no_amount", 1);

    set(DEFEND, 5);
    set(BUFF_STR, 1);
    set(BUFF_AGI, 2);

    set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
    set(BUFF_FULLSUIT+BUFF_AGI, 1);

    ::setup_equipment(EQ_BOOTS, HIY"¾c"NOR YEL"¤l"NOR);
}

