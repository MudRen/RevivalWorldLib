/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wuju.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : 妙行無住
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
	set_idname(HIW"wu"NOR WHT"ju"NOR, HIW"妙行"NOR WHT"無住"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "菩薩於法，應無所住。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 1000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 115);
	set(BUFF_STAMINA_REGEN, 30);
	set("buff/status", HIW"妙行"NOR WHT"無住"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
