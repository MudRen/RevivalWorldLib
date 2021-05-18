/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : xmas_antlers.c
 * Author : Msr@RevivalWorld
 * Date   : 2009-12-03
 * Note   : ¶Àª÷³À¯ñ
 * Update :
 *  o 2000-00-00
 *    2009-12-21 ¥[¤J bmw ·Qªº¤@¨Ç©Û¦¡
 *    2009-12-24 ­×¥¿ bug / ½Õ¾ã³¡¥÷¼Æ­È by Clode
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
    set_idname("gold antlers", HIY"¶Àª÷"NOR YEL"³À"NOR HIY"¯ñ"NOR);
    set_temp("status", HIY"2009"NOR HIC" «i"NOR CYN"¤h" NOR HIC"ª©");

    if( this_object()->set_shadow_ob() ) return;

    set("long", "¤@¹ï¦b­«¥Í¥@¬É¤¤©Ò¦V©ÜÃûªº¶W¯ÅªZ¾¹");
    set("unit", "¹ï");
    set("mass", 0);
    set("value", 10);
    set("flag/no_amount", 1);
    set("skill", "stick");

    set(BUFF_STR, 1);
    set(ATTACK, 30);
    set(BUFF_SLIP_CHANCE, 10);

    ::setup_equipment(EQ_TWOHAND, HIY"ÀY"NOR YEL"³»"NOR);
}

string *attack =
({
  "$ME­I«á¥X²{¡u[1;35m³Á[0;35m¥i[1;35m³Ç[0;35m§J[1;35m·Æ[m¡vªº¨­¼v¡A³»µÛ%s¡A¨q°_¤F©ÛµP¡u[37mªÅ¤¤·Æ¨B[m¡v¡AÂ¶µÛ$YOU¤£°±¦a·Æ¨Ó·Æ¥h¡I\n",
  "$ME·n¯ñ®Ì¸£ªº¥Ï°Ê%s¡A¤j³ÛµÛ¡G¡u¸U¯àªº³À¯«¡A½Ð½ç¤©§Ú¯«©_ªº¤O¶q¡I¡vÀþ¶¡[0;36m¥¢[m[1;36m±±¼É[m[0;36m½Ä[m¦a¦V$YOU·Æ¤F¹L¥h¡I\n",
  "$MEÀY³»µÛ%s¡Aµo´§¡u[1;31m´c[m[0;31m³À°«[m[1;31m«iªÌ[0m¡vªººë¯«¡A©¹$YOU [1;37mœòµ£[0m ¤F¹L¥h¡I\n",
  "$MEºò´¤%s¡A¥s¤F¤@Án¡G¦nÎx¡IÀH§YÅÜ¨­¦¨¡u[1;30mºQÁy[mÅ][1;30m³À[m¡v¡A¹ï$YOU¨Ï¥X¡u[1;32mºQ[m[0;32mÁy§ð[m[1;32mÀ»[m¡v¡I\n"
});

//
// ªZ¾¹¦Û°Ê¯S®í§ðÀ»
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
