/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : terra.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �a�D
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

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIY"te"NOR YEL"rr"HIY"a", HIY"�a"NOR YEL"�D"HIY"�D"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�ߦa���D��X�P��C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 145);
	set(BUFF_ENERGY_REGEN, 40);
	set(BUFF_SALE_ABILITY, 2);

	set("buff/status", HIY"�a"NOR YEL"�D"HIY"�D"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
