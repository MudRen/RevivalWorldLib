/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : king_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 帝王石
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
	set_idname(HIC"king "NOR CYN"stone"NOR, HIC"帝"NOR CYN"王"HIC"石"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "這青灰色的厚實長方石塊，周圍雕刻古代圖案，隱約中不斷地散發出一種令人臣服\n的威嚴。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 100000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 65);
	set(BUFF_ENERGY_REGEN, 10);
	set("buff/status", HIC"帝"NOR CYN"王"HIC"石"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
