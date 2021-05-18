/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : coarse_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �ʻs�c�l
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
	set_idname("coarse boots", "�ʻs�c�l");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ƴ��q���c�l�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 1 ]));

	set(DEFEND, 1);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);

	::setup_equipment(EQ_BOOTS, HIY"�c"NOR YEL"�l"NOR);
}

