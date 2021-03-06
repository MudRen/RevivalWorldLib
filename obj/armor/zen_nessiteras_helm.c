/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_nessiteras_helm.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : �I�D�����Y��
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
	set_idname(HIW"zen "NOR HIB"nessiteras"NOR MAG" helm"NOR, HIW"�I"NOR WHT"�D"NOR HIB"����"NOR MAG"�Y��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq��������ǤW�������v���Y���C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem1" : 12 ]));

	set(DEFEND, 15);
	set(BUFF_STR, 3);
	set(BUFF_INT, 3);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_ENERGY_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_HEAD, HIY"�Y"NOR YEL"��"NOR);
}

