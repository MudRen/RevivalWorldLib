/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : eagle_feather.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-01-16
 * Note	  : �e�N���l
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIC"eagle"NOR CYN" feather"NOR, HIW"�j"NOR WHT"���D"HIC"�e�N"NOR CYN"���l"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�ҥj�Ǯa�q�@���ժ���e�������檺���N���W���U�����l�A���G�O�s�@�˳ƪ��n���~�C");
	set("unit", "��");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
