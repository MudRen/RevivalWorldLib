/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : room_godsland_4.c
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
	if( arg == "石柱" )
		return "這個石柱很像是人造的，似乎是一個開關，或許用力一點可以推動(push)它。\n";
	
	return 0;
}

void create()
{
	set("short", WHT"潮濕洞穴"NOR);
	set("long",@TEXT
　　這個潮濕的洞穴似乎很久沒有人來過了，隱約可見石壁上刻有許多圖文，但因
年代久遠已經無法辨識，只有地面一個突出的方型石柱，似乎是某種開關似的。往
深處走似乎還有一些叉路，但絕大部份都已經被落石所封閉。
TEXT);

	set("exits", ([ 
		"south" : "/quest/godsland/room_godsland_3",
		"west" : "/quest/godsland/room_godsland_5",
	 ]));

	::reset_objects();
}

void do_push(object me, string arg)
{
	int *quest = query_temp("godsland_enter", me) || allocate(0);
	
	if( !sizeof(quest) || quest[<1] != 1 )
	{
		msg("$ME奮力推動石柱，但石柱卻又很快的退回原位，似乎有什麼地方弄錯了。\n", me, 0, 1);
		delete_temp("godsland_enter", me);
	}
	else
	{
		msg("$ME奮力推動石柱，地板傳回了一些震動。\n", me, 0, 1);
		set_temp("godsland_enter", quest + ({ 4 }), me);
	}
}

mapping actions = 
([
	"push" : (: do_push :),
]);
