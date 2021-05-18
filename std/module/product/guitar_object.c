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
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU��A�����A";
}

//
// �Z���۰ʯS�����
//
int special_attack(object attacker, object defender)
{
	if( !random(30) )
	{
		msg("$ME�|�_"+this_object()->query_name()+"���$YOU�}�l�u���u"HIY"�_"NOR YEL"��"HIY"�M"NOR YEL"��"NOR"�v�C\n", attacker, defender, 1);
		
		defender->start_condition(GUITAR_WEAK);
	}
	return 0;
}

varargs void create(string file, string type)
{
	::create(file, type);

	set("unit", "��");
	set("mass", 2000);
	set("value", query_value());
	set("flag/no_amount", 1);

	set("skill", "guitar");
	set(SPEED, 5);
	set(BUFF_CHA, 5);

	::setup_equipment(EQ_TWOHAND, HIY"��"NOR YEL"��"NOR);

	setup_inlay();
}
