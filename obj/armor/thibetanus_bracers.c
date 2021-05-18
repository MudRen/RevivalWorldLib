/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : thibetanus_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �º��@��
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
	set_idname(GRN"thibetanus"HIG" bracers"NOR, GRN"�º�"HIG"�@��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�O�W�º��W�������@�áC\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 2 ]));

	set(DEFEND, 2);
	set(BUFF_STR, 1);

	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_BRACERS, HIY"�@"NOR YEL"��"NOR);
}

