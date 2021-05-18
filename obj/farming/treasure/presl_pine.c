/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : presl_pine.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-05-19
 * Note   : ù�~�Q
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
	set_idname(HIG"presl "NOR GRN"pine"NOR, HIG"ù"NOR GRN"�~"NOR HIG"�Q"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�@�ӷ����}�������j�Q��H���C\n");
	set("unit", "��");
	set("mass", 0);
	set("value", 200000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 85);
	set(BUFF_SOCIAL_EXP_BONUS, 60);
	set(BUFF_COMBAT_EXP_BONUS, 60);
	set("buff/status", HIG"ù"NOR GRN"�~"NOR HIG"�Q"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"��"NOR GRN"�w"NOR);
}
