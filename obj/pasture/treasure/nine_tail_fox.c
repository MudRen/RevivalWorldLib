/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : nine_tail_fox.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-05-20
 * Note   : 九尾狐
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIW"nine "NOR WHT"tail "HIW"fox"NOR, HIW"九"NOR WHT"尾"HIW"狐"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "憾動地界之神話座騎。\n");
	set("unit", "頭");
	set("mass", 0);
	set("value", 200000000);
	set("flag/no_amount", 1);
	set(BUFF_STR, 85);
	set(BUFF_STAMINA_REGEN, 15);
	set("buff/status", HIW"九"NOR WHT"尾"HIW"狐"NOR);
	
	::setup_equipment(EQ_MOUNT, HIG"座"NOR GRN"騎"NOR);
}
