/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : jade_emerald.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-25
 * Note	  : �եɻB�A
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"jade "HIG"emerald"NOR, HIW"�j"NOR WHT"���D"HIW"�ե�"HIG"�B�A"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ҥj�Ǯa�q�@���_�h�`�B���X���@�ӻB�A�A���G�O�s�@���㪺�n���~�C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
