/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : intelligence_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 大智石
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
	set_idname(HIB"intelligence "NOR BLU"stone"NOR, HIB"大智"NOR BLU"石"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "在這湛藍如深海的石頭中，飄浮流動著幽暗的粒子，反覆發出微弱的光芒後又熄滅\n，彷彿要訴說些什麼。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 4000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 24);
	set("buff/status", HIB"大智"NOR BLU"石"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
