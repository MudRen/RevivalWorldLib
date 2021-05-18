/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : faith_halberd.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : �H�����u
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
#include <combat.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;

void create()
{
	set_idname(HIG"faith"NOR GRN" halberd"NOR, HIG"�H��"NOR GRN"���u"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@��j�N��r���O�����v�־Դu�C\n");
	set("unit", "��");
	set("mass", 3000);
	set("value", 10000);
	set("flag/no_amount", 1);
	set("special", 1);

	set("disassemble", ([ 
		"/obj/gem/gem0" : 5,
		"/obj/gem/gem1" : 5,
		"/obj/gem/gem2" : 5,
		"/obj/gem/gem3" : 5,
		"/obj/gem/gem4" : 5,
		"/obj/gem/gem5" : 5,
		"/obj/gem/gem6" : 5,
		"/obj/inlay/faith_halberd_secret_art_1st" : 1,
		"/obj/inlay/faith_halberd_secret_art_2nd" : 1,
	]));

	set("skill", "halberd");
	
	set(ATTACK, 50);
	
	set(BUFF_ANTIFATAL_CHANCE, 3);
	set(BUFF_COUNTERATTACK_CHANCE, 5);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

void remove_defend_buff(object attacker)
{
	delete_temp(DEFEND);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU�A��A";
}

//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	if( !random(50) )
	{
		object *weapons = defender->query_equipping_object("hand");
		
		if( sizeof(weapons) ) 
		{
			int index = random(sizeof(weapons));
			msg("$ME�o�ʡu"HIG"�H��"NOR GRN"�q��"NOR"�v�A�N$YOU��"+weapons[index]->query_idname()+"�S������ϼu�^�h�I�I\n", attacker, defender, 1);
		
			return weapons[index]->special_attack(attacker, defender);
		}
	}
	
	if( !random(30) )
	{
		msg("$ME�o�ʡu"HIG"�Ƥs"NOR GRN"�u�}"NOR"�v�A�u�ɶ����Z������ "HIY"30"NOR" �I���m�O�I\n", attacker, defender, 1);
		
		set_temp(DEFEND, 30);
		
		call_out((: remove_defend_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
		
		return;	
	}
}
