/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tsouni_horse.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-05-04
 * Note   : 草泥馬
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
	set_idname(HIW"tsouni "NOR WHT"horse"NOR, HIW"草泥"NOR WHT"馬"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一種極為缺德的近代神話座騎。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 500000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 100);
	set(BUFF_STAMINA_REGEN, 20);
	set("buff/status", HIW"草泥"NOR WHT"馬"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
