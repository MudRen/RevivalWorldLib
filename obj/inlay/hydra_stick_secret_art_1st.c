/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : hydra_stick_secret_art_1st.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : ���w�ԴҪk�D�Ĥ@��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"hydra stick "NOR WHT"secret art "HIW"1st"NOR, HIW"���w��"NOR WHT"�Ҫk"NOR WHT"�D"HIW"�Ĥ@��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q���w�ԥ۴ҤW��ѥX�Ӫ��Ҫk���D�C");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
