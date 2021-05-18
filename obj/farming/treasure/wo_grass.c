/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wo_grass.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-05-04
 * Note   : �ׯ�
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
	set_idname(HIG"wo "NOR GRN"grass"NOR, HIG"��"NOR GRN"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�@�ط����ʼw����N���ܴӪ��C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 500000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 100);
	set(BUFF_SOCIAL_EXP_BONUS, 70);
	set(BUFF_COMBAT_EXP_BONUS, 70);
	set("buff/status", HIG"��"NOR GRN"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
