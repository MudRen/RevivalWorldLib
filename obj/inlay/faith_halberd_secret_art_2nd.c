/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : faith_halberd_secret_art_2nd.c
 * Author : Clode@RevivalWorld
 * Date	  : 2009-12-26
 * Note	  : �H�����u�D�ĤG��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void create()
{
	set_idname(HIG"faith"NOR GRN" halberd "NOR WHT"secret art "HIW"2nd"NOR, HIG"�H��"NOR GRN"���u"NOR WHT"�D"HIW"�ĤG��"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "�q�H�����u�W��ѥX�Ӫ��u�k���D�C");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
