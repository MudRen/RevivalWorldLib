/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_wizhall_4.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-23
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

mapping actions;

void do_read(object me, string arg)
{
        if( query("total_online_time", me) > 86400*45 )
                return tell(me, pnoun(2, me)+"的總上線時間已經超過 45 天，無法再享用這個輔助法術了。\n");

        msg("$ME仔細地翻了翻「"HIG"新手"NOR GRN"培育術"NOR"」，突然全身上泛起一陣綠光，社會經驗值與戰鬥經驗值獲得 50% 的加成。\n", me, 0, 1);

        me->start_condition(NEWBIE);
}

void create()
{
        set("short", HIW"巫師"NOR WHT"神殿 "NOR YEL"正北殿"NOR);
        set("long",@TEXT
    這裡是巫師神殿正北邊的房間，這間房間非常的高大，往樓上看去似乎有一群
人正站在那討論著事情，但往四處查看卻找不到通往樓上的路。這間房間四周的牆
壁上畫滿著各式各樣的圖騰，與其他的房間相較起來，似乎詭譎許多。牆角則放著
一本破舊的書籍，上面寫著「新手培育術」，或許可以讀讀看(read)。
TEXT);

        set("exits", ([
                "west" : "/wiz/wizhall/room_wizhall_5",
                "east" : "/wiz/wizhall/room_wizhall_3",
                "south" : "/wiz/wizhall/room_wizhall_1",
                "up" : "/wiz/wizhall/room_wizhall_7",
        ]));

        set("lock", ([
                "up" : 6,
        ]));

        actions = ([
                "read": (: do_read :),
        ]);

        ::reset_objects();
}
