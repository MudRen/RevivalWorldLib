/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_gigas_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �I�D�����c�l
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
	set_idname(HIW"zen "NOR HIR"gigas "NOR RED"boots"NOR, HIW"�I"NOR WHT"�D"NOR HIR"����"NOR RED"�c�l"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�����t�W�������v�־c�l�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 9 ]));

	set(DEFEND, 9);
	set(BUFF_PHY, 6);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 30);
	set(BUFF_FULLSUIT+BUFF_PHY, 3);

	::setup_equipment(EQ_BOOTS, HIY"�c"NOR YEL"�l"NOR);
}

