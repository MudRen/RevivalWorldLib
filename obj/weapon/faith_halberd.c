/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : faith_halberd.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-12-24
 * Note   : 信念之戟
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
	set_idname(HIG"faith"NOR GRN" halberd"NOR, HIG"信念"NOR GRN"之戟"NOR);
	set_temp("status", HIW"史"NOR WHT"詩");

	if( this_object()->set_shadow_ob() ) return;
	
	set("long", "一把古代文字中記載的史詩戰戟。\n");
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
		"/obj/inlay/faith_halberd_secret_art_1st" : 1,
		"/obj/inlay/faith_halberd_secret_art_2nd" : 1,
	]));

	set("skill", "halberd");
	
	set(ATTACK, 50);
	
	set(BUFF_ANTIFATAL_CHANCE, 3);
	set(BUFF_COUNTERATTACK_CHANCE, 5);
	
	::setup_equipment(EQ_HAND, HIY"單"NOR YEL"手"NOR);
}

void remove_defend_buff(object attacker)
{
	delete_temp(DEFEND);
	
	if( objectp(attacker) )
		attacker->reset_buff_cache();
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
	if( !random(50) )
	{
		object *weapons = defender->query_equipping_object("hand");
		
		if( sizeof(weapons) ) 
		{
			int index = random(sizeof(weapons));
			msg("$ME發動「"HIG"信念"NOR GRN"電轉"NOR"」，將$YOU的"+weapons[index]->query_idname()+"特殊攻擊反彈回去！！\n", attacker, defender, 1);
		
			return weapons[index]->special_attack(attacker, defender);
		}
	}
	
	if( !random(30) )
	{
		msg("$ME發動「"HIG"排山"NOR GRN"戟陣"NOR"」，短時間內武器提升 "HIY"30"NOR" 點防禦力！\n", attacker, defender, 1);
		
		set_temp(DEFEND, 30);
		
		call_out((: remove_defend_buff($(attacker)) :), 30);
			
		attacker->reset_buff_cache();
		
		return;	
	}
}
