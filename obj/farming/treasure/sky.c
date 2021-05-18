/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sky.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �ѹD
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
	set_idname(HIC"s"NOR CYN"k"HIC"y"NOR, HIC"��"NOR CYN"�D"HIC"�D"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ߤѤ��D�곱�P���C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 145);
	set(BUFF_SOCIAL_EXP_BONUS, 100);
	set(BUFF_COMBAT_EXP_BONUS, 100);
	set(BUFF_GROW_ABILITY, 2);
	
	set("buff/status", HIC"��"NOR CYN"�D"HIC"�D"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
