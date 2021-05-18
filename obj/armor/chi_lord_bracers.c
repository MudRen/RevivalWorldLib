/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chi_hydra_bracers.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-10
 * Note   : 伐臔得
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
	set_idname(HIW"chi "NOR HIY"lord"NOR YEL" bracers"NOR, HIW"伐"NOR WHT""NOR HIY""NOR YEL""HIY"臔"NOR YEL"得"NOR);
	set_temp("status", HIW""NOR WHT"钢");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "捌ゅい癘更钢臔得\n");
	set("unit", "捌");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem4" : 15 ]));

	set(DEFEND, 20);
	set(BUFF_STR, 3);
	set(BUFF_PHY, 3);
	set(BUFF_AGI, 3);
	set(BUFF_INT, 3);
	set(BUFF_CHA, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 3);
	set(BUFF_FULLSUIT+BUFF_PHY, 3);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);
	set(BUFF_FULLSUIT+BUFF_INT, 3);
	set(BUFF_FULLSUIT+BUFF_CHA, 3);
	set(BUFF_FULLSUIT+BUFF_ANTIFATAL_CHANCE, 1);
	
	::setup_equipment(EQ_BRACERS, HIY"臔"NOR YEL"得"NOR);
}

