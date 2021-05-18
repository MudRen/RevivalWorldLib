/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : player.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-30
 * Note   : ���a�d���O
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
	set_idname(HIG"player"NOR GRN" board"NOR, HIG"���a"NOR GRN"�d��"NOR);
	
	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;
		
	set("board_id", "player");
	
	set("mass", -1);
}

