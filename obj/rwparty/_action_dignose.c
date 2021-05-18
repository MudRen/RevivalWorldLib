/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_dignose.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-05-16
 * Note   : dignose action
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <condition.h>
#include <daemon.h>

void do_dignose(object me, string arg)
{
	object target;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"想要把鼻屎往誰的身上彈過去？\n");
	
	if( !objectp(target = present(arg)) )
		return tell(me, "這裡沒有 "+arg+" 這個東西。\n");
		
	if( !target->is_living() || target->is_module_npc() )
		return tell(me, pnoun(2, me)+"無法對這些對象彈鼻屎。\n");
		
	if( target == me )
		return tell(me, pnoun(2, me)+"無法對自己丟鼻屎。\n");
		
	if( userp(target) && !BATTLEFIELD_D->in_battle(me) )
		return tell(me, pnoun(2, me)+"只有在戰場中可以對玩家丟鼻屎。\n");

	if( !me->cost_stamina(500) )
		return tell(me, pnoun(2, me)+"沒有力氣再挖鼻孔了。\n");

	if( query_temp("dignose", me) > time() - 30 )
		return tell(me, pnoun(2, me)+"每 30 秒才能挖一次鼻孔。\n");
		
	set_temp("dignose", time(), me);
		
	msg("$ME用力地挖了幾下鼻孔，只見其指甲中暗藏的數枚濃黑顆粒突然自四面八方往$YOU連續彈射！！"HIY"１２ "NOR YEL"ＣＯＭＢＯ"NOR"！！\n", me, target, 1);

	target->start_condition(DIGNOSE, 60, 5, me);
	
	if( !random(10) )
	{
		msg("$ME的鼻孔在手指的摧殘下不堪負荷，血管爆了！\n", me, 0, 1);
		me->faint();
	}
}