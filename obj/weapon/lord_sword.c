/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lord_sword.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : ���̤��C
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
	set_idname(HIW"chi "HIY"lord"NOR YEL" sword"NOR, HIW"��"WHT"�D"HIY"��"NOR YEL"��"HIY"��"NOR YEL"�C"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@��j�N��r���O�����v�֪��C�C\n");
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
		"/obj/inlay/lord_sword_secret_art_1st" : 1,
		"/obj/inlay/lord_sword_secret_art_2nd" : 1,
	]));

	set("skill", "sword");
	
	set(ATTACK, 20);	
	set(BUFF_AGI, 10);
	
	set(BUFF_FATAL_CHANCE, 2);
	set(BUFF_FATAL_POWER, 2);
	set(BUFF_COMBO_CHANCE, 1);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
}

//
// �Z�������T��
//
string combat_attack_message()
{
	return "$ME���R��"+this_object()->query_name()+"��$YOU�A��A";
}

void remove_attack_buff(object attacker)
{
	delete_temp(ATTACK);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

array swordart = ({ ({HIW"�s"NOR, 50}), ({HIR"�}"NOR, 60}), ({HIY"�_"NOR, 70}), ({HIM"��"NOR, 80}), ({HIB"��"NOR, 90}), ({HIG"��"NOR, 100}), ({YEL"�h"NOR, 110}), ({GRN"�I"NOR, 120}), ({MAG"��"NOR, 130}), ({HIC"��"NOR, 140}), ({WHT"��"NOR, 150}) });
//
// �Z���۰ʯS�����
//
void special_attack(object attacker, object defender)
{
	if( !random(10) )
	{
		int again = 0;
		int damage = 0;
		int total_damage = 0;
		string art;
		array damagetype;
		
		do
		{
			damage = 0;
			art = "";
			
			for(int i=0;i<6;++i)
			{
				damagetype = swordart[random(sizeof(swordart))];
				damage += damagetype[1];
				
				art += damagetype[0]+( i < 5 ? "�B":"");
			}
					
			msg("$ME�o�X�u"HIG"�j��"NOR GRN"���C"NOR"�v�A�ϥX�C�Z�u"+art+"�v��$YOU�y����H���ˮ`�I\n", attacker, defender, 1);

			if( damage < 400 )
			{
				msg("$ME�j�ܤ@�n�A�A���o�ʡu"HIG"�j��"NOR GRN"���C"NOR"�v�I�I�I�I\n", attacker, defender, 1);
				again = 1;
			}
			else
			{
				again = 0;
			}
			
			total_damage += damage;
				
		} while (again);
		
		COMBAT_D->cause_damage(attacker, defender, total_damage);
		
		return;
	}
		
	if( !random(50) )
	{
		msg("$ME�o�ʡu"HIC"�Q�D"NOR CYN"���C"NOR"�v�A�u�ɶ����Z������ "HIY"30"NOR" �I�����O�I\n", attacker, defender, 1);
		
		set_temp(ATTACK, 30);
		
		call_out((: remove_attack_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
	}
}
