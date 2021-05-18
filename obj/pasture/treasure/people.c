/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : people.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 人道
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
	set_idname(HIG"peo"NOR GRN"pl"HIG"e"NOR, HIG"人"NOR GRN"．"HIG"道"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "立人之道曰仁與義。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 145);
	set(BUFF_STAMINA_REGEN, 40);
	set(BUFF_COLLECT_ABILITY, 2);
	
	set("buff/status", HIG"人"NOR GRN"．"HIG"道"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
