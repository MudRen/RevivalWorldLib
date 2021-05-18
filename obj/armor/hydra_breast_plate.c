/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra_breast_plate.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 海德拉胸甲
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
	set_idname(HIREV HIW"hydra"WHT" breast plate"NOR, HIREV HIW"海德拉"WHT"胸甲"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從九頭蛇海德拉上掉落的胸甲。\n");
	set("unit", "副");
	set("mass", 1000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 4,
		"/obj/gem/gem1" : 4,
		"/obj/gem/gem2" : 4,
		"/obj/gem/gem3" : 4,
		"/obj/gem/gem4" : 4,
		"/obj/gem/gem5" : 4,
		"/obj/gem/gem6" : 4,
	]));

	set(DEFEND, 12);
	set(BUFF_PHY, 2);
	set(BUFF_INT, 2);
	set(BUFF_STR, 2);

	set(BUFF_FULLSUIT+BUFF_STAMINA_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_BREASTPLATE, HIY"胸"NOR YEL"甲"NOR);
}

