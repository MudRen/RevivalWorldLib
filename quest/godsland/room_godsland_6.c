/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_godsland_6.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-30
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

string query_look_message(string arg)
{
	if( arg == "石壁" )
		return "這個石壁相當的高，但或許有機會攀爬過去(climb)。\n";
	
	return 0;
}

void create()
{
	set("short", WHT"潮濕洞穴"NOR);
	set("long",@TEXT
　　這裡是洞穴的最深處了，面前出現一道高聳的石壁，壁上刻劃的圖像仍然保持
得相當完整，圖中描繪著大量的人民準備了大量的祭品正在祭拜的天神，只見那刻
畫的天神高聳入雲，其態勢非常震懾人心。石壁的最頂端隱約發著白光，好像有個
出口在那，或許可以試著攀爬上去。
TEXT);

	set("exits", ([ 
		"south" : "/quest/godsland/room_godsland_5",
	 ]));

	::reset_objects();
}

void do_climb(object me, string arg)
{
	int *quest = query_temp("godsland_enter", me) || allocate(0);

	if( sizeof(quest) != 5 )
		return msg("$ME試著攀爬那道石壁，但是實在是太高了，$ME退回底下嘆息了一聲。\n", me, 0, 1);
	
	delete_temp("godsland_enter", me);
	
	msg("$ME成功攀爬上那道變矮了的石壁，很快的$ME已經消失在石壁後的山洞之中...。\n", me, 0, 1);
		
	me->move(({ 49, 99, 0, "godsland", 0, 0, "49/99/0/godsland/0/0" }));
	msg("$ME穿越漆黑的通道，來到"+AREA_D->query_area_idname("godsland", 0)+"。\n", me, 0, 1);
	
	CHANNEL_D->channel_broadcast("combat", me->query_idname()+"穿越漆黑的通道，到達"+AREA_D->query_area_idname("godsland", 0)+"。");
}

mapping actions = 
([
	"climb" : (: do_climb :),
]);

