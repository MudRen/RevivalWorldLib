/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bug_red_envelope.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-12
 * Note   : bug �^�����]
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
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"���}"+this_object()->query_idname()+"�A�o�� "+msg+"�C");
	
	destruct(this_object());
	
	me->save();
}

void create()
{
	set_idname(HIR"bug red envelope"NOR, NOR YEL"���j "HIY"BUG "NOR YEL"�^��"HIR"���y"NOR RED"���]"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "���j BUG �^�����y���]�A���}��i�H�H����o�S����~");
	set("mass", 100);
	set("value", -1);
	set("flag/no_amount", 1);
	set("flag/no_give", 1);
	set("flag/no_drop", 1);

	actions = ([ "open_envelope" : (: do_open_envelope :) ]);
}
