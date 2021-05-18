/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : taichi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : 太極指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <skill.h>

inherit COMMAND;

string help = @HELP
   易有太極，是生兩儀。兩儀生四象，四象生八卦。

   taichi 100 [能力值] to [能力值]	- 轉換 100 點能力值至另一個能力值
   				          能力包含 stamina、health、energy
   例: taichi 300 energy to stamina	- 轉換 300 點精神至體力
   
   轉換效率隨技能等級提升。
HELP;

private void do_command(object me, string arg)
{
	int amount;
	string from, to;
	string from_name;
	int level;
	int to_amount;

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	if( !arg || sscanf(arg, "%d %s to %s", amount, from, to) != 3 )
		return tell(me, "請輸入正確的指令格式(help taichi)。\n");

 	if( me->is_fatigue_full() )
		return tell(me, pnoun(2, me)+"實在是太疲勞了，無法再進行轉換。\n");

	if( amount <= 0 )
		return tell(me, "轉換數量不可小於 0。\n");

	if( COMBAT_D->in_fight(me) && time() - query_temp("taichi_time", me) < 5 )
		return tell(me, "還需要 "+(5 - (time() - query_temp("taichi_time", me)))+" 秒後才能夠再度施展易生太極。\n");

	if( !random(10000) )
		me->add_fatigue(1);
			
	level = me->query_skill_level("taichi");
	
	if( level < 1 )
		return tell(me, pnoun(2, me)+"不懂得如何施展這個技能。\n");

	switch(from)
	{
		case "stamina":
			if( !me->cost_stamina(amount) )
				return tell(me, pnoun(2, me)+"沒有這麼多的體力。\n");
				
			from_name = "體力";
			break;
		case "health":
			if( !me->cost_health(amount) )
				return tell(me, pnoun(2, me)+"沒有這麼多的生命。\n");
				
			from_name = "生命";
			break;
		case "energy":
			if( !me->cost_energy(amount) )
				return tell(me, pnoun(2, me)+"沒有這麼多的精神。\n");
				
			from_name = "精神";
			break;
		default:
			return tell(me, "請輸入正確的轉換來源種類。\n");
	}
	
	to_amount = to_int(amount * level / 100.);

	switch(to)
	{
		case "stamina":
			me->earn_stamina(to_amount);
			msg("$ME使出「"HIW"易"NOR WHT"生"HIW"太"NOR WHT"極"NOR"」，轉換 "+amount+" 點的"+from_name+"至 "+to_amount+" 點的體力。\n", me, 0, 1);
			break;
		case "health":
			me->earn_health(to_amount);
			msg("$ME使出「"HIW"易"NOR WHT"生"HIW"太"NOR WHT"極"NOR"」，轉換 "+amount+" 點的"+from_name+"至 "+to_amount+" 點的生命。\n", me, 0, 1);
			break;
		case "energy":
			me->earn_energy(to_amount);
			msg("$ME使出「"HIW"易"NOR WHT"生"HIW"太"NOR WHT"極"NOR"」，轉換 "+amount+" 點的"+from_name+"至 "+to_amount+" 點的精神。\n", me, 0, 1);
			break;
		default:
			return tell(me, "請輸入正確的轉換目標種類。\n");
	}
	
	set_temp("taichi_time", time(), me);
}
