/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mydas_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 綠蠵鞋子
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
	set_idname(HIG"mydas "NOR GRN"boots"NOR, HIG"綠蠵"NOR GRN"鞋子"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從綠蠵龜上掉落的鞋子。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 2 ]));

	set(DEFEND, 2);
	set(BUFF_PHY, 1);

	set(BUFF_FULLSUIT+BUFF_PHY, 1);

	::setup_equipment(EQ_BOOTS, HIY"鞋"NOR YEL"子"NOR);
}

