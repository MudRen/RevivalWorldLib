/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_antlers.c
 * Author : Msr@RevivalWorld
 * Date   : 2009-12-03
 * Note   : ��������
 * Update :
 *  o 2000-00-00
 *    2009-12-21 �[�J bmw �Q���@�ǩۦ�
 *    2009-12-24 �ץ� bug / �վ㳡���ƭ� by Clode
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
    set_idname("gold antlers", HIY"����"NOR YEL"��"NOR HIY"��"NOR);
    set_temp("status", HIY"2009"NOR HIC" �i"NOR CYN"�h" NOR HIC"��");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "�@��b���ͥ@�ɤ��ҦV�������W�ŪZ��");
    set("unit", "��");
    set("mass", 0);
    set("value", 10);
    set("flag/no_amount", 1);
    set("skill", "stick");

    set(BUFF_STR, 1);
    set(ATTACK, 30);
    set(BUFF_SLIP_CHANCE, 10);

    ::setup_equipment(EQ_TWOHAND, HIY"�Y"NOR YEL"��"NOR);
}

string *attack =
({
  "$ME�I��X�{�u[1;35m��[0;35m�i[1;35m��[0;35m�J[1;35m��[m�v�����v�A����%s�A�q�_�F�۵P�u[37m�Ť��ƨB[m�v�A¶��$YOU�����a�ƨӷƥh�I\n",
  "$ME�n��̸����ϰ�%s�A�j�۵ۡG�u�U�઺�����A�н礩�گ��_���O�q�I�v����[0;36m��[m[1;36m����[m[0;36m��[m�a�V$YOU�ƤF�L�h�I\n",
  "$ME�Y����%s�A�o���u[1;31m�c[m[0;31m����[m[1;31m�i��[0m�v���믫�A��$YOU [1;37m��[0m �F�L�h�I\n",
  "$ME��%s�A�s�F�@�n�G�n�x�I�H�Y�ܨ����u[1;30m�Q�y[m�][1;30m��[m�v�A��$YOU�ϥX�u[1;32m�Q[m[0;32m�y��[m[1;32m��[m�v�I\n"
});

//
// �Z���۰ʯS�����
//
int special_attack(object attacker, object defender)
{
    if( !random(10) )
    {
        int damage = range_random(50, 100);

        msg( sprintf(attack[random(sizeof(attack))], this_object()->query_idname()), attacker, defender, 1);

        return damage;
    }
    return 0;
}
