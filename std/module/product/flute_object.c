/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : object.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-14
 * Note   : 
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
#include <condition.h>
#include <daemon.h>

inherit STANDARD_MODULE_PRODUCT;
inherit EQUIPMENT;

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU橫劈攻擊，";
}

//
// 武器自動特殊攻擊
//
int special_attack(object attacker, object defender)
{
	if( !random(30) )
	{
		msg("$ME舉起"+this_object()->query_name()+"對著$YOU開始吹奏「"HIR"震"NOR RED"懾"HIR"幻"NOR RED"音"NOR"」。\n", attacker, defender, 1);
		
		defender->start_condition(FLUTE_WEAK);
	}
	return 0;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "根");
	set("mass", 2000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "flute");
	set(SPEED, 5);
	set(BUFF_INT, 5);

	::setup_equipment(EQ_TWOHAND, HIY"雙"NOR YEL"手"NOR);

	setup_inlay();
}
