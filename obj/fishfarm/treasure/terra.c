/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : terra.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 地道
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
	set_idname(HIY"te"NOR YEL"rr"HIY"a", HIY"地"NOR YEL"．"HIY"道"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "立地之道曰柔與剛。\n");
	set("unit", "顆");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 145);
	set(BUFF_ENERGY_REGEN, 40);
	set(BUFF_SALE_ABILITY, 2);

	set("buff/status", HIY"地"NOR YEL"．"HIY"道"NOR);
	
	::setup_equipment(EQ_EYE, HIG"眼"NOR GRN"睛"NOR);
}
