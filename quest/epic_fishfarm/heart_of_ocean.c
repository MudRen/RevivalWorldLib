/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : heart_of_ocean.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-10
 * Note   : ���v����
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <material.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname(HIB"heart of "NOR BLU"ocean"NOR, HIB"���v"NOR BLU"����"NOR);
	set_temp("status", HIC"��"NOR CYN"��"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("long","�q�����i��V���W���o�����v���ߡA��������γ~����...�C\n");
	set("unit", "��");
	set("mass", 50);
	set("value", 10000);
	set("special", 1);

	set("material/"+WATER, 5000000);
	set("material/"+FUEL, 5000000);
	set("material/"+METAL, 5000000);
	set("material/"+STONE, 5000000);
	set("material/"+WOOD, 5000000);
}
