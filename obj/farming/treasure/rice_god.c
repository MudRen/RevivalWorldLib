/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice_god.c
 * Author : Clode@RevivalWorld
 * Date   : 2013-03-04
 * Note   : �_����
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
	set_idname(HIG"r"NOR GRN"i"HIG"c"NOR GRN"e "HIG"g"NOR GRN"o"HIG"d", HIG"�_"NOR GRN"��"HIG"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�D�޽\���P�צ������A�ڻ�����ӥժ��H�q�C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 180);
	set(BUFF_SOCIAL_EXP_BONUS, 115);
	set(BUFF_COMBAT_EXP_BONUS, 115);
	set(BUFF_GROW_ABILITY, 3);
	
	set("buff/status", HIG"�_"NOR GRN"��"HIG"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
