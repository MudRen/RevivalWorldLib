/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_stone.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-05-19
 * Note   : â�I��
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
	set_idname(HIM"zen "NOR MAG"stone"NOR, HIM"â"NOR MAG"�I"HIM"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӷ����}�������j���Y�C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 200000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 85);
	set(BUFF_ENERGY_REGEN, 15);
	set("buff/status", HIM"â"NOR MAG"�I"HIM"��"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
