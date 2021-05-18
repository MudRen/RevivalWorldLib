/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sishiang.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : �|�H
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
	set_idname(HIC"si"NOR CYN"shiang"NOR, HIC"�|"NOR CYN"�H"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�ֳ��B�ֶ��B�Ӷ��B�ӳ��C\n");
	set("unit", "�Y");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 130);
	set(BUFF_STAMINA_REGEN, 35);
	set("buff/status", HIC"�|"NOR CYN"�H"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
