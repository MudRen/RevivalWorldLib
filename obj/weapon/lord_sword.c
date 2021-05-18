/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : lord_sword.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : 王者之劍
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
	set_idname(HIW"chi "HIY"lord"NOR YEL" sword"NOR, HIW"極"WHT"．"HIY"王"NOR YEL"者"HIY"之"NOR YEL"劍"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一把古代文字中記載的史詩長劍。\n");
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
		"/obj/inlay/lord_sword_secret_art_1st" : 1,
		"/obj/inlay/lord_sword_secret_art_2nd" : 1,
	]));

	set("skill", "sword");
	
	set(ATTACK, 20);	
	set(BUFF_AGI, 10);
	
	set(BUFF_FATAL_CHANCE, 2);
	set(BUFF_FATAL_POWER, 2);
	set(BUFF_COMBO_CHANCE, 1);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

//
// 武器攻擊訊息
//
string combat_attack_message()
{
	return "$ME揮舞著"+this_object()->query_name()+"往$YOU劈砍，";
}

void remove_attack_buff(object attacker)
{
	delete_temp(ATTACK);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
}

array swordart = ({ ({HIW"連"NOR, 50}), ({HIR"破"NOR, 60}), ({HIY"斷"NOR, 70}), ({HIM"易"NOR, 80}), ({HIB"勢"NOR, 90}), ({HIG"散"NOR, 100}), ({YEL"退"NOR, 110}), ({GRN"截"NOR, 120}), ({MAG"瞬"NOR, 130}), ({HIC"揚"NOR, 140}), ({WHT"隱"NOR, 150}) });
//
// 武器自動特殊攻擊
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
				
				art += damagetype[0]+( i < 5 ? "、":"");
			}
					
			msg("$ME發出「"HIG"迴風"NOR GRN"六劍"NOR"」，使出劍訣「"+art+"」對$YOU造成驚人的傷害！\n", attacker, defender, 1);

			if( damage < 400 )
			{
				msg("$ME大喝一聲，再次發動「"HIG"迴風"NOR GRN"六劍"NOR"」！！！！\n", attacker, defender, 1);
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
		msg("$ME發動「"HIC"霸道"NOR CYN"之劍"NOR"」，短時間內武器提升 "HIY"30"NOR" 點攻擊力！\n", attacker, defender, 1);
		
		set_temp(ATTACK, 30);
		
		call_out((: remove_attack_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
	}
}
