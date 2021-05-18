/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �ʻs�ݥ�
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
	set_idname("coarse breast plate", "�ʻs�ݥ�");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ƶ}�����ʻs�ݥҡC\n");
	set("unit", "��");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 1,
		"/obj/gem/gem1" : 1,
		"/obj/gem/gem2" : 1,
		"/obj/gem/gem3" : 1,
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
		"/obj/gem/gem6" : 1,
	]));

	set(DEFEND, 2);
	set(BUFF_PHY, 2);
	
	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 30);

	::setup_equipment(EQ_BREASTPLATE, HIY"��"NOR YEL"��"NOR);
}

