/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sheep.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : �p�֦�
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
	set_idname("sheep", HIW"�p�֦�"NOR);
	set_temp("status", HIC"�}"NOR CYN"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ǥi�H�ΨӸ������~���ʪ��C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 5000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 15);
	set(BUFF_AGI, 15);
	set(BUFF_PHY, 15);
	set("buff/status", HIW"�p�֦�"NOR);

	::setup_equipment(EQ_MOUNT, HIG"�y"NOR GRN"�M"NOR);
}
