/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moonrabbit_gloves.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-27
 * Note   : 月兔手套
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
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("moonrabbit gloves", HIC"月兔"NOR+CYN"兔掌"HIW"手套"NOR);
	set_temp("status", HIB"中"NOR BLU"秋");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "重生的世界中秋節特殊活動的手套。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);

	set(DEFEND, 5);
	set(BUFF_STR, 1);
	set(BUFF_AGI, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_GLOVES, HIY"手"NOR YEL"套"NOR);
}

