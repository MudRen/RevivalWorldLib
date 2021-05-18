/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rice_god.c
 * Author : Clode@RevivalWorld
 * Date   : 2013-03-04
 * Note   : 稻荷神
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
	set_idname(HIG"r"NOR GRN"i"HIG"c"NOR GRN"e "HIG"g"NOR GRN"o"HIG"d", HIG"稻"NOR GRN"荷"HIG"神"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "主管穀物與豐收的神，據說有兩個白狐隨從。\n");
	set("unit", "株");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 180);
	set(BUFF_SOCIAL_EXP_BONUS, 115);
	set(BUFF_COMBAT_EXP_BONUS, 115);
	set(BUFF_GROW_ABILITY, 3);
	
	set("buff/status", HIG"稻"NOR GRN"荷"HIG"神"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"胸"NOR GRN"針"NOR);
}
