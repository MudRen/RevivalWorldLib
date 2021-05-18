/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_caerulea_helm.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : 禪．藍鵲頭盔
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
	set_idname(HIW"zen "NOR HIB"caerulea"NOR BLU" helm"NOR, HIW"禪"NOR WHT"．"NOR HIB"藍鵲"NOR BLU"頭盔"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一副從臺灣藍鵲上掉落的史詩頭盔。\n");
	set("unit", "副");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem1" : 9 ]));

	set(DEFEND, 9);
	set(BUFF_AGI, 6);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 30);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_HEAD, HIY"頭"NOR YEL"盔"NOR);
}

