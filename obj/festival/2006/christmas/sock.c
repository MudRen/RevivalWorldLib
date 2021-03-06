/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : christmas_sock.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-12-25
 * Note   : 聖誕襪
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
        "/obj/festival/2006/christmas/xmas_ring",
        "/obj/festival/2006/christmas/xmas_ring_hide",
});

void do_pray_deer(object me, string arg)
{
        object award;
        string msg;

        award = new(awards[random(sizeof(awards))]);

        msg = award->query_idname();
        
        log_file("command/sock", me->query_idname()+"獲得"+msg);
        
        CHANNEL_D->channel_broadcast("news", me->query_idname()+"打開在聖誕夜掛上的"+this_object()->query_idname()+"後，發現裡面有鹿神的禮物「"+msg+"」。");
        
        award->set_keep();

        award->move(me);

        me->save();

        destruct(this_object());
}

void create()
{
        set_idname("christmas sock", HIR"聖"NOR+RED"誕"HIW"襪"NOR);
        
        if( this_object()->set_shadow_database() ) return;
        
        set("unit", "個");
        set("long", "聖誕節襪子");
        set("mass", 100);
        set("value", 100);
        set("flag/no_amount", 1);

        actions = ([ "pray_deer" : (: do_pray_deer :) ]);
}
