/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : liangyi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : ���
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
	set_idname(HIY"liang"NOR YEL"yi"NOR, HIY"��"NOR YEL"��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ֳ��Ӷ��B�ֶ��ӳ��C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 2000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 130);
	set(BUFF_SOCIAL_EXP_BONUS, 90);
	set(BUFF_COMBAT_EXP_BONUS, 90);
	set("buff/status", HIY"��"NOR YEL"��"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
