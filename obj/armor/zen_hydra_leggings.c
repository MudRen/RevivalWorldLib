/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_hydra_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : �I�D���w�ԻL��
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
	set_idname(HIW"zen "NOR HIREV HIW"hydra"WHT" leggings"NOR, HIW"�I"NOR WHT"�D"NOR HIREV HIW"���w��"WHT"�L��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�E�Y�D���w�ԤW�������v�ֻL�ҡC\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 12 ]));

	set(DEFEND, 18);
	set(BUFF_PHY, 3);
	set(BUFF_INT, 3);
	set(BUFF_STR, 3);

	set(BUFF_FULLSUIT+BUFF_STAMINA_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_STR, 3);

	::setup_equipment(EQ_LEGGINGS, HIY"�L"NOR YEL"��"NOR);
}

