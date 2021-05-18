/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_ring.c
 * Author : Msr@RevivalWorld
 * Date   : 2018-12-08
 * Note   : �t�ϧ٫�
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
    set_idname("christmas ring", HIR"�C"NOR RED"��"NOR HIW"�٫�"NOR);
    set_temp("status", HIY"2018"NOR HIW" �����" NOR HIC"��");

    if( this_object()->set_shadow_database() ) return;

    set("long", "2018 RW �t�Ϭ����٫��C\n");

    set("unit", "��");
    set("mass", 100);
    set("value", 10);
    set("badsell", 1);
    set("flag/no_amount", 1);
    /*    
#define BUFF_STAMINA_MAX                "buff/abi/stamina/max"          // ��O�̤j��
#define BUFF_HEALTH_MAX                 "buff/abi/health/max"           // �ͩR�̤j��
#define BUFF_ENERGY_MAX                 "buff/abi/energy/max"           // �믫�̤j��
    */
    set(BUFF_ENERGY_MAX, 3); // �믫�̤j��

    ::setup_equipment(EQ_FINGER, HIG"��"NOR GRN"��"NOR);
}
