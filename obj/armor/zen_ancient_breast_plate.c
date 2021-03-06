/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_ancient_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : �I�D���j�ݥ�
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
	set_idname(HIW"zen "NOR HIM"ancient"NOR MAG"breast plate"NOR, HIW"�I"NOR WHT"�D"NOR HIM"���j"NOR MAG"�ݥ�"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�Q��Ѫ����j�Z�h�W�������v�֯ݥҡC\n");
	set("unit", "��");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 12,
		"/obj/gem/gem1" : 12,
		"/obj/gem/gem2" : 12,
		"/obj/gem/gem3" : 12,
		"/obj/gem/gem4" : 12,
		"/obj/gem/gem5" : 12,
		"/obj/gem/gem6" : 12,
	]));

	set(DEFEND, 36);
	set(BUFF_STR, 6);
	set(BUFF_PHY, 6);
	set(BUFF_AGI, 6);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_INT, 3);

	::setup_equipment(EQ_BREASTPLATE, HIY"��"NOR YEL"��"NOR);
}

