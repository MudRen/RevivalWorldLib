/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cake_glass.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-10-07
 * Note   : 中秋節禮物 - 月餅太陽眼鏡
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
	set_idname("cake glass", HIR"月餅"NOR+RED"太陽"HIW"眼鏡"NOR);
	set_temp("status", HIB"20"NOR BLU"06 "HIB"中"NOR BLU"秋");

	if( this_object()->set_shadow_database() ) return;

	set("long", "重生的世界中秋節的禮物。\n");
	
	set("unit", "個");
	set("mass", 0);
	set("value", 10);
	set("badsell", 1);
	set("flag/no_amount", 1);

	set(BUFF_FOOD_MAX, 10);
	set(BUFF_DRINK_MAX, 10);

	::setup_equipment(EQ_ETC, HIG"頭"NOR GRN"頂"NOR);
}
