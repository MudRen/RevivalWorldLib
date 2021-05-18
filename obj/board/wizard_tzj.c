/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_tzj.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-12
 * Note   : Tzj的個人板
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit STANDARD_BULLETIN_BOARD;

void create()
{
	set_idname("tzj's board", HIW"潼"+NOR+"恩"+WHT+"的"+HIY+"留"+NOR YEL+"言"+HIY+"板"NOR);
	
	set_temp("decorated", 1);

	// 利用 shadow_ob 技術來做分散式公佈欄, 即時資料皆儲存於 shadow_ob 上
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_tzj");
	
	set("mass", -1);
}
