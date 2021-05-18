/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_antlers.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : 黃金鹿茸
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
    set_idname("gold antlers", HIY"黃金鹿茸"NOR);
    set_temp("status", HIY"2008"NOR HIC" 隱"NOR CYN"藏" NOR HIC"版");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "一對在重生世界中所向披靡的超級武器");
    set("unit", "對");
    set("mass", 0);
    set("value", 100000000);
    set("flag/no_amount", 1);
    set("skill", "stick");

    set(ATTACK, 5);

    ::setup_equipment(EQ_HAND, HIY"頭"NOR YEL"頂"NOR);
}

//
// 武器自動特殊攻擊
//
int special_attack(object attacker, object defender)
{
    if( !random(50) )
    {
        int damage = range_random(50, 100);

        msg("$ME手中的"+this_object()->query_idname()+"召喚出神獸，神獸滑行對著$YOU撞了過去！\n", attacker, defender, 1);

        return damage;
    }

    return 0;
}
