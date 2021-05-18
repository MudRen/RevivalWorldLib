/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : huge_lord.c
 * Author : Clode@RevivalWorld
 * Date   : 2013-03-04
 * Note   : 大物主
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
	set_idname(HIC"h"NOR CYN"u"HIC"g"NOR CYN"e "HIC"l"NOR CYN"o"HIC"r"NOR CYN"d", HIC"大"NOR CYN"物"HIC"主"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "別名為三輪明神。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 180);
	set(BUFF_STAMINA_REGEN, 50);
	set(BUFF_COLLECT_ABILITY, 3);
	
	set("buff/status", HIC"大"NOR CYN"物"HIC"主"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
