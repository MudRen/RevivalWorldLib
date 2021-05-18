/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tsouni_horse.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-05-04
 * Note   : ��d��
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
	set_idname(HIW"tsouni "NOR WHT"horse"NOR, HIW"��d"NOR WHT"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ط����ʼw����N���ܮy�M�C\n");
	set("unit", "�Y");
	set("mass", 0);
	set("value", 500000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 100);
	set(BUFF_STAMINA_REGEN, 20);
	set("buff/status", HIW"��d"NOR WHT"��"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
