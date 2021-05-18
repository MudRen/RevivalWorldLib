/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : jintu.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �b�g
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
	set_idname(HIY"jin"NOR YEL"tu", HIY"�b"NOR YEL"�g"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�p�Ӧb�M�O��ҡA��k��L�ұo�C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 1000000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 115);
	set(BUFF_ENERGY_REGEN, 30);
	set("buff/status", HIY"�b"NOR YEL"�g"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
