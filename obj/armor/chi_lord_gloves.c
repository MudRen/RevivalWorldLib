/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : chi_hydra_gloves.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-10
 * Note   : ���D���̤�M
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
	set_idname(HIW"chi "NOR HIY"lord"NOR YEL" gloves"NOR, HIW"��"NOR WHT"�D"NOR HIY"��"NOR YEL"��"HIY"��"NOR YEL"�M"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�ƥj�N��r���O�����v�֤�M�C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem2" : 15 ]));

	set(DEFEND, 20);
	set(BUFF_STR, 3);
	set(BUFF_PHY, 3);
	set(BUFF_AGI, 3);
	set(BUFF_INT, 3);
	set(BUFF_CHA, 3);

	set(BUFF_FULLSUIT+BUFF_STR, 3);
	set(BUFF_FULLSUIT+BUFF_PHY, 3);
	set(BUFF_FULLSUIT+BUFF_AGI, 3);
	set(BUFF_FULLSUIT+BUFF_INT, 3);
	set(BUFF_FULLSUIT+BUFF_CHA, 3);
	set(BUFF_FULLSUIT+BUFF_COUNTERATTACK_CHANCE, 1);
	
	::setup_equipment(EQ_GLOVES, HIY"��"NOR YEL"�M"NOR);
}

