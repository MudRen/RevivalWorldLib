/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : map.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-08-26
 * Note	  : 藏寶圖
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit	STANDARD_OBJECT;

void create(string option)
{
	int level;
	
	if( !stringp(option) ) return;

	sscanf(option, "%d", level);
	
	set_idname(HIW"lv"+level+NOR WHT" treasure map", HIW+CHINESE_D->chinese_number(level)+"級"NOR WHT"藏寶圖"NOR);
	
	if( level >= 5 )
		set_temp("status", HIW"史"NOR WHT"詩");
	else if( level >= 3 )
		set_temp("status", HIG"傳"NOR GRN"奇");
	else
		set_temp("status", HIC"稀"NOR CYN"有");

	set("level", level);

	//if( this_object()->set_shadow_ob() ) return;

	set("long", "意外中獲得的藏寶圖，圖上似乎描繪著稀有寶物的地點。");
	set("unit", "張");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
