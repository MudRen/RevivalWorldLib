/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rabbit_stick.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-09-22
 * Note   : 月兔搗槌
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
#include <daemon.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname("rabbit stick", HIC"月兔"NOR+CYN"搗"HIW"槌"NOR);
	set_temp("status", HIB"20"NOR BLU"07 "HIB"中"NOR BLU"秋");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "中秋發放的紀念性禮物。\n");
	set("unit", "個");
	set("mass", 10);
	set("value", 1);
	set("badsell", 1);
	set("flag/no_amount", 1);
	
	set("skill", "stick");

	set(ATTACK, 1);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME舉起手中"+this_object()->query_name()+"往$YOU攻擊，";
}

//
// 武器自動特殊攻擊
//
int special_attack(object attacker, object defender)
{
	if( !random(100) )
	{
		msg("$ME舉起手中的"+this_object()->query_idname()+"，狠狠擊中$YOU的後腦，使$YOU造成暈眩！\n", attacker, defender, 1);

		defender->start_delay("moon_rabbit", 3, pnoun(2, defender)+"正在暈眩中。\n", pnoun(2, defender)+"停止了暈眩。\n");
	}

	return 0;
}
