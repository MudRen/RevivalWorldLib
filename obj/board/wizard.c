/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-21
 * Note   : �Ův�d���O
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
	set_idname(HIY"wizard"NOR YEL" board"NOR, HIY"�Ův"NOR YEL"�d��"NOR);
	
	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;
		
	set("board_id", "wizard");
	
	set("mass", -1);
}
