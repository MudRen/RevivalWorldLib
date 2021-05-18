/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_ancient_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 禪．遠古鞋子
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
	set_idname(HIW"zen "NOR HIM"ancient"NOR MAG" boots"NOR, HIW"禪"NOR WHT"．"NOR HIM"遠古"NOR MAG"鞋子"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從被遺忘的遠古武士上掉落的史詩鞋子。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 12 ]));

	set(DEFEND, 15);
	set(BUFF_STR, 3);
	set(BUFF_PHY, 3);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 60);
	set(BUFF_FULLSUIT+BUFF_INT, 3);

	::setup_equipment(EQ_BOOTS, HIY"鞋"NOR YEL"子"NOR);
}

