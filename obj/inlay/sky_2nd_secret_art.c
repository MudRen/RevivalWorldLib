/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : sky_2nd_secret_art.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-30
 * Note	  : �ѼC�S��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIW"sky 2nd "NOR WHT"secret art"NOR, HIW"�ѼC"NOR WHT"�S��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q�ѼC���W���o���S�𯵧ޡC");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
