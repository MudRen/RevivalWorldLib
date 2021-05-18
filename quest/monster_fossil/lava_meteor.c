/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lava_meteor.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-24
 * Note	  : �����k��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(NOR RED"lava "HIR"meteor"NOR, HIW"�j"NOR WHT"���D"NOR RED"����"HIR"�k��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ҥj�Ǯa�q�@���_�h�`�B���X���k�ۡA�k�ۤ������G�٦��`�������b�y�ʡA���G�O�s�@�Z�����n���~�C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
