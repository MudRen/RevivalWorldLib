/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : red_envelope_1strwparty.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-05-14
 * Note   : �Ĥ@�����E���ʩ�����]
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
	
	log_file("command/rwparty", me->query_idname()+"��o"+msg);
	
         CHANNEL_D->channel_broadcast("news", me->query_idname()+"���}"+this_object()->query_idname()+"�A���u"+msg+"�v�A���L�]���S�Ӻ��E�A�F��Q Clode �^���F�C");
	


	destruct(this_object());
}

void create()
{
	set_idname(HIR"red envelope 1st rwparty"NOR, HIC"�Ĥ@��"NOR CYN"���ͪ��@�ɺ��E"HIR"���"NOR RED"���]"NOR);
	
	if( this_object()->set_shadow_database() ) return;
	
	set("unit", "��");
	set("long", "�Ĥ@�����ͪ��@�ɺ��E������]");
	set("mass", 100);
	set("value", 100);
	set("flag/no_amount", 1);

	actions = ([ "open_envelope" : (: do_open_envelope :) ]);
}
