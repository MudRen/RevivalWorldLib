/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_thibetanus_gloves.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �I�D�º���M
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
	set_idname(HIW"zen "NOR GRN"thibetanus"HIG" gloves"NOR, HIW"�I"NOR WHT"�D"NOR GRN"�º�"HIG"��M"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�O�W�º��W�������v�֤�M�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem2" : 6 ]));

	set(DEFEND, 6);
	set(BUFF_STR, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 3);

	::setup_equipment(EQ_GLOVES, HIY"��"NOR YEL"�M"NOR);
}

