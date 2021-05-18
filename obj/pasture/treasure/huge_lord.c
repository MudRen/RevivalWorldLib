/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : huge_lord.c
 * Author : Clode@RevivalWorld
 * Date   : 2013-03-04
 * Note   : �j���D
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
	set_idname(HIC"h"NOR CYN"u"HIC"g"NOR CYN"e "HIC"l"NOR CYN"o"HIC"r"NOR CYN"d", HIC"�j"NOR CYN"��"HIC"�D"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�O�W���T�������C\n");
	set("unit", "�Y");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 180);
	set(BUFF_STAMINA_REGEN, 50);
	set(BUFF_COLLECT_ABILITY, 3);
	
	set("buff/status", HIC"�j"NOR CYN"��"HIC"�D"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
