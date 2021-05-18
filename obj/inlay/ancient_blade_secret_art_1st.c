/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : ancient_blade_secret_art_1st.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : ���j�M�k�D�Ĥ@��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIM"ancient "NOR MAG"blade "NOR WHT"secret art "HIW"1st"NOR, HIM"���j"NOR MAG"�M�k"NOR WHT"�D"HIW"�Ĥ@��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q���j���M�W��ѥX�Ӫ��M�k���D�C");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
