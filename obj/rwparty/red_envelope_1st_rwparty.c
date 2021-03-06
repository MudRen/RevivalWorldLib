/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : red_envelope_1strwparty.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-05-14
 * Note   : 第一屆網聚活動抽獎紅包
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
	"/obj/rwparty/gift_of_clode_1st",
	"/obj/rwparty/gift_of_cookys_1st",
	"/obj/rwparty/gift_of_sinji_1st",
	"/obj/rwparty/gift_of_msr_1st",
	"/obj/rwparty/gift_of_whatup_1st",
	"/obj/rwparty/gift_of_lak_1st",
	"/obj/rwparty/gift_of_dragonku_1st",
	"/obj/rwparty/gift_of_kyo_1st",
	"/obj/rwparty/gift_of_tzj_1st"
});

void do_open_envelope(object me, string arg)
{
	object award;
	string msg;

	award = new(awards[random(sizeof(awards))]);

	msg = award->query_idname();
	
	log_file("command/rwparty", me->query_idname()+"獲得"+msg);
	
	CHANNEL_D->channel_broadcast("news", me->query_idname()+"打開"+this_object()->query_idname()+"，得到「"+msg+"」。");
	
	award->set_keep();
	award->move(me);

	me->save();

	destruct(this_object());
}

void create()
{
	set_idname(HIR"red envelope 1st rwparty"NOR, HIC"第一屆"NOR CYN"重生的世界網聚"HIR"抽獎"NOR RED"紅包"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "個");
	set("long", "第一屆重生的世界網聚抽獎紅包");
	set("mass", 100);
	set("value", 100);
	set("flag/no_amount", 1);

	actions = ([ "open_envelope" : (: do_open_envelope :) ]);
}
