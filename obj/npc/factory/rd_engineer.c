/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rd_engineer.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-17
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <gender.h>

inherit STANDARD_NPC;

int is_factory_staff()
{
	return 1;
}

void create()
{
        set_idname("RD engineer", "研發工程師");

	set("unit", "位");
	set("position", HIY"工程師"NOR);
        set("gender", random(2) ? MALE_GENDER : FEMALE_GENDER);
	set("salary", ({ 1000, 100000 }));
	set_temp("status", HIR"研發"NOR);
	
	startup_living();
}
