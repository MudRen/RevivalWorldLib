/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : monster_gall.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-25
 * Note	  : ���~���x
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR YEL"monster "HIY"gall"NOR, HIW"�j"NOR WHT"���D"NOR YEL"���~"HIY"���x"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ҥj�Ǯa�q�@���_�h�`�B���X���@���x�n�ƥۡA���G�O�s�@�Z�����n���~�C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
