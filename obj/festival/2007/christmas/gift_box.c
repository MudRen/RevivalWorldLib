/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gift_box.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-20
 * Note   : 禮物
 * Update :
 *  o 2003-00-00  


 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <location.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

void destruct_self()
{
	object me = this_player();
	set_temp("christmas", 0, me); // 步驟歸0
	tell( me,HIR"任務失敗，請重新來過。\n"NOR);
	destruct(this_object());
}

void create()
{
	set_idname("gift box", HIR"禮"NOR+RED"物"HIW"盒"NOR);

	if( this_object()->set_shadow_database() ) return;

	set("unit", "份");
	set("long", "聖誕老人交托的東西。");
	set("mass", 1);
	set("value", 1);
	set("flag/no_amount", 1);

	if( clonep() )
		call_out( (: destruct_self :), 180 ); // 三分鐘內沒送到就消失
}
