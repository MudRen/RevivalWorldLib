/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : flatbed_tricycle.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-08
 * Note   : 平板車
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
	set_idname("flatbed tricycle", "平板車");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一輛適合用來載重的木造平板車。\n");

	set("unit", "台");
	set("mass", 8000);
	set("value", 50000);
	set("flag/no_amount", 1);
	set("badsell", 1);

	set(BUFF_LOADING_MAX, 2000);
	set(BUFF_SLOT_MAX, 10);
	set(BUFF_MOVING_STAMINA, -50);
	
	::setup_equipment(EQ_VEHICLE, HIC"拖"NOR CYN"車"NOR);
}
