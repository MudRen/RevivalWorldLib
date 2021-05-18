/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : firework.c
 * Author : 
 * Date   : 2017-12-12
 * Note   : 耶誕寶箱
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <feature.h>
#include <object.h>
#include <daemon.h>
#include <message.h>
#include <inherit.h>
#include <ansi.h>

inherit STANDARD_OBJECT;

int do_open( object me, string arg );

mapping actions;

string gift_dir = "/obj/festival/2017/christmas/gift/";

void create()
{
    set_idname( "christmas box 2017", "耶誕寶盒" );

    if( clonep() ) 
        set("shadow_ob", find_object(base_name(this_object())));
    else
    {
        set("long", "這是2017年紀念品專用寶箱, 你可以打開它(open box)看得到什麼獎品。\n");
        set("unit", "箱");
        set("mass", 2);
        set("badsell", 1);
    }

    actions = 
    ([ 
      "open"     : (: do_open :)
    ]);
}

int do_open( object me, string arg)
{
    string msg ,*gifts = ({});
    object ob;

    if(!arg) 
        return tell( me, "你想打開什麼呢？\n");
    else if( arg == "box" ) 
    {
        gifts = get_dir( gift_dir );

        if( !sizeof(gifts) )
        {
            tell( me, sprintf("目前並沒有任何獎品可以領取, 請通知巫師處理\n") );
            return 1;
        }

        if( !objectp( ob = new(sprintf("%s%s", gift_dir, gifts[random(sizeof(gifts))] )) ))
        {
            tell( me, sprintf("禮品取得失敗, 請通知巫師處理\n") );
            return 1;
        }

        ob->move(me);
        ob->set_keep();

        // save
        find_object("/wiz/home/msr/room/room_ring.c")->save_user( me, base_name(ob) );

        CHANNEL_D->channel_broadcast("news",
          sprintf("%s抽中一只%s。\n", me->query_idname(), ob->short(1)));        

        msg = sprintf("$ME得到一只%s。\n", ob->query_idname());
        msg( msg, me, 0, 1, "123");
        destruct( this_object());
        return 1;
    }
}
