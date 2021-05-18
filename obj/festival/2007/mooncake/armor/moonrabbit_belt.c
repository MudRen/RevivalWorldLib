/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moonrabbit_belt.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-26
 * Note   : 月兔腰帶
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
	set_idname("moonrabbit belt", HIC"月兔"NOR+CYN"尾巴"HIW"腰帶"NOR);
	set_temp("status", HIB"中"NOR BLU"秋");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "重生的世界中秋節特殊活動的腰帶。\n");
	set("unit", "條");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);

	set(DEFEND, 5);
	set(BUFF_STR, 1);
	set(BUFF_AGI, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_BELT, HIY"腰"NOR YEL"帶"NOR);
}

