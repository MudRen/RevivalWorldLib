/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_msr.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-12
 * Note   : Msr���ӤH�O
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
	set_idname("lililala board", "�o���o���d���O");

	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_msr");
	
	set("mass", -1);
}
