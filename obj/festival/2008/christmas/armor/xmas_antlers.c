/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_antlers.c
 * Author : Msr@RevivalWorld
 * Date   : 2008-11-04
 * Note   : ��������
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
    set_idname("gold antlers", HIY"��������"NOR);
    set_temp("status", HIY"2008"NOR HIC" ��"NOR CYN"��" NOR HIC"��");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "�@��b���ͥ@�ɤ��ҦV�������W�ŪZ��");
    set("unit", "��");
    set("mass", 0);
    set("value", 100000000);
    set("flag/no_amount", 1);
    set("skill", "stick");

    set(ATTACK, 5);

    ::setup_equipment(EQ_HAND, HIY"�Y"NOR YEL"��"NOR);
}

//
// �Z���۰ʯS�����
//
int special_attack(object attacker, object defender)
{
    if( !random(50) )
    {
        int damage = range_random(50, 100);

        msg("$ME�⤤��"+this_object()->query_idname()+"�l��X���~�A���~�Ʀ���$YOU���F�L�h�I\n", attacker, defender, 1);

        return damage;
    }

    return 0;
}
