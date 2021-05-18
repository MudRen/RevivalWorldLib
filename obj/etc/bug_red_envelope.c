/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug_red_envelope.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-12
 * Note   : bug 回報紅包
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
	"/obj/etc/honor_card",
});

void do_open_envelope(object me, string arg)
{
	object award = new(awards[random(sizeof(awards))]);

	string msg = award->query_idname();
		
	award->set_keep();
	award->move(me);
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"打開"+this_object()->query_idname()+"，得到 "+msg+"。");
	
	destruct(this_object());
	
	me->save();
}

void create()
{
	set_idname(HIR"bug red envelope"NOR, NOR YEL"重大 "HIY"BUG "NOR YEL"回報"HIR"獎勵"NOR RED"紅包"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "個");
	set("long", "重大 BUG 回報獎勵紅包，打開後可以隨機獲得特殊獎品");
	set("mass", 100);
	set("value", -1);
	set("flag/no_amount", 1);
	set("flag/no_give", 1);
	set("flag/no_drop", 1);

	actions = ([ "open_envelope" : (: do_open_envelope :) ]);
}
