/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fly_horse.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 飛馬
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

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("fly horse", HIW"飛"NOR WHT"馬"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一匹可以用來載重物品的動物。\n");
	set("unit", "匹");
	set("mass", 0);
	set("value", 6000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 30);
	set(BUFF_STAMINA_REGEN, 1);
	set("buff/status", HIW"飛"NOR WHT"馬"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
