/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : moon_cake.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-10-07
 * Note   : 中秋節月餅
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

mapping actions;

string *awards =
({
	"/obj/festival/2006/mooncake/cake_glass",
	"/obj/festival/2006/mooncake/rabbit_ear",
	"/obj/festival/2006/mooncake/wukang_axe",
	"/obj/festival/2006/mooncake/shaddock_hat",
	"/obj/festival/2006/mooncake/hairpin",
});

void do_eat_cake(object me, string arg)
{
	object award;
	string msg;

	award = new(awards[random(sizeof(awards))]);

	msg = award->query_idname();
	
	log_file("command/mooncake", me->query_idname()+"獲得"+msg);
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"咬了幾口"+this_object()->query_idname()+"後，發現裡面包著「"+msg+"」。");
	
	award->set_keep();
	award->move(me);

	me->save();

	destruct(this_object());
}

void create()
{
	set_idname("moon cake", HIC"中秋"NOR+CYN"節"HIW"月餅"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "個");
	set("long", "中秋節月餅");
	set("mass", 100);
	set("value", 100);
	set("flag/no_amount", 1);

	actions = ([ "eat_cake" : (: do_eat_cake :) ]);
}
