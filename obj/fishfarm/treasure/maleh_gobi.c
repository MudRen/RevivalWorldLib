/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : maleh_gobi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-05-04
 * Note   : ���Ǥ��z
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
	set_idname(HIR"maleh "NOR RED"gobi"NOR, HIR"����"NOR RED"���z"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ӷ����ʼw����N���ƤƥۡC\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 500000000);
	set("flag/no_amount", 1);
	set(BUFF_INT, 100);
	set(BUFF_ENERGY_REGEN, 20);
	set("buff/status", HIR"����"NOR RED"���z"NOR);
	
	::setup_equipment(EQ_EYE, HIG"��"NOR GRN"��"NOR);
}
