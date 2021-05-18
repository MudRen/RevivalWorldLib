/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : heart_of_ocean.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-10
 * Note   : 海洋之心
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
	set_idname(HIB"heart of "NOR BLU"ocean"NOR, HIB"海洋"NOR BLU"之心"NOR);
	set_temp("status", HIC"神"NOR CYN"秘"NOR);

	if( this_object()->set_shadow_ob() ) return;

	set("long","從海神波塞冬身上取得的海洋之心，但不知其用途為何...。\n");
	set("unit", "顆");
	set("mass", 50);
	set("value", 10000);
	set("special", 1);

	set("material/"+WATER, 5000000);
	set("material/"+FUEL, 5000000);
	set("material/"+METAL, 5000000);
	set("material/"+STONE, 5000000);
	set("material/"+WOOD, 5000000);
}
