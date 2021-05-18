/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_msr.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-12
 * Note   : Msr的個人板
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
	set_idname("lililala board", "囉哩囉唆留言板");

	set_temp("decorated", 1);

	// 利用 shadow_ob 技術來做分散式公佈欄, 即時資料皆儲存於 shadow_ob 上
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_msr");
	
	set("mass", -1);
}
