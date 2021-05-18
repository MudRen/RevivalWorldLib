/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battle_insigne.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-17
 * Note   : 戰爭勳章
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
	set_idname(HIC"bat"NOR CYN"tle "HIC"insi"NOR CYN"gne"NOR, HIC"戰"NOR CYN"爭"HIC"勳"NOR CYN"章"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");

	if( this_object()->set_shadow_database() ) return;

	set("long", "　從戰爭中獲得的戰爭勳章，可以用來兌換稀有的物品。\n");
	set("unit", "個");
	set("mass", 1);
	set("value", 100);
	set("badsell", 1);
	
	set("flag/no_drop", 1);
	set("flag/no_give", 1);
}
