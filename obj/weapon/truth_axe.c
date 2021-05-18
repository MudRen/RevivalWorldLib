/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : truth_axe.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : �u�z����
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

string axe_message = @TEXT
                           ..
                           ;:
                          .::.
                         . ::`.                      `;.
                        :  ::  :                      ; :.
                       :   ::   :                    .;  :.
                      :    ::    :                   ::   :.
                     :     ::     :                .::    ::
                     :     ::     :               .::      ::
                     `.    ::    .               .::       ::
                      `.   ::   .               .::         ::
                        `. :: .               .::           ::
                         :    :            .:::             :;
                  _--.   :....:       ...::::               :::
              __-`    `-.:....:.---:::::::                  ::;
    ___----           ..............                        ::;
--==___===============..............                        ::;
       ----___          .........:::::..                    ::;
              ``-_    .  :\\//: ```:::::::..                :;
                  `--    ://\\:        ``:::::.             :;
                         :\\//:            ``:::.           :;
                         ://\\:               `::.         :;
                         :\\//:                :::.        :;
                         ://\\:                :::;       :;
                         :\\//:                ::::      :;
                         ://\\:                :::      :;
                         :\\//:                :::      :
                         ://\\:               :::      :
                         :\\//:               :::     :
                         ://\\:               ;;     :
                         :\\//:               ;     :
                         ://\\:              ;     :
                         [____]              ;    :
                         [____]             ;    :
                         [____]            .    :
                         [____]           .    :
                         [____]          .   .:`
                         [____]         .    :
                         :\\//:         :    :
                         ://\\:         `.   `.
                         :\\//:          `.   `.
                         ://\\:            `.   ;
                         [____]              `..
                         [____]
                         [____]
                         [____] 
                         [____]
                         [____]
                         :....:
                       .-  :: `-.
                     .     ::  ..`.
                    /      ::  `   \
                   :       ::       ;
                   ::::::::::::::::::
                   :       ::       ;
                    \      ::      /
                     `.    ::    .
                       `-..::..-
TEXT;

void create()
{
	set_idname(HIW"truth"NOR WHT" axe"NOR, HIW"�u�z"NOR WHT"����"NOR);
	set_temp("status", HIW"�v"NOR WHT"��");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "�@��j�N��r���O�����v�֥���C\n");
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
		"/obj/inlay/truth_axe_secret_art_1st" : 1,
		"/obj/inlay/truth_axe_secret_art_2nd" : 1,
	]));

	set("skill", "axe");
	
	set(ATTACK, 50);
	
	set(BUFF_FATAL_CHANCE, 3);
	set(BUFF_FATAL_POWER, 5);
	
	::setup_equipment(EQ_HAND, HIY"��"NOR YEL"��"NOR);
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
	if( !random(100) )
	{
		int energy = attacker->query_energy_cur();
		int ratio = range_random(5, 10);
		int damage;
		
		if( energy < 20 )
			damage = ratio;
		else
			damage = ratio * energy / 20;
		
		if( userp(defender) && random(3) )
			return;

		msg("$ME�j�ܤ@�n�A�ӶO "HIC+ratio+NOR CYN"%"NOR" ���믫�o�ʡu"WHT"�u�z"NOR WHT"���q"NOR"�v��$YOU�y���������ˮ`�I�I\n", attacker, defender, 1);
		
		if( energy > damage )
			attacker->cost_energy(damage / 5);
		else
			attacker->cost_energy(energy - 1);
			
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
	
	if( !random(30) )
	{
		int damage = range_random(1000, 2000);
		
		if( userp(defender) && random(3) )
			return;

		msg("$ME�o�ʡu"HIW"�u�z"NOR WHT"�g��"NOR"�v\n"HIW+axe_message+NOR"��$YOU�y���Y���ˮ`�I�I\n", attacker, defender, 1);	
	
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
