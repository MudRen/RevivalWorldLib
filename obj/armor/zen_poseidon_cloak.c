/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_poseidon_cloak.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-11-10
 * Note   : 禪．波塞冬披風
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
	set_idname(HIW"zen "NOR HIC"po"HIR"sei"HIG"don"HIY" cloak", HIW"禪"NOR WHT"．"NOR HIC"波"HIR"塞"HIG"冬"HIY"披風");
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "波塞冬的傳奇披風，擁有不凡的力量。\n");
	set("unit", "件");
	set("mass", 100);
	set("value", 100000);
	set("flag/no_amount", 1);
	set("special", 1);

	set(ATTACK, 75);
	set(BUFF_STR, 75);
	
	::setup_equipment(EQ_CLOAK, HIY"傳奇"NOR YEL"披風"NOR);
}
