/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : s_alvin_room.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-10
 * Note   : 蘇菲亞的弟弟－阿爾文的臥室
 * Update :
 *  
 * 2007-08-17  整理資料夾位置
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_ROOM;

string help = @HELP
在這個平凡無奇的房間內並沒有任何特殊的資訊。
HELP;

void create()
{
	set("short", HIC"臥室"NOR);
	set("long",@TEXT
　　推開半掩的門走進此處，一旁置放著張兒童床。這張床看起來有重新舖過，
但因為上面隨意的擺放積木、小汽車模型及折紙之類的童玩，因此顯得有些凌亂
。在靠窗的桌上散落些玻璃珠子，一把粗製彈弓。並有張頑皮男孩與小白狗的合
照被放置在木質相框內，擱在桌上。
TEXT);


	set("exits", ([
		//往東走是牆
		//"east"    : ,
		
		//往南走是牆
		//"south" : ,
		
		//往西回到客廳
		"west"    : "/quest/suphia_and_benjamin/room/s_frontroom",
		
		//往北走是牆
		//"north" : ,
		
	]));

/*	預留放NPC或物品的...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
