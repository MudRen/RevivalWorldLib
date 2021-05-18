/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lord_sword_secret_art_2nd.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : ���̤��C�D�ĤG��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIY"lord"NOR YEL" sword "NOR WHT"secret art "HIW"2nd"NOR, HIY"����"NOR YEL"���C"NOR WHT"�D"HIW"�ĤG��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q���D���̤��C�W��ѥX�Ӫ��C�k���D�C");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
