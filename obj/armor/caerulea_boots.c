/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : caerulea_boots.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ���N�c�l
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
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIB"caerulea"NOR BLU" boots"NOR, HIB"���N"NOR BLU"�c�l"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�O�W���N�W�������c�l�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem5" : 3 ]));

	set(DEFEND, 3);
	set(BUFF_AGI, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_BOOTS, HIY"�c"NOR YEL"�l"NOR);
}

