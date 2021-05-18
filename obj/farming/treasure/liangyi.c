/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : liangyi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 兩儀
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
	set_idname(HIY"liang"NOR YEL"yi"NOR, HIY"兩"NOR YEL"儀"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "少陰太陽、少陽太陰。\n");
	set("unit", "株");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 130);
	set(BUFF_SOCIAL_EXP_BONUS, 90);
	set(BUFF_COMBAT_EXP_BONUS, 90);
	set("buff/status", HIY"兩"NOR YEL"儀"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"胸"NOR GRN"針"NOR);
}
