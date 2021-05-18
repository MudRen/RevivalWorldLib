/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battle_insigne.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-17
 * Note   : �Ԫ�����
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname(HIC"bat"NOR CYN"tle "HIC"insi"NOR CYN"gne"NOR, HIC"��"NOR CYN"��"HIC"��"NOR CYN"��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�q�Ԫ�����o���Ԫ������A�i�H�ΨӧI���}�������~�C\n");
	set("unit", "��");
	set("mass", 1);
	set("value", 100);
	set("badsell", 1);
	
	set("flag/no_drop", 1);
	set("flag/no_give", 1);
}
