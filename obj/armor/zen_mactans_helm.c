/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : zen_mactans_helm.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-16
 * Note   : �I�D����Y��
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
	set_idname(HIW"zen "NOR WHT"mactans "NOR HIW"helm"NOR, HIW"�I"NOR WHT"�D"NOR WHT"���"NOR HIW"�Y��"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@�Ʊq�¹���W�������v���Y���C\n");
	set("unit", "��");
	set("mass", 500);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ "/obj/gem/gem1" : 6 ]));

	set(DEFEND, 6);
	set(BUFF_AGI, 3);

	set(BUFF_FULLSUIT+BUFF_AGI, 3);

	::setup_equipment(EQ_HEAD, HIY"�Y"NOR YEL"��"NOR);
}

