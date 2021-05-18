/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : truth_axe.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : 真理之斧
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
	set_idname(HIW"truth"NOR WHT" axe"NOR, HIW"真理"NOR WHT"之斧"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一把古代文字中記載的史詩巨斧。\n");
	set("unit", "把");
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
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU劈砍，";
}

//
// 武器自動特殊攻擊
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

		msg("$ME大喝一聲，耗費 "HIC+ratio+NOR CYN"%"NOR" 的精神發動「"WHT"真理"NOR WHT"之吼"NOR"」對$YOU造成五倍的傷害！！\n", attacker, defender, 1);
		
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

		msg("$ME發動「"HIW"真理"NOR WHT"狂擊"NOR"」\n"HIW+axe_message+NOR"對$YOU造成嚴重傷害！！\n", attacker, defender, 1);	
	
		COMBAT_D->cause_damage(attacker, defender, damage);
		
		return;
	}
}
