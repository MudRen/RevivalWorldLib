/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chi_hydra_cloak.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-10
 * Note   : 極．王者披風
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
	set_idname(HIW"chi "NOR HIY"lord"NOR YEL" cloak"NOR, HIW"極"NOR WHT"．"NOR HIY"王"NOR YEL"者"HIY"披"NOR YEL"風"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副古代文字中記載的史詩披風。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem3" : 15 ]));

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
	set(BUFF_FULLSUIT+BUFF_COUNTERATTACK_CHANCE, 1);
	
	::setup_equipment(EQ_CLOAK, HIY"披"NOR YEL"風"NOR);
}

