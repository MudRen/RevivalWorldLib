/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_tzj.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-12
 * Note   : Tzj���ӤH�O
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
	set_idname("tzj's board", HIW"��"+NOR+"��"+WHT+"��"+HIY+"�d"+NOR YEL+"��"+HIY+"�O"NOR);
	
	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_tzj");
	
	set("mass", -1);
}
