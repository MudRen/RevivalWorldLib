/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nessiteras_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : �����c�l
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
	set_idname(HIB"nessiteras"NOR MAG" boots"NOR, HIB"����"NOR MAG"�c�l"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq��������ǤW�������c�l�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 4 ]));

	set(DEFEND, 5);
	set(BUFF_STR, 1);
	set(BUFF_INT, 1);
	set(BUFF_AGI, 1);

	set(BUFF_FULLSUIT+BUFF_ENERGY_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_BOOTS, HIY"�c"NOR YEL"�l"NOR);
}
