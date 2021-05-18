/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hydra_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-20
 * Note   : 海德拉護腕
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
	set_idname(HIREV HIW"hydra"WHT" bracers"NOR, HIREV HIW"海德拉"WHT"護腕"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從九頭蛇海德拉上掉落的護腕。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 4 ]));

	set(DEFEND, 5);
	set(BUFF_PHY, 1);
	set(BUFF_INT, 1);
	set(BUFF_STR, 1);

	set(BUFF_FULLSUIT+BUFF_STAMINA_MAX, 20);
	set(BUFF_FULLSUIT+BUFF_STR, 1);

	::setup_equipment(EQ_BRACERS, HIY"護"NOR YEL"腕"NOR);
}

