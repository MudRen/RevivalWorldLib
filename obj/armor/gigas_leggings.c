/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gigas_leggings.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �����L��
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
	set_idname(HIR"gigas "NOR RED"leggings"NOR, HIR"����"NOR RED"�L��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�����t�W�������L�ҡC\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem0" : 3 ]));

	set(DEFEND, 4);
	set(BUFF_PHY, 3);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_PHY, 1);

	::setup_equipment(EQ_LEGGINGS, HIY"�L"NOR YEL"��"NOR);
}

