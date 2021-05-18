/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ancient_belt.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : ���j�y�a
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
	set_idname(HIM"ancient"NOR MAG" belt"NOR, HIM"���j"NOR MAG"�y�a"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�Q��Ѫ����j�Z�h�W�������y�a�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem6" : 4 ]));

	set(DEFEND, 5);
	set(BUFF_STR, 1);
	set(BUFF_PHY, 1);
	set(BUFF_AGI, 1);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_INT, 1);

	::setup_equipment(EQ_BELT, HIY"�y"NOR YEL"�a"NOR);
}

