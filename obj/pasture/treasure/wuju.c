/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wuju.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : ����L��
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
	set_idname(HIW"wu"NOR WHT"ju"NOR, HIW"����"NOR WHT"�L��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "���ĩ�k�A���L�Ҧ�C\n");
	set("unit", "�Y");
	set("mass", 0);
	set("value", 1000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 115);
	set(BUFF_STAMINA_REGEN, 30);
	set("buff/status", HIW"����"NOR WHT"�L��"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
