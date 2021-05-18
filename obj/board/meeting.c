/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : meeting.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-3-21
 * Note   : �|ĳ�����d���O
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
	set_idname("meeting board", "�|ĳ�����P²��");
	
	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;

	set("board_id", "meeting");
	
	set("mass", -1);
}
