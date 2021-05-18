/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : wizard_clode.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-27
 * Note   : Clode���ӤH�O
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
	set_idname("clode's board", HIC"��"NOR CYN"��"HIW BYEL"��"BCYN"��"BGRN"�O"BRED"ï"NOR);
	
	set_temp("decorated", 1);

	// �Q�� shadow_ob �޳N�Ӱ����������G��, �Y�ɸ�Ƭ��x�s�� shadow_ob �W
	if( this_object()->set_shadow_ob() ) return;
	
	set("board_id", "wizard_clode");
	
	set("mass", -1);

}
