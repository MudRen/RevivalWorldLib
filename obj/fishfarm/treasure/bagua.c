/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bagua.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �K��
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
	set_idname(HIM"ba"NOR MAG"gua", HIM"�K"NOR MAG"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "���B�_�B��B�I�B�S�B�[�B���B���C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 130);
	set(BUFF_ENERGY_REGEN, 35);
	set("buff/status", HIM"�K"NOR MAG"��"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
