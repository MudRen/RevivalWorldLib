/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_lostland.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-18
 * Note   : 標準房間
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <quest.h>
#include <daemon.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIM"神秘"NOR MAG"的墓穴"NOR);
	set("long",@TEXT
    這裡是一個極為古老的墓穴，手邊摸去都是冷冰冰且潮濕的石璧，石壁上滿佈
著武器劈砍過的痕跡，地上也佈滿著各式武器的殘骸，最深處放了三副石棺，打開
這些石棺往下一看居然都深不見底，只有一陣腐臭味不斷從深處撲鼻而來，實在讓
人有些難受，但這三個這麼深的棺底洞穴卻著實惹人好奇。

　　╔═══════╗　╔═══════╗　╔═══════╗
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　　　遠　　　║　║　　　消　　　║　║　　　遠　　　║
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　　　古　　　║　║　　　失　　　║　║　　　古　　　║
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　　　刀　　　║　║　　　的　　　║　║　　　海　　　║
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　　　塚　　　║　║　　　文　　　║　║　　　島　　　║
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　　　　　　　║　║　　　明　　　║　║　　　　　　　║
　　║　　　　　　　║　║　　　　　　　║　║　　　　　　　║
　　║　║╔═╗║　║　║　║╔═╗║　║　║　║╔═╗║　║
　　╚═╩╩　╩╩═╝　╚═╩╩　╩╩═╝　╚═╩╩　╩╩═╝
TEXT);
	set("exits", ([ "out" : ({ 34, 58, 0, "sixcastle", 1, 0, "34/58/0/sixcastle/1/0" }) ]));

	::reset_objects();
}

void do_jump(object me, string arg)
{
	if( !arg )
		return tell(me, pnoun(2, me)+"想要跳進哪個石棺(left、right、middle)？\n");

	arg = lower_case(arg);

	if( arg == "left" )
	{
		msg("$ME往放置在左邊的棺材裡跳了進去...。\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key" :))) )
		{
			msg("$ME被一道無形的力量反彈了出來，似乎有一道牆擋住了通道...。\n", me, 0, 1);
			return;
		}
		
		me->move(({ 0, 0, 0, "lostland", 0, 0, "0/0/0/lostland/0/0" }));
		msg("$ME穿越漆黑的通道，來到"+AREA_D->query_area_idname("lostland", 0)+"。\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"穿越漆黑的通道，到達"+AREA_D->query_area_idname("lostland", 0)+"。");
	}
	else if( arg == "right" )
	{
		msg("$ME往放置在右邊的棺材裡跳了進去...。\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key_2" :))) )
		{
			msg("$ME被一道無形的力量反彈了出來，似乎有一道牆擋住了通道...。\n", me, 0, 1);
			return;
		}
		
		me->move(({ 28, 89, 0, "lostland", 1, 0, "28/89/0/lostland/1/0" }));
		msg("$ME穿越漆黑的通道，來到"+AREA_D->query_area_idname("lostland", 1)+"。\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"穿越漆黑的通道，到達"+AREA_D->query_area_idname("lostland", 1)+"。");
	}
	else if( arg == "middle" )
	{
		msg("$ME往放置在中央的棺材裡跳了進去...。\n", me, 0, 1);
		
		if( !sizeof(filter_array(all_inventory(me), (: base_name($1) == "/quest/lost_shard/lostland_key_3" :))) )
		{
			msg("$ME被一道無形的力量反彈了出來，似乎有一道牆擋住了通道...。\n", me, 0, 1);
			return;
		}
		
		me->move(({ 0, 0, 0, "lostland", 2, 0, "0/0/0/lostland/2/0" }));
		msg("$ME穿越漆黑的通道，來到"+AREA_D->query_area_idname("lostland", 2)+"。\n", me, 0, 1);
		
		CHANNEL_D->channel_broadcast("combat", me->query_idname()+"穿越漆黑的通道，到達"+AREA_D->query_area_idname("lostland", 2)+"。");
	}
	else
		tell(me, pnoun(2, me)+"想要跳進哪個石棺(left、right、middle)？\n");
}

mapping actions = 
([
	"jump" : (: do_jump :),
]);
