/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : thousandleaf_lotus.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-12-19
 * Note   : 千葉蓮
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
	set_idname(HIW"thousandleaf "NOR WHT"lotus"NOR, HIW"千葉"NOR WHT"蓮"NOR);
	set_temp("status", HIC"稀"NOR CYN"有");

	if( this_object()->set_shadow_ob() ) return;

	set("long", "宋蘇轍詩云：「蓮花生淤泥，淨色比天女。臨池見千葉，謫墮問何故。\n空明世無匹，銀瓶送佛所。清泉養芳潔，為我三日住。\n籍然落實床，應返梵天去。」\n");
	set("unit", "株");
	set("mass", 0);
	set("value", 4000000);
	set("flag/no_amount", 1);
	set(BUFF_CHA, 24);
	set("buff/status", HIW"千葉"NOR WHT"蓮"NOR);
	
	::setup_equipment(EQ_BROOCH, HIG"胸"NOR GRN"針"NOR);
}
