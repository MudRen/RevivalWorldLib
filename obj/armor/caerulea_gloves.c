/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : caerulea_gloves.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : ���N��M
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
	set_idname(HIB"caerulea"NOR BLU" gloves"NOR, HIB"���N"NOR BLU"��M"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�O�W���N�W��������M�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem2" : 3 ]));

	set(DEFEND, 3);
	set(BUFF_AGI, 2);

	set(BUFF_FULLSUIT+BUFF_HEALTH_MAX, 10);
	set(BUFF_FULLSUIT+BUFF_AGI, 1);

	::setup_equipment(EQ_GLOVES, HIY"��"NOR YEL"�M"NOR);
}

