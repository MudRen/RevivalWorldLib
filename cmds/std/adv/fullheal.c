/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : fullheal.c
 * Author : Sinji@RevivalWorld
 * Date   : 2001-6-10
 * Note   : fullheal 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <condition.h>

inherit COMMAND;

string help = @HELP
	標準 fullheal 指令
HELP;

private void fullheal(object ob)
{
	ob->set_all_ability_full();

	if( ob->is_faint() )
		ob->wakeup();
	
	if( ob->is_dead() )
		ob->revive();
		
	if( ob->in_condition(DIE) )
		ob->stop_condition(DIE);
	
	ob->set_all_ability_full();
}
	
private void command(object me, string arg)
{
    	object ob;

	if( !is_command() ) return;
	
    	if( !arg )
    		ob = me;
    	else if( !objectp(ob = find_player(arg)) )
    		ob = present(arg);
 
    	if( !objectp(ob) )
    		return tell(me, "沒有 "+arg+" 這種生物。\n");
    	
    	if( !ob->is_living() )
    		return tell(me, ob->query_idname()+"不是生物。\n");

    	if( ob == me )
    		msg("$ME雙掌掌心迅速凝聚了一股"HIR"血"NOR RED"紅"NOR"漩渦，又快速地吸入自己的身體。\n", me, 0, 1);
    	else		
		msg("$ME雙掌掌心迅速凝聚了一股"HIR"血"NOR RED"紅"NOR"漩渦，揮掌打進$YOU的身體。\n", me, ob, 1);
	
	msg("$ME的臉色逐紅潤了起來...。\n", ob, 0, 1);
	
	fullheal(ob);
}
