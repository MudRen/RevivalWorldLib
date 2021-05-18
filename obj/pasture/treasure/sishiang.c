/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sishiang.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 四象
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
	set_idname(HIC"si"NOR CYN"shiang"NOR, HIC"四"NOR CYN"象"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "少陰、少陽、太陽、太陰。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 130);
	set(BUFF_STAMINA_REGEN, 35);
	set("buff/status", HIC"四"NOR CYN"象"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
