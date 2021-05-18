/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dashen.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-08
 * Note   : ¤j­¼
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
	set_idname(HIB"da"NOR BLU"shen"NOR, HIB"¤j"NOR BLU"­¼"NOR);
	set_temp("status", HIW"¥v"NOR WHT"¸Ö");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "½ÑµÐÂÄ¼¯¶FÂÄ¡AÀ³¦p¬O­°¥ñ¨ä¤ß¡C\n");
	set("unit", "®è");
	set("mass", 0);
	set("value", 1000000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 115);
	set(BUFF_SOCIAL_EXP_BONUS, 80);
	set(BUFF_COMBAT_EXP_BONUS, 80);
	set("buff/status", HIB"¤j"NOR BLU"­¼"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"¯Ý"NOR GRN"°w"NOR);
}
