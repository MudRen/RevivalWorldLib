/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : true_kylin.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 真麒麟
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
	set_idname("true kylin", HIW"真"HIG"麒"NOR GRN"麟"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一匹可以用來載重物品的動物。\n");
	set("unit", "隻");
	set("mass", 0);
	set("value", 8000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 42);
	set(BUFF_STAMINA_REGEN, 3);
	set("buff/status", HIW"真"HIG"麒"NOR GRN"麟"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
