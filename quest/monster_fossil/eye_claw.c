/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : eye_claw.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-01-16
 * Note	  : �ʲ��s��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIM"eye"NOR MAG" claw"NOR, HIW"�j"NOR WHT"���D"HIM"�ʲ�"NOR MAG"�s��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ҥj�Ǯa�q�@���ǻ����֦��W�ʭӲ��������s���W���o���s���A���G�O�s�@�˳ƪ��n���~�C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
