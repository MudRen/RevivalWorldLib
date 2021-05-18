/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : people.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �H�D
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
	set_idname(HIG"peo"NOR GRN"pl"HIG"e"NOR, HIG"�H"NOR GRN"�D"HIG"�D"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�ߤH���D�꤯�P�q�C\n");
	set("unit", "�Y");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 145);
	set(BUFF_STAMINA_REGEN, 40);
	set(BUFF_COLLECT_ABILITY, 2);
	
	set("buff/status", HIG"�H"NOR GRN"�D"HIG"�D"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
