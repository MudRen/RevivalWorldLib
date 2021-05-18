/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : s_afterroom.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-09
 * Note   : 蘇菲亞家的置物間
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
	set("short", CYN"置物間"NOR);
	set("long",@TEXT
　　兩側存放了幾件盛水器具及幾袋雞飼料，也擱置了農耕所需要用到的各種工
具。由於經過整理，所以雖然此處東西種類繁多，但看起來卻一點也不覺得雜亂
。站在此處不時可聞到食物的香氣，從屋子深處飄來，而屋外也經常傳來雞群細
瑣的鳴叫聲。
TEXT);


	set("exits", ([
		//往東走是門
		"east"    : ({ 82, 66, 0, "lightforest", 0, 0, "82/66/0/lightforest/0/0" }),
		
		//往南走是牆
		//"south" : ,
		
		//往西走
		"west"    : "/quest/suphia_and_benjamin/room/s_kitchen",
		
		//往北走是門
		"north" : ({ 81, 65, 0, "lightforest", 0, 0, "81/65/0/lightforest/0/0" }),
			
		//此處有通往2F的樓梯
		//"up"    : ,
		
	]));

/*	預留放NPC或物品的...
	set("objects", ([
		"/quest/suphia_and_benjamin/npc_suphia" : 1 ,
	]));
*/
	
	::reset_objects();
	replace_program(STANDARD_ROOM);
}
