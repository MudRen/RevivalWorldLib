/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_cookys.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-27
 * Note   : cookys的個人板
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
	set_idname("cookys' board", HIC"餅乾"NOR  BYEL"的"BCYN"留"BGRN"言"BRED"板"NOR);

	set_temp("decorated", 1);

	// 利用 shadow_ob 技術來做分散式公佈欄, 即時資料皆儲存於 shadow_ob 上
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_cookys");
	
	set("mass", -1);
}
