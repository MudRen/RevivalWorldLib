/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bamboo_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 竹韻石
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
	set_idname(HIG"bamboo "NOR GRN"stone"NOR, HIG"竹韻"NOR GRN"石"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一顆墨綠色的石頭，石頭中央深處似乎隱約有光流動著，而隨著流光石頭內也傳出\n了細微的清脆聲響。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 8000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 36);
	set(BUFF_ENERGY_REGEN, 3);
	set("buff/status", HIG"竹韻"NOR GRN"石"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
